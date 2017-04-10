#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <limits>    //for bit conversion
#include <stdint.h> //to combine bytes into int

#include "NxShield.h"

NxMotor::NxMotor() {
  m_runCommand = 0xff;
  m_resetCommand = 0x72;
  m_brakeCommand = 0x41;
  m_floatCommand = 0x61;
  m_time = 0;
  m_speed = 0;
}

// writes an unsigned long int to the encoder target
void NxMotor::write_encoderTarget(unsigned int target) {
  char *somechars = (char*)&target;

  m_i2c->set_byte(m_reg_encTarget[0], somechars[0]);
  m_i2c->set_byte(m_reg_encTarget[1], somechars[1]);
  m_i2c->set_byte(m_reg_encTarget[2], somechars[2]);
  m_i2c->set_byte(m_reg_encTarget[3], somechars[3]);
}

// reads the last encoder target
int NxMotor::read_encoderTarget() {
  return m_i2c->get_long(m_reg_encTarget[0]);
  // b1 = m_i2c->get_byte(m_reg_encTarget[0]);
  // b2 = m_i2c->get_byte(m_reg_encTarget[1]);
  // b3 = m_i2c->get_byte(m_reg_encTarget[2]);
  // b4 = m_i2c->get_byte(m_reg_encTarget[3]);
  // unsigned int target = (b4 << 24) | (b3 << 16) | (b2 << 8) | (b1);
  // printf("Encoder Target: %i\n", target);
  // return(target);
}

// reads the current encoder position
int NxMotor::read_encoderPosition() {
  return m_i2c->get_long(m_reg_encPosition[0]);
}

// resets the encoder
void NxMotor::reset_encoder() {
  m_i2c->set_byte(0x41, m_resetCommand);
}

void NxMotor::set_time(unsigned int value) {
  m_i2c->set_byte(m_reg_time, value);
}

void NxMotor::set_speed(unsigned int value) {
  m_i2c->set_byte(m_reg_speed, value);
}

// takes a bit string such as '10000110' where each bit refers to a command
// attribute
void NxMotor::run(const char* comm) {
  int command_num = strtoul(comm, NULL, 2);
  std::cout << "Set run command " << comm << " or " << command_num << std::endl;
  m_i2c->set_byte(m_reg_command, command_num);
}

// stops the motor
void NxMotor::stop(bool brake) {
  if (brake) {m_i2c->set_byte(0x41, m_brakeCommand);}
  else {m_i2c->set_byte(0x41, m_floatCommand);}
}

// adds the motor1 to the bank from which the method is called
void NxBank::add_motor1() {
  m_motor1.m_i2c = &m_i2c;

  m_motor1.m_resetCommand = 0x72;
  m_motor1.m_brakeCommand = 0x41;
  m_motor1.m_floatCommand = 0x61;

  m_motor1.m_reg_command = 0x49;
  m_motor1.m_reg_time = 0x47;
  m_motor1.m_reg_speed = 0x46;

  m_motor1.m_reg_status = 0x72;
  m_motor1.m_reg_tasks = 0x76;

  m_motor1.m_reg_encTarget[0] = 0x42;
  m_motor1.m_reg_encTarget[1] = 0x43;
  m_motor1.m_reg_encTarget[2] = 0x44;
  m_motor1.m_reg_encTarget[3] = 0x45;

  m_motor1.m_reg_encPosition[0] = 0x62;
  m_motor1.m_reg_encPosition[1] = 0x63;
  m_motor1.m_reg_encPosition[2] = 0x64;
  m_motor1.m_reg_encPosition[3] = 0x65;

}

void NxBank::add_motor2() {
  m_motor2.m_i2c = &m_i2c;

  m_motor2.m_resetCommand = 0x72;
  m_motor2.m_brakeCommand = 0x41;
  m_motor2.m_floatCommand = 0x61;

  m_motor2.m_reg_command = 0x49;
  m_motor2.m_reg_time = 0x47;
  m_motor2.m_reg_speed = 0x46;

  m_motor2.m_reg_status = 0x73;
  m_motor2.m_reg_tasks = 0x77;

  m_motor2.m_reg_encTarget[0] = 0x4A;
  m_motor2.m_reg_encTarget[1] = 0x4B;
  m_motor2.m_reg_encTarget[2] = 0x4C;
  m_motor2.m_reg_encTarget[3] = 0x4D;

  m_motor2.m_reg_encPosition[0] = 0x66;
  m_motor2.m_reg_encPosition[1] = 0x67;
  m_motor2.m_reg_encPosition[2] = 0x68;
  m_motor2.m_reg_encPosition[3] = 0x69;

}


NxShield::NxShield() {
  m_bankA_address = 0x03;
  m_bankB_address = 0x04;
  m_filename = "/dev/i2c-2";
}

// initializes bankA
void NxShield::init_bankA() {
  m_bankA.m_address = m_bankA_address;
  m_bankA.m_i2c.init(m_filename, m_bankA_address);
  strncpy(m_bankA.m_vendorID, m_bankA.m_i2c.get_string(8,15).c_str(), 9);
  strncpy(m_bankA.m_firmware, m_bankA.m_i2c.get_string(0,7).c_str(), 9);
  strncpy(m_bankA.m_deviceID, m_bankA.m_i2c.get_string(16,23).c_str(), 9);
  printf("initialized bank A:\n");
  printf("  vendorID: %s\n", m_bankA.m_vendorID);
  printf("  firmware: %s\n", m_bankA.m_firmware);
  printf("  deviceID: %s\n", m_bankA.m_deviceID);
}

// initializes bankB
void NxShield::init_bankB() {
  m_bankB.m_address = m_bankB_address;
  m_bankB.m_i2c.init(m_filename, m_bankB_address);
  strncpy(m_bankB.m_firmware, m_bankB.m_i2c.get_string(0,7).c_str(), 9);
  strncpy(m_bankB.m_vendorID, m_bankB.m_i2c.get_string(8,15).c_str(), 9);
  strncpy(m_bankB.m_deviceID, m_bankB.m_i2c.get_string(16,23).c_str(), 9);
  printf("initialized bank B:\n");
  printf("  vendorID: %s\n", m_bankB.m_vendorID);
  printf("  firmware: %s\n", m_bankB.m_firmware);
  printf("  deviceID: %s\n", m_bankB.m_deviceID);
}
