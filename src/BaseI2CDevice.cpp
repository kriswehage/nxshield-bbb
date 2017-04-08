// BaseI2CDevice.cpp
//
// This is a base class for devices that use the I2C protocol.
//
// 2010-05-31 - Initial version, by Clinton Blackmore
//
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "BaseI2CDevice.h"

// Max I2C message length is 16 bytes.
const int BUFFER_LEN = 16;


// Initialize static variables
uint8_t* BaseI2CDevice::m_buffer = 0;
bool BaseI2CDevice::m_initialized = false;

/*! @brief constructor for BaseI2CDevice
*/
BaseI2CDevice::BaseI2CDevice() {
  m_initialized = false;
}

/*! @brief reinitialize i2c device with a possibly different address
*/
void BaseI2CDevice::init(int file, uint8_t i2c_address) {

  m_device_address = i2c_address;
  m_buffer = (uint8_t*) calloc(BUFFER_LEN, sizeof(uint8_t));
  m_file = file;
  if(ioctl(m_file, I2C_SLAVE, i2c_address) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }
  m_initialized = true;

  if(m_device_address == i2c_address) {
    return;
  } else {
    m_device_address = i2c_address;
    if(ioctl(m_file, I2C_SLAVE, i2c_address) < 0) {
      printf("Failed to acquire bus access and/or talk to slave.\n");
      /* ERROR HANDLING; you can check errno to see what went wrong */
      exit(1);
    }
  }
}

/*!
@brief Set pointer to address
*/
void BaseI2CDevice::setPointer(uint8_t i2c_address) {
  m_buffer[0] = i2c_address;

  if(write(m_file, m_buffer, 1) != 1) {
    std::cerr << "Error setting pointer" << std::endl;
  }
}


// READING FUNCTIONS

/*! Reads registers of an I2C device.
See the documentation for your device to know what a given register or register range indicates.

@param start_register start of the register range
@param bytes_to_read number of bytes to read (max 16 for lego devices)
@param buffer (optional) user-supplied buffer
@param buffer_length (optional) length of user-supplied buffer
@param clear_buffer should we zero out the buffer first? (optional)
*/
uint8_t* BaseI2CDevice::readRegisters(
  uint8_t start_register,
  uint8_t bytes_to_read,
  uint8_t* buffer,
  uint8_t buffer_length,
  bool clear_buffer) {

  if (!buffer) {
    buffer = m_buffer;
  }

  if (!buffer_length) {
    buffer_length = BUFFER_LEN;
  }

  /* avoid buffer overflow */
  bytes_to_read = std::min(bytes_to_read, buffer_length);

  if(clear_buffer) {
    memset(buffer, 0, buffer_length);
  }

  /* write to the I2C device to tell it where to read from */
  setPointer(m_device_address);

  /* Now read the data from the device */
  if(read(m_file, buffer, bytes_to_read) != bytes_to_read){
    fprintf(stderr, "Error reading %i bytes\n", bytes_to_read);
    exit(1);
  }

  return buffer;
}

// Reads a byte from the given register on the I2C device.
uint8_t BaseI2CDevice::readByte(uint8_t location) {
  readRegisters(location, 1);
  return m_buffer[0];
}

// Reads two bytes from the given register pair on the I2C device.
int16_t BaseI2CDevice::readInteger(uint8_t location) {
  readRegisters(location, 2);

  // I believe the data has the least significant byte first
  return readIntFromBuffer(m_buffer);
}

// Reads four bytes from the given registers, starting at the specified location, on the I2C device.
uint32_t BaseI2CDevice::readLong(uint8_t location) {
  readRegisters(location, 4);
  return readLongFromBuffer(m_buffer);
}

/*!@brief Reads a string.  Be certain that your buffer is large enough
 to hold the string and a trailing 'nul'!
@param location starting location of the string
@param bytes_to_read number of bytes to read
@param buffer optional user-supplied buffer
@param buffer_length length of user-supplied buffer)
*/
char* BaseI2CDevice::readString(
  uint8_t  location,
  uint8_t  bytes_to_read,
  uint8_t* buffer,
  uint8_t  buffer_length) {

  return (char *)readRegisters(
    location, bytes_to_read, buffer, buffer_length, true);
}


// WRITING FUNCTIONS

