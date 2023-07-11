// Lab Xv6 and Unix utilities
// primes.c

#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

#define PRIME_NUM 35
#define READEND 0
#define WRITEEND 1
#define ERROREND 2


void child(int* p) {
	int pr[2];
	int n;

	close(p[WRITEEND]);
	// 读取第一个数
	int read_result = read(p[READEND], &n, sizeof(int));
	if (read_result == 0)
		exit(0);
	// 管道右边
	pipe(pr);

	if (fork() == 0) {
		child(pr);
	} else {
		close(pr[READEND]);
		printf("prime %d\n", n);
		int prime = n;
		while (read(p[READEND], &n, sizeof(int)) != 0) {
			if (n%prime != 0) {
				write(pr[WRITEEND], &n, sizeof(int));
			}
		}
		close(pr[WRITEEND]);
		wait((int *) 0);
		exit(0);
	}
}

int main(int argc, char *argv[]) {
	int p[2];
	pipe(p);
	if (fork() == 0) {
        // child process
        // handle with finding primes
		child(p);	
	} else {
		close(p[READEND]);
		// feed the int array
		for (int i=2; i<PRIME_NUM+1; i++) {
			write(p[WRITEEND], &i, sizeof(int));
		}
		close(p[WRITEEND]);
        // wait for child to end
		wait(NULL);
	}
	exit(0);
}
