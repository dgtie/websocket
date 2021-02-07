#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SA struct sockaddr

#define MAX 1024
#define PORT 3333

using namespace std;

int get_word(char **string);
bool stringEqual(int len, char *p, const char *q);
int getKey(char **string);
bool encrypt(char*, char*);
char *unmask(char*);

char response[] =
"HTTP/1.1 101 Switching Protocols\r\n"
"Upgrade: websocket\r\n"
"Connection: Upgrade\r\n"
"Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n" "\r\n";
//"Sec-WebSocket-Protocol: chat\r\n\r\n";
//97 = s3pPL...

void func(int sockfd) {
  char buff[MAX];

    bzero(buff, MAX);
    read(sockfd, buff, sizeof(buff));
    char *string = buff;
    int i = getKey(&string);
    if (i) {
      printf("%s\n", string);
      if (encrypt(string, &response[97])) {
        printf("%s\n%d\n", response, (int)sizeof(response));
        write(sockfd, response, sizeof(response)-1); 
        while(1) {
          bzero(buff, MAX);
          i = read(sockfd, buff, sizeof(buff));
          printf("%d\n", i);
//          for (int j = 0; j < i; j++) printf("%02X\n", buff[j]);
          char *s = unmask(buff);
          if (s) cout << s << endl;
          i = get_word(&s);
          if (stringEqual(i, s, "echo")) {
            s += 2;
            s[0] = (char)0x81;
            i = 2;
            while (s[i]) i++;
            s[1] = i - 2;
            write(sockfd, s, i);
          }
          if (!(buff[1] & 127)) break;
        }
      }
    }
}

int main() {
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    cout << "socket creation failed...\n";
    exit(0);
  } else cout << "socket successfully created..\n";
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);
  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
    cout << "socket bind failed...\n";
    exit(0);
  } else cout << "Socket successfully binded..\n";
  if ((listen(sockfd, 5)) != 0) {
    cout << "Listen failed...\n";
    exit(0);
  } else cout << "Server listening..\n";
  len = sizeof(cli);
  connfd = accept(sockfd, (SA*)&cli, (socklen_t*)&len);
  if (connfd < 0) {
    cout << "server accept failed..\n";
    exit(0);
  } else cout << "server accept the client...\n";
  func(connfd);
  close(sockfd);
}
