// Lab Xv6 and Unix utilities
// find.c

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

// find 函数
void
find(char *path, char *file)
{   
    //文件描述符、状态结构、目录项结构
	int fd;
	struct stat st;
	struct dirent de;

	char buf[512], *p;

    // 报错：提示无法打开此路径
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    //报错：无法获取状态
    if (fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    //递归读取文件/目录下的文件
    while(read(fd,&de,sizeof(de))==sizeof(de)){
        strcpy(buf,path);
        p=buf+strlen(buf);
        *p++='/';
        //处理目录项为空的情况
        if(de.inum==0)
            continue;
        memmove(p, de.name, DIRSIZ);
        //增加一个字符串结束符，确保字符串的正确
        p[DIRSIZ]=0;

        
		if (stat(buf, &st) < 0) {
			fprintf(2, "ERROR: cannot stat %s\n", buf);
		}


        switch(st.type){
        case T_FILE:
            if (strcmp(file, de.name) == 0){
                printf("%s\n", buf);
            }
            break;

        case T_DIR:
            if ((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0)) {
                find(buf, file);
            }     
        }

    }
    close(fd);
    return;
}

int 
main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2,"ERROR: You need pass in only 2 arguments\n");
        exit(1);
    }
    char* path=argv[1];
    char* file=argv[2];
    find(path,file);
    // 正常退出
    exit(0);
}