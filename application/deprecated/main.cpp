#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "nxtalk.h"

int main() {
   nxshield nx;
   nx.init_bankA();
   nx.bankA.add_motor1();

   nxmotor* m1 = &nx.bankA.motor1;

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
      m1->write_encoderTarget(int_buff);
      std::cout << "set T pos to " << buff << std::endl;
    } else if (buff == "getET" || buff == "gt") {
      m1->read_encoderTarget();
    } else if (buff == "getEP" || buff == "gp") {
      m1->read_encoderPosition();
    } else if (buff == "dump") {
      m1->i2c->dump(true);
    } else if (buff == "dump_int") {
      m1->i2c->dump(false);
    } else if (buff == "reset") {
      m1->reset_encoder();
      std::cout << "reset" << std::endl;
    } else if (buff == "setTime") {
      std::cout << "-->";
      getline(std::cin, buff);
      std::stringstream(buff) >> int_buff;
      m1->set_time(int_buff);
    } else if (buff == "setSpeed") {
      std::cout << "-->";
      getline(std::cin, buff);
      std::stringstream(buff) >> int_buff;
      m1->set_speed(int_buff);
    } else if (buff == "exit") {
      alive = false;
    } else if (buff == "q") {
      m1->stop();
      alive = false;
    } else {
      std::cout << "not a command ..." << std::endl;
    }

   }
}
