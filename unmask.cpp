/*
#include <iostream>

char test[] = { (char)0x81, (char)0x84, 0x0b, 0x37, 0x55, (char)0xa6, 0x4e, 0x6f, 0x1c, (char)0xf2, 0 };

using namespace std;

char *unmask(char *p);

int main() {
  char *s = unmask(test);
  if (s) cout << s << endl;
  return 0;
}

//*/

char *unmask(char *p) {
  int mask = 0, len = p[1] & 127;
  char *q;
  if (*p != (char)0x81) return 0;
  if (p[1] & 0x80) {
    q = &p[6];
    p = &p[2];
    for (int i = 0; i < len; i++) {
      q[i] ^= p[mask++];
      mask &= 3;
    }
  } else {
    q = &p[2];
    q[len] = 0;
  }
  return q;
}

