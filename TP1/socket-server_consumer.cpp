#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
	
using namespace std;

int checkPrime(int num){
    if(num==0){
        return 0;
    }
    else if(num>=2){
        int i=2;
        while(i<num){
            if(num%i == 0){ //num is not a prime number
                return 0;
            }
            i++;
        }
    }
    return 1;
}

int main (int argc, char** argv) {

	int socketfd, newsocketfd, result; 
	int buffer = 1;
	int zero = 0;
	const char* portno = argv[1];
	struct addrinfo hints, *res;
	struct sockaddr_in client_address;

	bzero((char*) &hints, sizeof(hints));

	hints.ai_family = AF_INET;     
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_flags = AI_PASSIVE;   

	getaddrinfo(NULL,portno,&hints,&res);

	socketfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

	bind(socketfd, res->ai_addr, res->ai_addrlen);

	listen(socketfd,5);

	socklen_t client_lenght = sizeof(client_address);
	newsocketfd = accept(socketfd,(struct sockaddr*)&client_address,&client_lenght);

	while(buffer>0) {
	    	recv(newsocketfd,&buffer,sizeof(int),0);
	    	result = checkPrime(buffer);
	    	send(newsocketfd,&result,sizeof(int),0);
	}

	cout  << "Consumer: Received 0 from producer. Exiting..." << endl;

	close(socketfd);

	return 0;
}	
