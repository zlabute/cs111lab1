#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


int main(int argc, char *argv[])
{
	if (argc < 2) //makes sure there is an argument besides ./pipe otherwise invalid input
	{
		exit(EINVAL);
	}

	pid_t pids[argc - 1];
	int curr_pipe[2];
	int prev_pipe[2];

	for (int i = 0; i < argc-1; i++)
	{
		if (i < argc - 2 && pipe(curr_pipe) == -1)
		{
			perror("pipe");
			exit(errno);
		}

		pids[i] = fork();
		if(pids[i] < 0)
		{
			exit(EXIT_FAILURE);
		}
		else if(pids[i] == 0) // child process
		{
			if(i < argc - 2) // redirect output to curr_pipe besides last
			{
				close(curr_pipe[0]);
				dup2(curr_pipe[1], 1);
				close(curr_pipe[1]);
			}

			if(i > 0) //redirect input to output of prev_pipe
			{
				close(prev_pipe[1]);
				dup2(prev_pipe[0], 0);
				close(prev_pipe[0]);
			}

			execlp(argv[i+1], argv[i+1], NULL);
			exit(errno);
		}
		else
		{
			if( i > 0) //if not first proccess close prev_pipe
			{
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			if(i != argc - 2)
			{
				prev_pipe[0] = curr_pipe[0];
				prev_pipe[1] = curr_pipe[1];
			}
		}
	}

	for(int i = 0; i < argc - 1; i++)
	{
		int status;
		if(waitpid(pids[i], &status, 0) == -1)
		{
			exit(EXIT_FAILURE);
		}
		if(WIFEXITED(status))
		{
			WEXITSTATUS(status);
			if(WEXITSTATUS(status) != 0)
			{
				exit(WEXITSTATUS(status));
			}
		}
		else{
			exit(EINVAL);
		}
	}
	return 0;
}
