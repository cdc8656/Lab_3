#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    char parent_command[100];
    char child_command[100];
    
    printf("Enter pipefd[0]: ");
    fgets(parent_command, sizeof(parent_command), stdin);
    parent_command[strlen(parent_command) - 1] = '\0';

    printf("Enter pipefd[1]: ");
    fgets(child_command, sizeof(child_command), stdin);
    child_command[strlen(child_command) - 1] = '\0';
    
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) { 
        close(fd[1]); 
        dup2(fd[0], STDIN_FILENO);
        execlp(child_command, child_command, NULL);
        perror("execlp");
    } else {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO); 
        execlp(parent_command, parent_command, NULL);
        perror("execlp");
    }
    return 0;
}

