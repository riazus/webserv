#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
	pid_t pid;
	int i = 0;
	int fd;
	while(1)
	{	
		fd = open("time.txt", O_RDWR);
		dup2(fd, 1);
		pid = fork();
		if (pid == 0)
		{
			execlp("python", "python", "current_time.py", (char*) NULL);
		}
		waitpid(pid, 0, 0);
		sleep(1);
		i++;
		close(fd);
	}
}