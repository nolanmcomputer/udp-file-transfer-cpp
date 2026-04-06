#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#define BUFFER_SIZE 512

int main(int argc, char *argv[]) {
        int serverSocket, domain;
        struct sockaddr_in serverAddress, clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);

        if(strcmp(argv[1],"-4") == 0){
                domain = AF_INET;
        }
        else if(strcmp(argv[1], "-6") == 0){
                domain = AF_INET6;
        }else{
                std::cerr << "ERROR: Make sure you are doing this correctly" << std::endl;
        }
    // Create the socket
        serverSocket = socket(domain, SOCK_DGRAM, 0);
        if (serverSocket == -1) {
                std::cerr << "Error creating socket" << std::endl;
                 return 1;
         }
        serverAddress.sin_family = domain;
        serverAddress.sin_port = htons(8081);  // Port number to listen on
        serverAddress.sin_addr.s_addr = INADDR_ANY;
    // Bind the socket
        if(domain == AF_INET){//IPV4
                if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1){
                        std::cerr << "Error binding socket" << std::endl;
                        return 1;
                }
        }
        if(domain == AF_INET6){//IPV6 NOT FUNCTIONING IN THIS CODE
                if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1){
                        std::cerr << "Error binding socket" << std::endl;
                        return 1;
                }
        }
    // Open file to write data to
	std::ofstream file("file.txt", std::ios::binary);
	if(!file.is_open()){
		std::cerr << "Error: File not open" << std::endl;
	close(serverSocket);
	return 1;
	}
	
   //The buffer
	char buffer[BUFFER_SIZE];

    // Receive the message (FILE)
     while(true){
	 ssize_t bytesread = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddress, &clientAddressLen);
	 std::cout << "Received:  " << buffer << std::endl;
	   if(bytesread == 0){
		break;
	   }
        file.write(buffer, bytesread);
       }



    // Close the socket
         close(serverSocket);
	 file.close();

	return 0;
}