/*! Returns true if the write was successful.
If not true, you may check the result by calling getWriteErrorCode.
@param start_register start of the register range
@param bytes_to_write number of bytes to write
@param buffer optional user-supplied buffer
*/
bool BaseI2CDevice::writeRegisters(
  uint8_t start_register,
  uint8_t bytes_to_write,
  uint8_t* buffer) {

  if(!buffer) {
    buffer = m_buffer;
  }

  // Tell I2C device where to write to
  setPointer(m_device_address);

  /* Send the data */
  if(write(m_file, buffer, bytes_to_write) != bytes_to_write) {
    fprintf(
      stderr,
      "Error writing %i bytes\n",
      bytes_to_write);
    m_write_error_code = 1;
  } else {
    m_write_error_code = 0;
  }

  return m_write_error_code == 0;  // 0 indicates success
}

// Writes a byte to a given register of the I2C device
bool BaseI2CDevice::writeByte(uint8_t location, uint8_t data) {
  return writeRegisters(location, 1, &data);
}

// Writes two bytes to a given register of the I2C device
bool BaseI2CDevice::writeInteger(uint8_t location, uint16_t data) {
  writeIntToBuffer(m_buffer, data);
  return writeRegisters(location, 2, m_buffer);
}

// Writes four bytes to a given register of the I2C device
bool BaseI2CDevice::writeLong(uint8_t location, uint32_t data) {
  writeLongToBuffer(m_buffer, data);
  return writeRegisters(location, 4, m_buffer);
}

// This is the status value returned from the last write command.
// A return value of zero indicates success.
// Non-zero results indicate failures.  From libraries/Wire/utility/twi.c, they are:
//          1 .. length to long for buffer
//          2 .. address send, NACK received
//          3 .. data send, NACK received
//          4 .. other twi error (lost bus arbitration, bus error, ..)
uint8_t BaseI2CDevice::getWriteErrorCode() {
  return m_write_error_code;
}

// READ SOME INFORMATION OFF OF THE DEVICE
// returns a string with the current firmware version of the device
char* BaseI2CDevice::getFirmwareVersion() {
  return readString(0, 8);
}

// returns a string indicating the vendor of the device
char* BaseI2CDevice::getVendorID() {
  return readString(0x08, 8);
}

// returns a string indicating the device's ID
char* BaseI2CDevice::getDeviceID() {
  return readString(0x10, 8);
}

/** returns a string indicating the features on this device
 some devices may return null.
*/
char* BaseI2CDevice::getFeatureSet() {
  return readString(0x18, 8);
}

// It is very unusual to do this
void BaseI2CDevice::setWriteErrorCode(uint8_t code) {
  m_write_error_code = code;
}


/*! @brief check to see if there is a device at its specified address */
bool BaseI2CDevice::checkAddress() {

  /* write to the I2C device to tell it where to read from */
  setPointer(m_device_address);

  /* Now read the data from the device */
  if(read(m_file, m_buffer, 1) != 1){
    fprintf(stderr, "Error reading %i bytes\n", 1);
    return false;
  }
  return true;
}


/**
 * setAddress(address)
 * this function set's the i2c address
 * for this instance to given address
 * Note that, generally i2c address of a physical device does not change.
 * Use this function if there are multiple devices on your bus and you want to
 * conserve processor memory from instantiating another class instance.
 */
bool BaseI2CDevice::setAddress(uint8_t i2c_address) {
  m_device_address = i2c_address;
  return true;
}

uint8_t BaseI2CDevice::getAddress() {
  return m_device_address;
}


/*!
@brief Dump a neat table of all the hex values...similar to i2cdump
*/
void BaseI2CDevice::dump(bool as_hex) {
	int n;
  int spc;
  int gbyte;
	int line_count = 0;
	char dump_header[] = " 0123456789abcdef";

	/* Print header */
  if(as_hex) {
    spc = 2;
  } else {
    spc = 3;
  }

	for (n = 0; n < 17; n++) {
		std::cout << std::setfill(' ') << std::setw(spc) << dump_header[n] << " ";
	}

	std::cout << std::endl << "00: ";

	for (n = 0; n < 256; n++) {
    gbyte = int(readByte(n));
		if (as_hex){
			std::cout << std::setfill('0') << std::setw(2) << std::hex << gbyte <<
      " ";
		}
		else {
			std::cout << std::setfill('0') << std::setw(3) << gbyte << " ";
		}

    /* check if end of line (16 values) is reached and add a header at the beginning of the next line */
		if(n != 255 && (n+1)%16 == 0){
			std::cout << std::endl;
			line_count++;
			std::cout << std::setfill('0') << std::setw(2) << dump_header[line_count+1] << ": ";
		}
	}
  printf("\n");
}
