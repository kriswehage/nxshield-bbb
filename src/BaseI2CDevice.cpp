
/*
#include <glib.h>
#include <glib/gprintf.h>
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


/* Initialize static variables */
uint8_t* BaseI2CDevice::_buffer = 0;
bool BaseI2CDevice::b_initialized = false;


BaseI2CDevice::BaseI2CDevice(uint8_t i2c_address)
{

  b_initialized = false;

  /* From the original nxshield version:  As I understand it, an I2C bus can address 127 different devices (a 7-bit quantity).
  When used, the 7-bit quantity is shifted right one bit, and the last bit is clear
  for a read operation and set for a write operation.  Arduino's Wire library expects
  a 7-bit address, but most tech specs list the 8-bit address.  Hence, we drop
  the least significant bit (and Wire.h shifts the address and sets the read/write
  bit as appropriate.)

  Removing bit shifting as it may not be necessary with i2c-dev library */
  //_device_address = i2c_address >> 1;
  _device_address = i2c_address;

  _buffer = (uint8_t*) calloc(BUFFER_LEN, sizeof(uint8_t));
}

/*!
@brief Initializes I2Cdevice
*/
int BaseI2CDevice::init(const char* filename, int addr) {
  if((file = open(filename, O_RDWR)) < 0) {
    printf("Failed to open the bus.");
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }

  if(ioctl(file, I2C_SLAVE, addr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }
  return file;
}

// bankA_address = 0x03;
// bankB_address = 0x04;
// filename = "/dev/i2c-2";

/*!
@brief Set address and verify that
*/
void BaseI2CDevice::set_address(int address) {
  buf[0] = address;

  if(write(file, buf, 1) != 1) {
    fprintf(stderr, "Error setting address\n");
  }
}

/*!
@brief Get byte value at address
*/
unsigned char BaseI2CDevice::get_byte(int address) {
  buf[0] = address;
  if (read(file, buf, 1) != 1){
    fprintf(stderr, "Error reading %i bytes\n",1);
    exit(1);
  } else {
    return buf[0];
  }
}

/*!
@brief Set byte value at address

FiX: For consistency, this function should take unsigned char addresses.
However, we need to make sure that this indeed makes sense.
*/
void BaseI2CDevice::set_byte(int address, int value) {
  buf[0] = address;
  buf[1] = value;
  if (write(file, buf, 2) != 2) {
    fprintf(stderr, "Error writing %i bytes\n", 1);
  }
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
  if (as_hex) {
    spc = 2;
  } else {
    spc = 3;
  }

	for (n = 0; n < 17; n++) {
		std::cout << std::setfill(' ') << std::setw(spc) << dump_header[n] << " ";
	}

	std::cout << std::endl << "00: ";

	for (n = 0; n < 256; n++) {
    gbyte = int(get_byte(n));
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

std::string BaseI2CDevice::get_string(int start, int stop) {
  int n;
  std::string final_string;
  for (n = start; n < stop+1; n++) {
    final_string.append(1,get_byte(n));
  }
  return(final_string);
}
