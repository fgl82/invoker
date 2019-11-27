#include <asm-generic/ioctls.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

char *getFilePath(char *fileName) {
	char *retstr;
	char *lastslash;
	retstr = malloc(strlen (fileName) + 1);
	strcpy (retstr, fileName);
	lastslash = strrchr (retstr, '/');
//	sleep(2);
	if (lastslash != NULL) {
		*lastslash = '\0';
	}
	return retstr;
}

int main(int argc, char *argv[]) {
	char menuDirectory[100] = "";
	char *directory=argv[1];
	char *executable=argv[2];
	char *fileToBeExecutedWithFullPath=argv[3];
	char *states=argv[4];
	printf("states %s\n", states);
	char *activePage=argv[5];
	char *returnTo=argv[6];
	char *pictureMode=argv[7];
	int ret=0;
//	int fd;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
	getcwd(menuDirectory, sizeof(menuDirectory));
	ret = chdir(directory);
	pid_t pid;
	pid = fork();
	if (pid == 0 ) {
//		fd = open("/dev/null",O_WRONLY | O_CREAT, 0666);
//		dup2(fd, 1);
//		dup2(fd, 2);
		//it's a section meant for native apps
		if (executable[0]=='#') {
//			printf("WTF!\n");
//			sleep(1);
			ret = chdir(menuDirectory);
			if (ret!=-1) {
//				printf("WTF2!\n");
//				sleep(1);
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
//					printf("WTF3 %s!\n", fileToBeExecutedWithFullPath);
//					sleep(1);
					if(strstr(fileToBeExecutedWithFullPath,".opk")) {
						//it's an opk with just a default desktop file
//						printf("SIMPLE OPK! %s\n",fileToBeExecutedWithFullPath);
//						sleep(2);
						ret = execlp("opkrun","invoker",fileToBeExecutedWithFullPath,NULL);
					} else {
						char* dirToSwitch = getFilePath(fileToBeExecutedWithFullPath);
//						printf("%s\n",dirToSwitch);
//						sleep(2);
						chdir(dirToSwitch);
						ret = execlp(fileToBeExecutedWithFullPath,"invoker",NULL);
					}
				}
			}
		} else {
			//it's an emulator
			if (strstr(executable,".opk")) {
				//an opk emulator
				if (strcmp(fileToBeExecutedWithFullPath,"*")==0) {
					ret=execlp("opkrun","invoker",executable,NULL);
				} else {
					ret=execlp("opkrun","invoker",executable,fileToBeExecutedWithFullPath,NULL);
				}
			} else {
				//it's an non-opk emulator
				char localExec[100];
				strcpy(localExec,"./");
				strcat(localExec,executable);
				if (strcmp(fileToBeExecutedWithFullPath,"*")==0) {
					ret=execlp(localExec,"invoker",NULL);
				} else {
					ret=execlp(localExec,"invoker",fileToBeExecutedWithFullPath,NULL);
				}
			}
		}
//		close(fd);
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
