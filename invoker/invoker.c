#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char *directory=argv[1];
	char *menuDirectory = "/mnt/gmenu2x/";
	char *executable=argv[2];
	char *fileToBeExecutedWithFullPath=argv[3];
	char *states=argv[4];
	char *activePage=argv[5];
	chdir(directory);
	char execLocal[20]="";
	pid_t pid;
	pid = fork();
    int fd;
	if (pid == 0) {
        fd = open("/dev/null",O_WRONLY | O_CREAT, 0666);
        dup2(fd, 1);
        dup2(fd, 2);
		if (executable[0]=='#') {
			chdir(menuDirectory);
			execlp(fileToBeExecutedWithFullPath,"invoker",NULL);
		} else {
			snprintf(execLocal,sizeof(execLocal),"./%s",executable);
			execlp(execLocal,"invoker",fileToBeExecutedWithFullPath,NULL);
		}
		close(fd);
	} else {
		wait(0);
	}
	chdir(menuDirectory);
	execlp("./simplemenu.elf","simplemenu.elf", states, activePage, NULL);
}
