#include <asm-generic/ioctls.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

void writeCenteredMessage(char *string, int rows, int cols) {
	int vertl = rows/2;
	int stringLength = strlen(string) / 2;
	for (int x = 0; x <= rows; x++) {
		printf("\n");
		if (x == vertl) {
			printf("\n%*s\n", cols / 2 + stringLength, string);
		}
	}
}

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
	char *activePage=argv[5];
	char *returnTo=argv[6];
	char *pictureMode=argv[7];
	int ret=0;
	#ifdef TARGET_BITTBOY
	int fd;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int columns = w.ws_col+30;
    int rows = w.ws_row-4;
	#endif
	getcwd(menuDirectory, sizeof(menuDirectory));
	ret = chdir(directory);
	pid_t pid;
	pid = fork();
	if (pid == 0 ) {
		#ifdef TARGET_BITTBOY
		fd = open("/dev/null",O_WRONLY | O_CREAT, 0666);
		dup2(fd, 1);
		dup2(fd, 2);
		#endif
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
//					printf("WTF3 %s!\n", fileToBeExecutedWithFullPath);
//					printf("WTF3 %s!\n", params[0]);
//					printf("WTF3 %s!\n", params[1]);
					if (strstr(params[2],"default.")!=NULL) {
						params[2][strlen(params[2])-1]='\0';
					}
//					printf("WTF3 %s!\n", params[2]);
//					printf("WTF3 %s!\n", params[3]);
//					sleep(2);
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
						//it's an executable
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
				//it's an opk emulator
				if (strcmp(fileToBeExecutedWithFullPath,"*")==0) {
					ret=execlp("opkrun","invoker",executable,NULL);
				} else {
					ret=execlp("opkrun","invoker",executable,fileToBeExecutedWithFullPath,NULL);
				}
			} else {
				//it's an non-opk emulator
				if (strstr(executable," ")==NULL) {
					char localExec[100];
					strcpy(localExec,"./");
					strcat(localExec,executable);
					if (strcmp(fileToBeExecutedWithFullPath,"*")==0) {
						ret=execlp(localExec,"invoker",NULL);
					} else {
						ret=execlp(localExec,"invoker",fileToBeExecutedWithFullPath,NULL);
					}
				} else {
					char localExec[100];
					char *params[10];
					char *ptr = strtok(executable, " ");
					strcpy(localExec,"./");
					strcat(localExec,ptr);
					params[0]=localExec;
					int i=1;
					ptr = strtok(NULL, " ");
					while(ptr != NULL) {
						params[i]=malloc(strlen(ptr)+1);
						strcpy(params[i],ptr);
						ptr = strtok(NULL, " ");
						i++;
					}
					params[i]=malloc(strlen(fileToBeExecutedWithFullPath)+1);
					strcpy(params[i],fileToBeExecutedWithFullPath);
					i++;
					params[i]=NULL;
//					printf("WTF3 %s!\n", params[2]);
//					printf("WTF3 %s!\n", params[3]);
//					sleep(2);
					ret = execvp(localExec,params);
				}
			}
		}
		#ifdef TARGET_BITTBOY
		close(fd);
		#endif
	} else {
		#ifdef TARGET_BITTBOY
	    writeCenteredMessage("\033[1;31mW\033[01;33mA\033[1;32mI\033[1;36mT\n", rows, columns);
		#endif
		wait(0);
	}
	ret = chdir(menuDirectory);
	#ifdef TARGET_BITTBOY
	writeCenteredMessage(" ", rows, columns);
	#endif
	if  (ret!=-1) {
		execlp("./simplemenu","simplemenu", states, activePage, returnTo, pictureMode, NULL);
	} else {
		return (-1);
	}
}
