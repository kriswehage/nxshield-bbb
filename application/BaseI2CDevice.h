#ifndef I2C_H
#define I2C_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


class i2c_module {
	int file;

	private:
  	void set_address(int);

	public:
      unsigned char buf[10];
      int init(const char *, int);
      unsigned char get_byte(int);
      unsigned char get_char(int);
			unsigned int get_uint(int);
			int get_int(int);
      void set_byte(int, int);
      void dump(bool as_hex=false);
      std::string get_string(int, int);
};

#endif
