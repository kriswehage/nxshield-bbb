
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


/*!
@brief Initializes I2Cdevice
*/
int BaseI2CDevice::init(const char * filename, int addr) {
  if((m_file = open(filename, O_RDWR)) < 0) {
    printf("Failed to open the bus.");
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }

  if (ioctl(m_file, I2C_SLAVE, addr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }
  return m_file;
}

/*!
@brief Set pointer to address
*/
void BaseI2CDevice::set_pointer(int address) {
  m_buffer[0] = address;

  if(write(m_file, m_buffer, 1) != 1) {
    fprintf(stderr, "Error setting pointer\n");
  }
}

/*!
@brief Get byte value at address
*/
unsigned char BaseI2CDevice::get_byte(int add1) {
  set_pointer(add1);

  if (read(m_file, m_buffer, 1) != 1){
    fprintf(stderr, "Error reading %i bytes\n", 1);
    exit(1);
  } else {
    return m_buffer[0];
  }
}

/*!
@brief Get byte value at address
*/
unsigned char* BaseI2CDevice::get_bytes(int add1, int bytes) {
  set_pointer(add1);

  if (read(m_file, m_buffer, bytes) != bytes){
    fprintf(stderr, "Error reading %i bytes\n", bytes);
    exit(1);
  } else {
    return m_buffer;
  }
}


/*!
@brief Get ulong value at address
*/
uint32_t BaseI2CDevice::get_ulong(int add1) {
  set_pointer(add1);

  unsigned char* buf = get_bytes(add1, 4);
  return *((uint32_t*)buf);
}


/*!
@brief Get long value at address
*/
int32_t BaseI2CDevice::get_long(int add1) {
  set_pointer(add1);

  unsigned char* buf = get_bytes(add1, 4);
  return *((int32_t*)buf);
}

/*!
@brief Get uint value at address
*/
uint16_t BaseI2CDevice::get_uint(int add1) {
  set_pointer(add1);

  unsigned char* buf = get_bytes(add1, 2);
  return *((uint16_t*)buf);
}


/*!
@brief Get uint value at address
*/
bool BaseI2CDevice::set_uint(int address, uint16_t value) {
  m_buffer[0] = address;
  memcpy(m_buffer + 1, &value, sizeof(int16_t));
  // int32_t temp = (int32_t)value;
  // char* temp2 = (char*)&temp;
  // for(int i = 1; i < 5; i++) {
  //   m_buffer[i] = temp2[i - 1];
  // }
  if (write(m_file, m_buffer, 3) != 3) {
    fprintf(stderr, "Error writing %i bytes\n", 2);
  }
}

/*!
@brief Get int at address
*/
int16_t BaseI2CDevice::get_int(int add1) {
  set_pointer(add1);

  unsigned char* buf = get_bytes(add1, 2);
  return *((int16_t*)buf);
}


/*!
@brief Set byte value at address

FiX: For consistency, this function should take unsigned char addresses.
However, we need to make sure that this indeed makes sense.
*/
void BaseI2CDevice::set_byte(int address, char value) {
  m_buffer[0] = address;
  m_buffer[1] = value;
  if (write(m_file, m_buffer, 2) != 2) {
    fprintf(stderr, "Error writing %i bytes\n", 1);
  }
}

/*!
@brief Set byte value at address

FiX: For consistency, this function should take unsigned char addresses.
However, we need to make sure that this indeed makes sense.
*/
void BaseI2CDevice::set_long(int address, int value) {
  m_buffer[0] = address;
  memcpy(m_buffer + 1, &value, sizeof(int32_t));
  // int32_t temp = (int32_t)value;
  // char* temp2 = (char*)&temp;
  // for(int i = 1; i < 5; i++) {
  //   m_buffer[i] = temp2[i - 1];
  // }
  if (write(m_file, m_buffer, 5) != 5) {
    fprintf(stderr, "Error writing %i bytes\n", 4);
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
  for (n = start; n < stop + 1; n++) {
    final_string.append(1, get_byte(n));
  }
  return(final_string);
}
