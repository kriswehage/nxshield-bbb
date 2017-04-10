#include <inttypes.h>

int main() {
  int32_t i = 225;
  char* b = (char*)&i;

  char b2[4];

  printf("i: %d\n", *((int32_t*)b));

  for(int i = 0; i < 4; i++) {
    b2[i] = b[i];
  }

  int j = (b2[0] << 24) | (b2[1] << 16) | (b2[2] << 8) | (b2[3]);
  printf("j: %d\n", j);
}
