#ifndef _NXSHIELD_H_
#define _NXSHIELD_H_

#include "BaseI2CDevice.h"

#define MMX_CONTROL_SPEED      0x01
#define MMX_CONTROL_RAMP       0x02
#define MMX_CONTROL_RELATIVE   0x04
#define MMX_CONTROL_TACHO      0x08
#define MMX_CONTROL_BRK        0x10
#define MMX_CONTROL_ON         0x20
#define MMX_CONTROL_TIME       0x40
#define MMX_CONTROL_GO         0x80

#define MMX_COMMAND     0x41
#define MMX_VOLTAGE     0x41

#define MMX_SETPT_M1     0x42
#define MMX_SPEED_M1     0x46
#define MMX_TIME_M1      0x47
#define MMX_CMD_B_M1     0x48
#define MMX_CMD_A_M1     0x49

#define MMX_SETPT_M2     0x4A
#define MMX_SPEED_M2     0x4E
#define MMX_TIME_M2      0x4F
#define MMX_CMD_B_M2     0x50
#define MMX_CMD_A_M2     0x51

/*
 * Read registers.
 */
#define MMX_POSITION_M1  0x62
#define MMX_POSITION_M2  0x66
#define MMX_STATUS_M1    0x72
#define MMX_STATUS_M2    0x73
#define MMX_TASKS_M1     0x76
#define MMX_TASKS_M2     0x77

#define MMX_ENCODER_PID  0x7A
#define MMX_SPEED_PID  0x80
#define MMX_PASS_COUNT  0x86
#define MMX_TOLERANCE  0x87

/* constants to be used by user programs */
/*
 * Motor selection related constants
 */
#define MMX_Motor_1                0x01
#define MMX_Motor_2                0x02
#define MMX_Motor_Both             0x03

/*
 * Next action related constants
 */
// stop and let the motor coast.
#define MMX_Next_Action_Float      0x00
// apply brakes, and resist change to tachometer
#define MMX_Next_Action_Brake      0x01
// apply brakes, and restore externally forced change to tachometer
#define MMX_Next_Action_BrakeHold  0x02

/*
 * Direction related constants
 */
#define MMX_Direction_Forward      0x01
#define MMX_Direction_Reverse      0x00

/*
 * Tachometer related constants
 */
#define MMX_Move_Relative 0x01
#define MMX_Move_Absolute 0x00

#define MMX_Completion_Wait_For    0x01
#define MMX_Completion_Dont_Wait   0x00

enum motor_enum_t {
  MOTOR1,
  MOTOR2,
  BOTH_MOTORS
};

class NxMotor {
  public:
    NxMotor();

    BaseI2CDevice* m_i2c;

    unsigned char m_command;

    unsigned char m_resetCommand;
    unsigned char m_brakeCommand;
    unsigned char m_floatCommand;
    unsigned char m_runCommand;

    unsigned char m_speed;
    unsigned char m_time;

    unsigned char m_reg_encTarget[4];
    unsigned char m_reg_encPosition[4];
    unsigned char m_reg_speed;
    unsigned char m_reg_time;
    unsigned char m_reg_command;
    unsigned char m_reg_status;
    unsigned char m_reg_tasks;

    unsigned char m_status_byte;
    unsigned char m_tasks_running_byte;

    struct m_status {
      bool speedControlOn;
      bool motorRamping;
      bool motorPowered;
      bool positionControlOn;
      bool brakeMode;
      bool overLoaded;
      bool timedMode;
      bool stalled;
    }

    int getEncoderTarget();
    void setEncoderTarget(int);
    int getEncoderPosition();
    void resetEncoder();
    void setTime(unsigned int);
    void setSpeed(int);
    void run(const char*);
    void setCommand(
      bool ramp = false,
      bool relative = false,
      bool brakeOnCompletion = false,
      bool holdOnCompletion = false,
      bool timedControl = false);
    void runSpeedControl(
      bool ramp = false,
      bool brakeOnCompletion = false,
      bool timedControl = false);
    void runPositionControl(
      bool ramp = false,
      bool relative = false,
      bool brakeOnCompletion = false,
      bool holdOnCompletion = false,
      bool timedControl = false);
    void runSpeedAndPositionControl(
      bool ramp = false,
      bool relative = false,
      bool brakeOnCompletion = false,
      bool holdOnCompletion = false,
      bool timedControl = false);
    void stop(bool brake = false);
};

class NxBank {
  public:
    NxBank() {

    }

    unsigned int m_address;
    char m_firmware[9];
    char m_vendorID[9];
    char m_deviceID[9];

    BaseI2CDevice m_i2c;
    NxMotor m_motor1;
    NxMotor m_motor2;

    void addMotor1();
    void addMotor2();

    /** set the PID control for the encoders */
  	void setEncoderPID(uint16_t Kp, uint16_t Ki, uint16_t Kd);

  	/** set the PID control for the speed of the motors */
  	void setSpeedPID(uint16_t Kp, uint16_t Ki, uint16_t Kd);

  private:

    /* register addresses */
    static const unsigned char m_position_Kp = 0x7a;
    static const unsigned char m_position_Ki = 0x7c;
    static const unsigned char m_position_Kd = 0x7e;
    static const unsigned char m_speed_Kp = 0x80;
    static const unsigned char m_speed_Ki = 0x82;
    static const unsigned char m_speed_Kd = 0x84;
};

class NxShield {
  public:
    unsigned char m_bankA_address;
    unsigned char m_bankB_address;
    const char* m_filename;

    NxBank m_bankA;
    NxBank m_bankB;

    NxShield();
    void initBankA();
    void initBankB();
    int getVoltage();

};


#endif
