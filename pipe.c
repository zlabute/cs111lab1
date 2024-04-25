#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	for(int i = 1; i < argc; i++)
	{
		printf("This argument is: ");
		printf("%s\n",argv[i]);
	}
	// TODO: it's all yours
	return 0;
}
