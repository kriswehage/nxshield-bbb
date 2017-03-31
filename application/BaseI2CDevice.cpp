
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

#include "i2c_utils.h"


/*!
@brief Initializes I2Cdevice
*/
int BaseI2CDevice::init(const char * filename, int addr) {
  if((file = open(filename, O_RDWR)) < 0) {
    printf("Failed to open the bus.");
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }

  if(ioctl(file, I2C_SLAVE,addr) < 0) {
    printf("Failed to acquire bus access and/or talk to slave.\n");
    /* ERROR HANDLING; you can check errno to see what went wrong */
    exit(1);
  }
  return file;
}

/*!
@brief Set pointer to address
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
unsigned char BaseI2CDevice::get_byte(int add1) {
  set_address(add1);

  if (read(file, buf, 1) != 1){
    fprintf(stderr, "Error reading %i bytes\n",1);
    exit(1);
  } else {
    return buf[0];
  }
}


/*!
@brief Get byte value at address
*/
unsigned int BaseI2CDevice::get_uint(int add1) {
  set_address(add1);

  if (read(file, buf, 1) != 1){
    fprintf(stderr, "Error reading %i bytes\n",1);
    exit(1);
  } else {
    unsigned char b1, b2, b3, b4;
    b1 = get_byte(reg_encTarget[0]);
    b2 = get_byte(reg_encTarget[1]);
    b3 = get_byte(reg_encTarget[2]);
    b4 = get_byte(reg_encTarget[3]);
    unsigned int value = (b4 << 24) | (b3 << 16) | (b2 << 8) | (b1);
    return value;
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
