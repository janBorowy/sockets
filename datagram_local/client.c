#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER "/tmp/studentsocket"
#define CLIENT "/tmp/studentclientsocket"
#define MAXMSG 512
#define MESSAGE "Yow!!! Are we having fun yet?!?"

int makeNamedSocket(const char *filename) {
  struct sockaddr_un name; // adres gniazda
  int sock;                // deskryptor
  size_t size;             // wielkość adresu
  sock = socket(PF_LOCAL, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  name.sun_family = AF_LOCAL;
  strncpy(name.sun_path, filename, sizeof(name.sun_path));
  name.sun_path[sizeof(name.sun_path) - 1] = '\0';
  size = (offsetof(struct sockaddr_un, sun_path) + strlen(name.sun_path));
  if (bind(sock, (struct sockaddr *)&name, size) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  return sock;
}

int main() {

  int sock;
  struct sockaddr_un name;
  char message[MAXMSG];
  size_t size;
  int nbytes;

  sock = makeNamedSocket(CLIENT);
  name.sun_family = AF_LOCAL;
  strcpy(name.sun_path, SERVER);
  size = strlen(name.sun_path) + sizeof(name.sun_family);
  nbytes = sendto(sock, MESSAGE, strlen(MESSAGE) + 1, 0,
                  (struct sockaddr *)&name, size);
  if (nbytes < 0) {
    perror("sendto (client)");
    exit(EXIT_FAILURE);
  }

  nbytes = recvfrom(sock, message, MAXMSG, 0, NULL, 0);

  if (nbytes < 0) {
    perror("recvfrom (client)");
    exit(EXIT_FAILURE);
  }

  printf("Client got message: %s\n", &message);

  remove(CLIENT);
  close(sock);
  return 0;
}