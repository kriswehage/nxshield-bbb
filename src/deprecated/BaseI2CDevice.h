
// I2CDevice.h
//
// This is a class for controlling some devices that
// communicate using the I2C protocol.
//
// Use at your own risk!
//
// Initial version: 2010-05-31 by Clinton Blackmore

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

/* Change log
* 07/07/11    Deepak Patil        Added getFeatureSet.
*/

#ifndef _BASEI2CDEVICE_H_
#define _BASEI2CDEVICE_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include "SHDefines.h"

/** parse the two bytes in the buffer into an integer */
inline uint16_t readIntFromBuffer(uint8_t* buf) {
	return buf[0] | (buf[1] << 8);
}

/** parse the four bytes in the buffer into an integer of type long */
inline uint32_t readLongFromBuffer(uint8_t* buf) {
	return buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
}

/** write the data as a byte to the supplied buffer */
inline void writeByteToBuffer(uint8_t* buf, uint8_t data) {
	buf[0] = data;
}

inline void writeByteToBuffer(uint8_t* buf, int8_t data) {
	writeByteToBuffer(buf, (uint8_t)data);
}

/** write unsigned two byte integer to the supplied buffer */
inline void writeIntToBuffer(uint8_t* buf, uint16_t data) {
	buf[0] = data & 0xFF;
	buf[1] = (data >> 8) & 0xFF;
}

/** write two byte integer to the supplied buffer */
inline void writeIntToBuffer(uint8_t* buf, int16_t data) {
	writeIntToBuffer(buf, (uint16_t)data);
}

/** write unsigned four byte integer of type long to the supplied buffer */
inline void writeLongToBuffer(uint8_t* buf, uint32_t data) {
	buf[0] = data & 0xFF;
	buf[1] = (data >> 8) & 0xFF;
	buf[2] = (data >> 16) & 0xFF;
	buf[3] = (data >> 24) & 0xFF;
}

/** write four byte integer of type long to the supplied buffer */
inline void writeLongToBuffer(uint8_t* buf, int32_t data) {
	writeLongToBuffer(buf, (uint32_t)data);
}

/**
  @brief This class implements hardware I2C protocol used by NXShield on an
	Arduino

	Note that this class is a base class, but not an abstract base class
	Feel free to instantiate I2CDevice.
	*/
class BaseI2CDevice {
public:

	/** constructor for the I2CDevice class; requires the path to the file pointer for the i2c bus that was created in NXShield and the address of the	i2c device */
	BaseI2CDevice();

	void init(int file, uint8_t i2c_address);

	/** read specified number of bytes from the start register.
	 @param start_register location to start reading from
	 @param bytes_to_read Number of bytes to read (max 16 for LEGO compatible devices)
	 @param buffer (optional) buffer to read the data into
	 @param buffer_length (optional) length of the buffer if it was provided
	 @param clear_buffer (optional) to clear the buffer or not before using.
	 @return pointer to data buffer that was read. If buffer was not provided, this is internal pointer.
	*/
	uint8_t* readRegisters(
		uint8_t start_register,
		uint8_t bytes_to_read,
		uint8_t* buffer = 0,
		uint8_t buffer_length = 0,
		bool clear_buffer = false);

	void setPointer(uint8_t i2c_address);

	/** Read a byte from specified location
	 @param location address to read at
	 @return  a byte value read from the location
	*/
	uint8_t readByte(uint8_t location);

	/** Read an integer from specified location. Integer comprises of 2 bytes.
	 @param location address to read at
	 @return  an integer value read from the location
	*/
	int16_t readInteger(uint8_t location);

	/** Read a long from specified location. Long comprises of 4 bytes.
	 @param location address to read at
	 @return  a long value read from the location
	*/
	uint32_t readLong(uint8_t location);

	/** Read a string from specified location
	 @param location address to read at
	 @param bytes_to_read  number of bytes to read
	 @param buffer optional, a buffer to read the data into.
	 @param buffer_length optional, length of the buffer supplied.
	 @return  a char array read from the location
	*/
	char* readString(
		uint8_t location,
		uint8_t bytes_to_read,
		uint8_t* buffer = 0,
		uint8_t buffer_length = 0);

	/** write data bytes to the i2c device starting from the start register
	@param start_register location to write at.
	@param bytes_to_write Number of bytes to write
	@param buffer (optional) data buffer, if not supplied, data from internal buffer is used.
	*/
	bool writeRegisters(
		uint8_t start_register,
		uint8_t bytes_to_write,
		uint8_t* buffer = 0);

	/** write one byte to the specified register location
	@param location location to write to.
	@param data the data to write.
	*/
	bool writeByte(uint8_t location, uint8_t data);

	/** write two bytes (int) to the specified register location
	@param location location to write to.
	@param data the data to write.
	*/
	bool writeInteger(uint8_t location, uint16_t data);

	/** write four bytes (long) to the specified register location
	@param location location to write to.
	@param data the data to write.
	*/
	bool writeLong(uint8_t location, uint32_t data);

	/** validate if a device is attached to the i2c bus with the specified i2c address */
	bool checkAddress();

	/** set the i2c address for this device
	@param i2c_address new device address.
	*/
	bool setAddress(uint8_t i2c_address);

	/** returns the current address for this instance of I2CDevice */
	uint8_t getAddress();

	/** returns the error code for an error with the Wire.h library on the i2c bus */
	uint8_t	getWriteErrorCode();

	/** return the firware version id of the device */
	char*	getFirmwareVersion();

	/** return the name of the vendor for the device */
	char*	getVendorID();

	/** get the name of the device */
	char*	getDeviceID();

	/** returns the features on the device, not supported by all devices */
	char*	getFeatureSet();

	void dump(bool as_hex);

	/** Buffer used for data that is returned from I2C commands */
	static uint8_t* m_buffer;

	static bool m_initialized;

protected:

	/** write the internal error code */
	void setWriteErrorCode(uint8_t code);

private:

	int m_file; /*!< file pointer to write i2c commands to */
	uint8_t m_device_address;	/*!< I2C address of the I2C device */
	uint8_t m_write_error_code;	/*!< Error code from last write */
};

#endif // I2CDevice_H
