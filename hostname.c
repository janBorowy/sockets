#include "arpa/inet.h"
#include "netdb.h"
#include "netinet/in.h"
#include "stdio.h"

int main() {
  char str[50] = "gnu.org\0";
  struct hostent host = *gethostbyname(str);
  printf("Host name: %s\n", host.h_name);
  for (int i = 0; i < host.h_length; ++i)
    printf("Host address: %d\n", host.h_addr_list[i]);
  struct hostent host2 =
      *gethostbyaddr(host.h_addr_list[0], sizeof(host.h_addr_list[0]), AF_INET);
  printf("Host name: %s\n", host2.h_name);
  return 0;
}