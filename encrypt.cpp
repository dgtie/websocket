/*
#include <iostream>

bool encrypt(char *in, char *out);

using namespace std;

char response[] =
"HTTP/1.1 101 Switching Protocols\r\n"
"Upgrade: websocket\r\n"
"Connection: Upgrade\r\n"
"Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n" "\r\n";
//"Sec-WebSocket-Protocol: chat\r\n\r\n";
//97 = s3pPL...

int main() {
  char test[] = "dGhlIHNhbXBsZSBub25jZQ==";
  encrypt(test, &response[97]);
  printf("%s\n%d\n", response, (int)sizeof(response));
  return 0;
}
//*/

#include "sha1.h"

namespace
{

char code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
              "abcdefghijklmnopqrstuvwxyz"
              "0123456789+/";

void print4(unsigned u, char *p) {
  p[0] = code[(u >> 18) & 63];
  p[1] = code[(u >> 12) & 63];
  p[2] = code[(u >> 6) & 63];
  p[3] = code[u & 63];
}

void print3(unsigned u, char *p) {
  p[0] = code[(u >> 18) & 63];
  p[1] = code[(u >> 12) & 63];
  p[2] = code[(u >> 6) & 63];
  p[3] = '=';
}

void base64(char *string, unsigned *md) {
  unsigned h0 = md[0] >> 8;
  unsigned h1 = (md[0] << 16) | (md[1] >> 16);
  unsigned h2 = (md[1] << 8) | (md[2] >> 24);
  unsigned h3 = md[2];
  unsigned h4 = md[3] >> 8;
  unsigned h5 = (md[3] << 16) | (md[4] >> 16);
  unsigned h6 = md[4] << 8;
  print4(h0, string); print4(h1, &string[4]); print4(h2, &string[8]);
  print4(h3, &string[12]); print4(h4, &string[16]);
  print4(h5, &string[20]); print3(h6, &string[24]);
}

} //anonymous

bool encrypt(char *in, char *out) {
  char magic[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  SHA1 sha;
  unsigned message_digest[5];
  sha.Input(in, 24);
  sha.Input(magic, 36);
  if (!sha.Result(message_digest)) return false;
  base64(out, message_digest);
  return true;
}
