#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	char *directory=argv[1];
	char *menuDirectory = "/mnt/gmenu2x/";
	char *executable=argv[2];
	char *fileToBeExecutedWithFullPath=argv[3];
	char *states=argv[4];
	char *activePage=argv[5];
	char execLocal[20]="";
	pid_t pid;
	pid = fork();
	if (pid == 0) {
		printf("child\n");
		/* We are in the child. */
		if (executable[0]=='#') {
			execlp(fileToBeExecutedWithFullPath,"invoker",NULL);
		} else {
			snprintf(execLocal,sizeof(execLocal),"./%s",executable);
			execlp(execLocal,"invoker",fileToBeExecutedWithFullPath,NULL);
		}
	} else {
		wait(0);
	}
	chdir(menuDirectory);
	execlp("./simplemenu.elf","simplemenu.elf", states, activePage, NULL);
}
