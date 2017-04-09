#ifndef _BASEI2CDEVICE_H_
#define _BASEI2CDEVICE_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


class BaseI2CDevice {

	public:

		BaseI2CDevice() {
			m_initialized = false;
		};
    int init(const char*, int);
    unsigned char get_byte(int);
    unsigned char get_char(int);
		unsigned int get_uint(int);
		int get_int(int);
		unsigned long get_long(int);
    void set_byte(int, int);
    void dump(bool as_hex=false);
    std::string get_string(int, int);

	private:

		void set_address(int);
		int m_file;
		unsigned char m_buffer[10];
		bool m_initialized;

};

#endif
