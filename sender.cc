
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#define BUFFER_SIZE 512

//Converts argv[2] (the port number) to an unsigned integer
int string_to_unint16(const char *str, uint16_t res) {
        char *end;
        errno = 0;
        long val = strtol(str, &end, 10);
        if(errno || end == str || *end != '\0' || val < 0 || val >= 0x10000){
                return false;
        }
        res = (uint16_t)val;
        return res;
}

// provide hostname and port value as command line arguments
// Mess up with these values and the socket call will likely fail
// argv[0] is the executable name
int main(int argc, char *argv[]) {
  int sock, rval, realres;
  const char* dataToSend;
  struct addrinfo hints, *results, *ptr;
  uint16_t res;

  // quick check if we provide the right arguments
  if (argc != 3) {
    std::cout << "Usage " << argv[0] << " destination_host destination_port" << std::endl;
    return 1;
  }
  // convert argv[2] to an unsigned integer here
  realres = string_to_unint16(argv[2], res);  

  // if we obtain the data from getaddrinfo, we use it to open the socket

  // first, we prepare hints by clearing all of its fields

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;  // we ask for both IPv4 and IPv6
  hints.ai_socktype = SOCK_DGRAM;
  
  if ((rval = getaddrinfo(argv[1], argv[2], &hints, &results)) != 0) { // error
    std::cerr << "Error getting the destination address: " << gai_strerror(rval) << std::endl;
    return 2;
  }
  // loop through the results from the linked list
  for (ptr = results; ptr != NULL; ptr = ptr->ai_next) {  
    if ((sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) != -1) {
      break;  // socket successful, no need to search further
    }
  }

  // but we might reach the end of the list...
  if (ptr == NULL) {
    std::cerr << "Failed to open socket" << std::endl;
    return 3;
  }

  //Send
  std::cout << "$ sender " << argv[1] << " " << realres << std::endl;
  //Open file and send file

  char buffer[BUFFER_SIZE];

  std::ifstream file("file.txt", std::ios::binary);
  if (!file.is_open()){
	std::cerr << "Error: File not open" << std::endl;
	close(sock);
	return 1;
  }
  
  struct sockaddr_in server;
  server.sin_family = AF_UNSPEC;
  server.sin_port = htons(realres);
  inet_pton(AF_UNSPEC, argv[1], &server.sin_addr);
  while (!file.eof()){
	file.read(buffer, BUFFER_SIZE);
	sendto(sock, buffer, file.gcount(), 0, (struct sockaddr*)&server, sizeof(server));
  }	

  close(sock);
}
