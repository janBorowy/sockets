#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER "/tmp/studentsocket"

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
  char message[512];
  struct sockaddr_un name;
  size_t size;
  int nbytes;

  unlink(SERVER);

  sock = makeNamedSocket(SERVER);
  while (1) {
    size = sizeof(name);
    nbytes = recvfrom(sock, message, 512, 0, (struct sockaddr *)&name, &size);
    if (nbytes < 0) {
      perror("recvfrom (server)");
      exit(EXIT_FAILURE);
    }

    printf("Server got message %s\n", &message);
    nbytes = sendto(sock, message, 512, 0, (struct sockaddr *)&name, size);
    if (nbytes < 0) {
      perror("sendto (server)");
      exit(EXIT_FAILURE);
    }
  }
}