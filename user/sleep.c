// Lab Xv6 and Unix utilities
// sleep.c

#include "kernel/types.h"
#include "user/user.h" 

int main(int argc, char *argv[]) {
    // If the number of command-line arguments is not equal to 2, print an error message
    if (argc != 2) { 
        // write(int fd, char *buf, int n) 
        write(2, "Usage: sleep time\n", strlen("Usage: sleep time\n")); 
        exit(-1); 
    }
    // Convert the string argument to an integer
    int time = atoi(argv[1]); 
    if(sleep(time)<0){
        write(2, "Cannot sleep!\n", strlen("Cannot sleep!\n")); 
        exit(-1);
    } 
    exit(0); 
}
