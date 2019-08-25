#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char *directory=argv[1];
	char *menuDirectory = "/mnt/gmenu2x/";
	char *executable=argv[2];
	char *fileToBeExecutedWithFullPath=argv[3];
	char *states=argv[4];
	char *activePage=argv[5];
	int ret=0;
	ret = chdir(directory);
	char execLocal[16]="./";
	pid_t pid;
	pid = fork();
    int fd;
	if (pid == 0 ) {
        fd = open("/dev/null",O_WRONLY | O_CREAT, 0666);
        dup2(fd, 1);
        dup2(fd, 2);
		if (executable[0]=='#') {
			ret = chdir(menuDirectory);
			if (ret!=-1) {
				ret = execlp(fileToBeExecutedWithFullPath,"invoker",NULL);
			}
		} else {
			char param[10]="";
			unsigned i;
			for (i=0;i<strlen(executable);i++) {
				if (executable[i]==32) {
					execLocal[i+2]='\0';
					break;
				} else {
					execLocal[i+2]=executable[i];
				}
			}
			int j=0;
			i++;
			for (;i<strlen(executable);i++) {
				if (executable[i]=='\0') {
					break;
				}
				param[j]=executable[i];
				j++;
			}
			if (param[0]=='\0') {
				snprintf(execLocal,sizeof(execLocal),"./%s",executable);
				ret=execlp(execLocal,"invoker",fileToBeExecutedWithFullPath,NULL);
			} else {
				ret=execlp(execLocal,"invoker",param,fileToBeExecutedWithFullPath,NULL);
			}
		}
		close(fd);
	} else {
		wait(0);
	}
	ret = chdir(menuDirectory);
	if  (ret!=-1) {
		execlp("./simplemenu.elf","simplemenu.elf", states, activePage, NULL);
	} else {
		printf("ERROR!!!");
		return (-1);
	}
}
