/*
#include <iostream>

char test[] =
"GET /chat HTTP/1.1\r\n"
"Host: server.example.com\r\n"
"Upgrade: websocket\r\n"
"Connection: Upgrade\r\n"
"Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\n"
"Origin: http://example.com\r\n"
"Sec-WebSocket-Protocol: chat, superchat\r\n"
"Sec-WebSocket-Version: 13\r\n";

int getKey(char **string);

using namespace std;

int main() {
  char *string = test;
  int i = getKey(&string);
  if (i) {
    string[i] = 0;
    cout << string << endl;
  }
  return 0;
}
//*/

namespace
{

bool isSpace(char c) {
  if (c == ' ') return true;
  if (c == '\t') return true;
  if (c == '\r') return true;
  return c == '\n' ? true : false;
}

} //anonymous

int get_word(char **string) {
  int i = 0;
  while (isSpace(**string)) (*string)++;
  if (**string == 0) return 0;
  while (!isSpace((*string)[i])) { if ((*string)[i] == 0) break; else i++; }
  return i;
}

bool stringEqual(int len, char *p, const char *q) {
  int i;
  for (i = 0; i < len; i++) if (p[i] != q[i]) return false;
  return q[i] ? false : true;
}

int getKey(char **string) {
  int i;
  while ((i = get_word(string))) {
    if (stringEqual(i, *string, "Sec-WebSocket-Key:")) {
      *string += i;
      i = get_word(string);
      return i;
    }
    *string += i;
  }
  return 0;
}
