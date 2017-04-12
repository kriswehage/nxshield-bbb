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

// writes a long int to the encoder target
void NxMotor::setEncoderTarget(int32_t target) {

  char* somechars = (char*)&target;
  // m_i2c->set_long(m_reg_encTarget[0], target);
  m_i2c->set_byte(m_reg_encTarget[0], somechars[0]);
  m_i2c->set_byte(m_reg_encTarget[1], somechars[1]);
  m_i2c->set_byte(m_reg_encTarget[2], somechars[2]);
  m_i2c->set_byte(m_reg_encTarget[3], somechars[3]);
}

// reads the last encoder target
int NxMotor::getEncoderTarget() {
  return m_i2c->get_long(m_reg_encTarget[0]);
  // char b[4];
  // b[0] = m_i2c->get_byte(m_reg_encTarget[0]);
  // b[1] = m_i2c->get_byte(m_reg_encTarget[1]);
  // b[2] = m_i2c->get_byte(m_reg_encTarget[2]);
  // b[3] = m_i2c->get_byte(m_reg_encTarget[3]);
  // unsigned int target = (b4 << 24) | (b3 << 16) | (b2 << 8) | (b1);
  // printf("Encoder Target: %i\n", target);
  // return(target);
}

// reads the current encoder position
int NxMotor::getEncoderPosition() {
  return m_i2c->get_long(m_reg_encPosition[0]);
}

// resets the encoder
void NxMotor::resetEncoder() {
  m_i2c->set_byte(0x41, m_resetCommand);
}

void NxMotor::setTime(unsigned int value) {
  m_i2c->set_byte(m_reg_time, value);
}

void NxMotor::setSpeed(unsigned int value) {
  m_i2c->set_byte(m_reg_speed, value);
}

// takes a bit string such as '10000110' where each bit refers to a command
// attribute
void NxMotor::run(const char* comm) {
  m_command = strtoul(comm, NULL, 2);
  std::cout << "Set run command " << comm << " or " << m_command << std::endl;
  m_i2c->set_byte(m_reg_command, m_command);
}

#define MMX_CONTROL_SPEED      0x01
#define MMX_CONTROL_RAMP       0x02
#define MMX_CONTROL_RELATIVE   0x04
#define MMX_CONTROL_TACHO      0x08
#define MMX_CONTROL_BRK        0x10
#define MMX_CONTROL_ON         0x20
#define MMX_CONTROL_TIME       0x40
#define MMX_CONTROL_GO         0x80

void NxMotor::runSpeedControl(
  bool ramp,
  bool brakeOnCompletion,
  bool timedControl) {

  m_command = MMX_CONTROL_SPEED | MMX_CONTROL_GO;

  if(ramp) {
    m_command |= MMX_CONTROL_RAMP;
  }
  if(brakeOnCompletion) {
    m_command |= MMX_CONTROL_BRK;
  }
  if(timedControl) {
    m_command |= MMX_CONTROL_TIME;
  }
  m_i2c->set_byte(m_reg_command, m_command);
}

void NxMotor::runPositionControl(
  bool ramp,
  bool relative, // relative or absolute encoder values
  bool brakeOnCompletion,
  bool holdOnCompletion, // hold encoder position on completion
  bool timedControl) {

  m_command = MMX_CONTROL_TACHO | MMX_CONTROL_GO;

  if(ramp) {
    m_command |= MMX_CONTROL_RAMP;
  }
  if(relative) {
    m_command |= MMX_CONTROL_RELATIVE;
  }
  if(brakeOnCompletion) {
    m_command |= MMX_CONTROL_BRK;
  }
  if(holdOnCompletion) {
    m_command |= MMX_CONTROL_ON;
  }
  if(timedControl) {
    m_command |= MMX_CONTROL_TIME;
  }
  m_i2c->set_byte(m_reg_command, m_command);
}

void NxMotor::runSpeedAndPositionControl(
  bool ramp,
  bool relative,
  bool brakeOnCompletion,
  bool holdOnCompletion,
  bool timedControl) {

  m_command =
    MMX_CONTROL_TACHO |
    MMX_CONTROL_GO |
    MMX_CONTROL_SPEED;

  if(ramp) {
    m_command |= MMX_CONTROL_RAMP;
  }
  if(relative) {
    m_command |= MMX_CONTROL_RELATIVE;
  }
  if(brakeOnCompletion) {
    m_command |= MMX_CONTROL_BRK;
  }
  if(holdOnCompletion) {
    m_command |= MMX_CONTROL_ON;
  }
  if(timedControl) {
    m_command |= MMX_CONTROL_TIME;
  }
  m_i2c->set_byte(m_reg_command, m_command);

}


// stops the motor
void NxMotor::stop(bool brake) {
  if (brake) {
    m_i2c->set_byte(0x41, m_brakeCommand);
  } else {
    m_i2c->set_byte(0x41, m_floatCommand);
  }
}

// adds the motor1 to the bank from which the method is called
void NxBank::addMotor1() {
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

void NxBank::addMotor2() {
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

/** set the PID control for the encoders */
void NxBank::setEncoderPID(uint16_t Kp, uint16_t Ki, uint16_t Kd) {

}

/** set the PID control for the speed of the motors */
void NxBank::setSpeedPID(uint16_t Kp, uint16_t Ki, uint16_t Kd) {

}


NxShield::NxShield() {
  m_bankA_address = 0x03;
  m_bankB_address = 0x04;
  m_filename = "/dev/i2c-2";
  initBankA();
  initBankB();
}

// initializes bankA
void NxShield::initBankA() {
  m_bankA.m_address = m_bankA_address;
  m_bankA.m_i2c.init(m_filename, m_bankA_address);
  strncpy(m_bankA.m_vendorID, m_bankA.m_i2c.get_string(8,15).c_str(), 9);
  strncpy(m_bankA.m_firmware, m_bankA.m_i2c.get_string(0,7).c_str(), 9);
  strncpy(m_bankA.m_deviceID, m_bankA.m_i2c.get_string(16,23).c_str(), 9);
  printf("initialized bank A:\n");
  printf("  vendorID: %s\n", m_bankA.m_vendorID);
  printf("  firmware: %s\n", m_bankA.m_firmware);
  printf("  deviceID: %s\n", m_bankA.m_deviceID);

  m_bankA.addMotor1();
  m_bankA.addMotor2();
}

// initializes bankB
void NxShield::initBankB() {
  m_bankB.m_address = m_bankB_address;
  m_bankB.m_i2c.init(m_filename, m_bankB_address);
  strncpy(m_bankB.m_firmware, m_bankB.m_i2c.get_string(0,7).c_str(), 9);
  strncpy(m_bankB.m_vendorID, m_bankB.m_i2c.get_string(8,15).c_str(), 9);
  strncpy(m_bankB.m_deviceID, m_bankB.m_i2c.get_string(16,23).c_str(), 9);
  printf("initialized bank B:\n");
  printf("  vendorID: %s\n", m_bankB.m_vendorID);
  printf("  firmware: %s\n", m_bankB.m_firmware);
  printf("  deviceID: %s\n", m_bankB.m_deviceID);

  m_bankB.addMotor1();
  m_bankB.addMotor2();

}

int NxShield::getVoltage() {
  char voltage = m_bankA.m_i2c.get_byte(0x41);
  return (int) voltage * 37;
}
