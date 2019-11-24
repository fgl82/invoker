#include <asm-generic/ioctls.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char menuDirectory[100] = "";
	char *directory=argv[1];
	char *executable=argv[2];
	char *fileToBeExecutedWithFullPath=argv[3];
	char *states=argv[4];
	char *activePage=argv[5];
	char *returnTo=argv[6];
	char *pictureMode=argv[7];
	int ret=0;
	int fd;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
	getcwd(menuDirectory, sizeof(menuDirectory));
	ret = chdir(directory);
	pid_t pid;
	pid = fork();
	if (pid == 0 ) {
		fd = open("/dev/null",O_WRONLY | O_CREAT, 0666);
		dup2(fd, 1);
		dup2(fd, 2);
		//it's a section meant for native apps
		if (executable[0]=='#') {
			ret = chdir(menuDirectory);
			if (ret!=-1) {
				//Native opk with a desktop file as parameter
				char *params[10];
				if (strstr(fileToBeExecutedWithFullPath,"|")!=NULL) {
					params[0]="opkrun";
					char *ptr = strtok(fileToBeExecutedWithFullPath, "|");
					int i=1;
					while(ptr != NULL) {
						params[i]=malloc(strlen(ptr));
						strcpy(params[i],ptr);
						ptr = strtok(NULL, "|");
						i++;
					}
					params[i]=NULL;
					ret = execvp("opkrun",params);
				} else {
					if(strstr(fileToBeExecutedWithFullPath,".opk")) {
						//it's an opk with just a default desktop file
						ret = execlp("opkrun","invoker",fileToBeExecutedWithFullPath,NULL);
					} else {
						//it's a generic executable file
						ret = execlp(fileToBeExecutedWithFullPath,"invoker",NULL);
					}
				}
			}
		} else {
			//Emulation: it's an opk with a rom as a parameter
			ret=execlp("opkrun","invoker",executable,fileToBeExecutedWithFullPath,NULL);
		}
		close(fd);
	} else {
		wait(0);
	}
	ret = chdir(menuDirectory);
	if  (ret!=-1) {
		execlp("./simplemenu.dge","simplemenu.dge", states, activePage, returnTo, pictureMode, NULL);
	} else {
		return (-1);
	}
}
