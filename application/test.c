int main() {
  unsigned char target[4];


  target[0] = 0x42;
  target[1] = 0x43;
  target[2] = 0x44;
  target[3] = 0x45;

  int result = (target[3] << 24) | (target[2] << 16) | (target[1] << 8) | (target[0]);
}
