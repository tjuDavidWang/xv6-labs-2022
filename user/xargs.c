// Lab Xv6 and Unix utilities
// xargs.c

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAX_LEN 100

int main(int argc, char *argv[]) {
	char *cmd = argv[1];
	char buf;
	char paramv[MAXARG][MAX_LEN]; 
	char *new_argv[MAXARG];

	while (1) {
		int count = argc-1;  
		memset(paramv, 0, MAXARG * MAX_LEN);
        // 将xargs后面的参数复制到paramv中
		for (int i=1; i<argc; i++) {
			strcpy(paramv[i-1], argv[i]);
		}

		int ptr = 0; //
		int flag = 0; 
		int read_result;
        //读取参数直至末尾，一次读取一个字符
		while (((read_result = read(0, &buf, 1))) > 0 && buf != '\n') {
            //遇到空格，参数个数+1
			if (buf == ' ' && flag == 1) {
				count++;
				ptr = 0;
				flag = 0;
			}
			else if (buf != ' ') {
				paramv[count][ptr++] = buf;
				flag = 1;
			}
		}
		// 处理EOF和\n的情况
		if (read_result <= 0) {
			break;
		}
		for (int i=0; i<MAXARG-1; i++) {
			new_argv[i] = paramv[i];
		}
        //添加NULL，表示数组结束
		new_argv[MAXARG-1] = '\0';

        //建立子进程处理指令
		if (fork() == 0) {
			exec(cmd, new_argv);
			exit(0);
		} else {
			wait((int *) 0);
		}
	}
	exit(0);
}