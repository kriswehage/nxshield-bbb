#ifndef _BASEI2CDEVICE_H_
#define _BASEI2CDEVICE_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>


class BaseI2CDevice {

	public:

		BaseI2CDevice():
			m_initialized(false) {
		};
    int init(const char*, int);
    unsigned char get_byte(int);
		unsigned char* get_bytes(int, int);
    unsigned char* get_char(int);
		uint16_t get_uint(int);
		int16_t get_int(int);
		uint32_t get_ulong(int);
		int32_t get_long(int);
    void set_byte(int, char);
		void set_long(int, int);
		void set_ulong(int, unsigned int);
		void set_int(int, int16_t);
		void set_uint(int, uint16_t);
    void dump(bool as_hex=false);
    std::string get_string(int, int);

	private:

		void set_pointer(int);
		int m_file;
		unsigned char m_buffer[10];
		bool m_initialized;

};

#endif
