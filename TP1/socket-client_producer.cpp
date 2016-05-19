#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <algorithm>

using namespace std;

int main (int argc, char** argv) {

	int cap = atoi(argv[1]);
	int outputArray[cap];
	int socketfd, intBuffer;
	int zero = 0;
	const char* portno = argv[3];
	struct addrinfo hints, *res;

	bzero((char*) &hints, sizeof(hints));

	hints.ai_family = AF_INET;     
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_flags = AI_PASSIVE;   

	getaddrinfo(argv[2],portno,&hints,&res);

	socketfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

	connect(socketfd,res->ai_addr,res->ai_addrlen);

	srand (time(NULL));

  	for(int i=0;i<cap;i++) {
  		outputArray[i] = rand()%100+1;
  	}

  	sort(outputArray,outputArray+cap);

  	for(int i=0;i<cap;i++) {
  		cout << "Producer: sending random number " << outputArray[i] << " to consumer." << endl;
  		send(socketfd,&outputArray[i],sizeof(int),0);
  		recv(socketfd,&intBuffer,sizeof(int),0);
  		cout << "Consumer: checkPrime result = " << intBuffer << endl;
  	}

  	send(socketfd,&zero,sizeof(int),0);

  	cout << "Sending 0 to consumer and exiting..." << endl;

	close(socketfd);

	return 0;
}	
