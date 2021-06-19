#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    pid_t pid;
    int fd;
	char *cmd[2];
	cmd[0] = (char *)("/bin/cat");
	cmd[1] = 0;

    fd = open("www/phpshit.php", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    if ((pid = fork()) < 0) {
        perror("fork");
        return EXIT_FAILURE;
    } else if (! pid) { /* child */
        dup2(fd, STDIN_FILENO);
        close(fd);
        execve(cmd[0], cmd, 0);
        perror("exec");
        return EXIT_FAILURE;
    } else { /* parent */
        close(fd);
        printf("Parent waiting\n");
    }

    return EXIT_SUCCESS;
}