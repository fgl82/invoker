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
    int columns = w.ws_col+30;
    int rows = w.ws_row-4;
	getcwd(menuDirectory, sizeof(menuDirectory));
	ret = chdir(directory);
	pid_t pid;
	pid = fork();
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
			ret=execlp("opkrun","invoker",executable,fileToBeExecutedWithFullPath,NULL);
		}
		close(fd);
	} else {
	    writeCenteredMessage("\033[1;31mW\033[01;33mA\033[1;32mI\033[1;36mT\n", rows, columns);
		wait(0);
	}
	ret = chdir(menuDirectory);
	writeCenteredMessage(" ", rows, columns);
	if  (ret!=-1) {
		execlp("./simplemenu.dge","simplemenu.dge", states, activePage, returnTo, pictureMode, NULL);
	} else {
		printf("ERROR!!!\n");
		return (-1);
	}
}
