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
			exit(EINVAL);
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
			exit(EXIT_FAILURE);
		}
	}



	return 0;








//  // working code
// 	int prev_pipe[2];
// 	int next_pipe[2];
// 	int pids[argc-1];

// 	for(int i = 1; i < argc; i++) { // loop through arguments


// 		if (i < argc - 1 && pipe(next_pipe) == -1) {
// 			perror("pipe");
// 			exit(errno);
// 		}

// 		pids[i-1] = fork();

// 		if(pids[i-1] == -1)  // fork failiure
// 		{
// 			perror("fork");
// 			exit(EXIT_FAILURE);
// 		}
// 		else if (pids[i-1] == 0) // child processes
// 		{
// 			if (i > 1) // skips over argv[0] as this is ./pipe
// 			{
// 				close(prev_pipe[1]);
// 				dup2(prev_pipe[0], STDIN_FILENO);  // redirect input to read the end of pipe
// 				close(prev_pipe[0]);  				// close read end of pipe after putting into standard input
// 			}

// 			if (i < argc - 1)  // if not last argument
// 			{
// 				close(next_pipe[0]);
// 				dup2(next_pipe[1], STDOUT_FILENO); // redirect output to write end of pipe
// 				close(next_pipe[1]); // close the write end of pipe after writing STDOUT into write end
// 			}

// 			// if (access(argv[i], X_OK) == -1)
// 			// {
// 			// 	perror("access");
// 			// 	exit(ENOENT);
// 			// }

// 			execlp(argv[i], argv[i], NULL); // executes the executable
// 			//perror("execlp"); // writes any error that occurs to errno
// 			exit(EXIT_FAILURE); // exits with errno if error occured continues if not
// 		}
// 		else // parent process
// 		{
// 			if ( i > 1)
// 			{
// 				close(prev_pipe[0]);
// 				close(prev_pipe[1]);
// 			}

// 			if (i == argc -1 )
// 			{
// 				close(next_pipe[0]);
// 				close(next_pipe[1]);
// 			}


// 			if ( i < argc - 1)
// 			{
// 				prev_pipe[0] = next_pipe[0];
// 				prev_pipe[1] = next_pipe[1];
// 			}
// 		}
// 	}

	
// 	for (int i =1; i < argc; i++) { //wait for child processes to finish
// 		int status;
// 		if (waitpid(pids[i-1],&status,0) == -1) {
// 			exit(EINVAL);
// 		}
// 		if (WIFEXITED(status)){
// 			WEXITSTATUS(status);
// 		}
// 		else
// 		{
// 			exit(EXIT_FAILURE);
// 		}
// 	}

// 	return 0;

	// int fd[2];

	// if (pipe(fd) == -1)
	// {
	// 	return 1;
	// }

	// pid_t pid = fork();
	// if (pid < 0){
	// 	return 2;
	// }

	// if (pid == 0) // child process 
	// {
	// 	dup2(fd[1], STDOUT_FILENO);
	// 	close(fd[0]);
	// 	close(fd[1]);
	// 	execlp("ping", "ping","-c","5","google.com", NULL);
	// }







	// if (argc < 2){
	// 	fprintf()
	// }

	// int pipefd[2]; //file descriptors of the pipe

	// //attempt creation of the pipe
	// if (pipe(pipefd) == -1){
	// 	perror("pipe");
	// 	return 1;
	// }

	// pid_t pid = fork();

	// if (pid == -1){
	// 	perror("fork");
	// 	return 1;
	// }
	// else if (pid == 0) { //child process 
		
	// 	close(pipefd[0]);// close the write end of the pipe

	// 	dup2(pipefd[1], STDOUT_FILENO); //redirect standard output to write to the pipe

	// 	if (execvp(arg[1], &argv[1]) == -1) {
	// 		perror("execvp");
	// 		return 1;
	// 	}

	// }
	// else{
	// 	close(pipefd[1]);

	// 	dup2(pipefd[0], STDIN_FILENO);

	// 	// Execute the secojnd command using execvp
	// 	if (execvp(argv[argc -1], &argv[argc -1]) == -1) {
	// 		perror
	// 	}

	// }




	



















	// int return_code = fork();
	// if(return_code == 0){
	// 	printf("This is the child process!\n");
	// 	execlp("ls","ls","-a","-l",NULL);
	// 	exit(0);

	// }
	// else if(return_code > 0){
	// 	printf("I am lazy parent, letting my child to ls the directory\n");
	// 	printf("I will just wait for their report\n");
	// 	int pid = return_code;
	// 	int status = 0;
	// 	waitpid(pid, &status, 0);
	// 	printf("Child process exots with code: %d\n", WEXITSTATUS(status));
	// }
	// else{
	// 	printf("Child process creatiion error! \n");
	// }
	// printf("They finished; Done!\n");
	// return 0;


	// for(int i = 1; i < argc; i++)
	// {
	// 	printf("This argument is: ");
	// 	printf("%s\n",argv[i]);
	// }
	// // TODO: it's all yours
	// return 0;
}
