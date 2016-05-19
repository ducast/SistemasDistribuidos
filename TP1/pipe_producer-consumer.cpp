#include <iostream>
#include <unistd.h>
#include <algorithm>
	
using namespace std;

#define READ 0
#define WRITE 1

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

	int fd[2];
	int zero = 0;
	int cap = atoi(argv[1]);

	pipe(fd);	

	if (fork() == 0) {
		//Child process (producer)
		close (fd[READ]);

		int outputArray[cap];

		srand (time(NULL));
		
		for(int i=0;i<cap;i++) {
			outputArray[i] = rand()%100+1;
		}

		sort(outputArray,outputArray+cap);

		for(int i=0;i<cap;i++) {
			cout << "Child: sending random number " << outputArray[i] << " to parent process." << endl;
			write(fd[WRITE],&outputArray[i],sizeof(int));
		}

		write(fd[WRITE],&zero,sizeof(int));

		close(fd[WRITE]);

	} else {
		//Parent process (consumer)
		close(fd[WRITE]);

		int input;

		while(read(fd[READ],&input,sizeof(int))>=0) {
			if (input == 0) {
				cout << "0 received. Exiting program..." << endl;
				break;
			}
			if(checkPrime(input)==true)
				cout << "Parent: Number " << input << " is prime!" << endl;
			else 
				cout << "Parent: Number " << input << " is NOT prime." << endl; 
		} 
		
		close(fd[READ]);
	}
	return 0;
}	




