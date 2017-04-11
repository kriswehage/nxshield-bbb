#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include <NxShield.h>

#define MMX_CONTROL_SPEED      0x01
#define MMX_CONTROL_RAMP       0x02
#define MMX_CONTROL_RELATIVE   0x04
#define MMX_CONTROL_TACHO      0x08
#define MMX_CONTROL_BRK        0x10
#define MMX_CONTROL_ON         0x20
#define MMX_CONTROL_TIME       0x40
#define MMX_CONTROL_GO         0x80


int main() {
   NxShield nx;

   NxMotor* m1 = &nx.m_bankA.m_motor1;

   std::string buff;
   unsigned long int_buff = 0;
   std::string last_command = "";
   bool alive = true;
   while(alive){
    std::cout << ">> ";
    getline(std::cin, buff);

    if(buff == "help") {
      std::cout << "help, exit, dump, dump_int, run, stop, setET, getET, getEP, reset, setTime, setSpeed\n";
    } else if (buff == "run") {
      std::cout << "-->";
      getline(std::cin, buff);
      m1->run(buff.c_str());
      last_command = buff;
    } else if (buff == "rl") {
      m1->run(last_command.c_str());
    } else if (buff == "stop") {
      m1->stop();
      std::cout << "STOP!\n";
    } else if (buff == "setET" || buff == "st") {
      std::cout << "-->";
      getline(std::cin, buff);
      std::stringstream(buff) >> int_buff;
      m1->setEncoderTarget(int_buff);
      std::cout << "set T pos to " << buff << std::endl;
    } else if (buff == "getET" || buff == "gt") {
      std::cout << m1->getEncoderTarget() << std::endl;
    } else if (buff == "getEP" || buff == "gp") {
      std::cout << m1->getEncoderPosition() <<  std::endl;
    } else if (buff == "dump") {
      m1->m_i2c->dump(true);
    } else if (buff == "dump_int") {
      m1->m_i2c->dump(false);
    } else if (buff == "reset") {
      m1->resetEncoder();
      std::cout << "reset" << std::endl;
    } else if (buff == "setTime") {
      std::cout << "-->";
      getline(std::cin, buff);
      std::stringstream(buff) >> int_buff;
      m1->setTime(int_buff);
    } else if (buff == "setSpeed") {
      std::cout << "-->";
      getline(std::cin, buff);
      std::stringstream(buff) >> int_buff;
      m1->setSpeed(int_buff);
      m1->runSpeedControl();
    } else if (buff == "getVoltage") {
      std::cout << nx.getVoltage() << std::endl;
    }else if (buff == "exit" || buff == "q") {
      alive = false;
    } else {
      std::cout << "not a command ..." << std::endl;
    }



   }
}
