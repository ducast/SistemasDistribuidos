#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <string.h>

using namespace std;


int main (int argc, char* argv[]) {
	int pid = atoi(argv[1]);
	int signal = atoi(argv[2]);

	cout << signal << endl;

	int error = kill(pid, signal);

	if(error !=0)
		cout << "Error sending signal to catcher program."<< endl;
		cout << "errno: " << errno << " " << strerror(errno) << endl;
	return 0;
}
