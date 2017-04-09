/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/*
For beginners tutorial (and user guide) visit:
http://www.openelectrons.com/docs/viewdoc/1
*/

#include <NXShield.h>
#include <time.h>


void delay(int ms) {
  struct timespec time;
  time.tv_sec = ms / 1000;
  time.tv_nsec = (ms % 1000) * (1000 * 1000);
  nanosleep(&time, NULL);
}


int main(int argc, char **argv) {

  // setup for this example:
  // attach external power to NXShield.
  // attach a touch sensor to BBS1
  // attach motors to motor ports (any number of motors is fine).
  // Open the Serial terminal to view.

  //
  // declare the NXShield(s) attached to your Arduino.
  //
  NXShield nxshield;

  nxshield.m_bankA.motorReset();
  nxshield.m_bankB.motorReset();

  int32_t enc_a_1 = 0;
  int32_t enc_a_2 = 0;
  int32_t enc_b_1 = 0;
  int32_t enc_b_2 = 0;

  /* repeat test n times */
  int n = 3;

  /* delay in ms between encoder polling */
  int delay_val = 300;

  /* number of times to poll encoders */
  int polling_count = 10;

  for(int i = 0; i < n; i++) {

    printf("run unlimited\n");
    nxshield.m_bankA.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, 50);
    nxshield.m_bankA.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, 50);
    nxshield.m_bankB.motorRunUnlimited(SH_Motor_1, SH_Direction_Forward, 50);
    nxshield.m_bankB.motorRunUnlimited(SH_Motor_2, SH_Direction_Forward, 50);
    nxshield.m_bankA.motorStartBothInSync();
    nxshield.m_bankB.motorStartBothInSync();

    for(int j = 0; j < polling_count; j++) {
      enc_a_1 = nxshield.m_bankA.motorGetEncoderPosition(SH_Motor_1);
      enc_a_2 = nxshield.m_bankA.motorGetEncoderPosition(SH_Motor_2);
      enc_b_1 = nxshield.m_bankB.motorGetEncoderPosition(SH_Motor_1);
      enc_b_2 = nxshield.m_bankB.motorGetEncoderPosition(SH_Motor_2);

      printf(
        "Encoder values bank A: %d, %d, bank B: %d, %d\n",
        (int)enc_a_1, (int)enc_a_2, (int)enc_b_1, (int)enc_b_2);

      delay(delay_val);
    }

    printf("stop (float)\n");
    nxshield.m_bankB.motorStop(SH_Motor_1, SH_Next_Action_Float);
    nxshield.m_bankB.motorStop(SH_Motor_2, SH_Next_Action_Float);
    nxshield.m_bankA.motorStop(SH_Motor_1, SH_Next_Action_Float);
    nxshield.m_bankA.motorStop(SH_Motor_2, SH_Next_Action_Float);

    for(int j = 0; j < polling_count; j++) {

      enc_a_1 = nxshield.m_bankA.motorGetEncoderPosition(SH_Motor_1);
      enc_a_2 = nxshield.m_bankA.motorGetEncoderPosition(SH_Motor_2);
      enc_b_1 = nxshield.m_bankB.motorGetEncoderPosition(SH_Motor_1);
      enc_b_2 = nxshield.m_bankB.motorGetEncoderPosition(SH_Motor_2);

      printf(
        "Encoder values bank A: %d, %d, bank B: %d, %d\n",
        (int)enc_a_1, (int)enc_a_2, (int)enc_b_1, (int)enc_b_2);

      delay(delay_val);
    }

  }

}
