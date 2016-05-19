#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <string.h>

using namespace std;


void signal_handler (int signo) {
	if (signo == SIGUSR1) 
		cout << "SIGUSR1 received." << endl;
	if (signo == SIGUSR2) 
		cout << "SIGUSR2 received." << endl;
	if (signo == SIGINT) 
		cout << "SIGINT received." << endl;
	if (signo == SIGKILL) 
		cout << "SIGKILL received" << endl;
}

int main (int argc, char* argv[]) {

	bool waitType = atoi(argv[1]); //0: busy wait | 1: blocking wait 

	if (signal(SIGUSR1,signal_handler) == SIG_ERR)
		cout << "Error catching SIGUSR1." << endl;	
	if (signal(SIGUSR2,signal_handler) == SIG_ERR)
		cout << "Error catching SIGUSR2." << endl;	
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		cout << "Error catching SIGINT." << endl;
	//if (signal (SIGKILL, signal_handler) == SIG_ERR)
	//	cout << "SIGKILL cannot be caught. Sorry ):" << endl;

	if (waitType == false) {
		cout << "Entering signal-catching loop (busy wait)." << endl;
		while (true)
			sleep(1);
	} else {
		while(true) {
			cout << "Pausing until signal is received (blocking wait)." << endl;		
			pause();
		}	
		cout << "errno: " << errno << " " << strerror(errno) << endl;
	}
	return 0;
}
