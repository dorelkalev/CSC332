#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

pid_t pid1;
pid_t pid2;
int pipefd[2];

// Create the pipe
    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Pipe creation failed\n");
        return 1;
    }
  
// Fork the first child
    pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "Fork for first child failed\n");
        return 1;
    } 
    
    else if (pid1 == 0) {
        // First child process: ls -F
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        close(pipefd[0]); // Close unused read end of the pipe
        close(pipefd[1]); // Close original write end after dup2

        execlp("ls", "ls", "-F", NULL);
        fprintf(stderr, "execlp for ls failed\n");
        exit(1);
    }

    // Fork the second child
    pid2 = fork();

    if (pid2 < 0) {
        fprintf(stderr, "Fork for second child failed\n");
        return 1;
    } 
    
    else if (pid2 == 0) {
    // Second child process: nl
    dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
    close(pipefd[1]); // Close unused write end of the pipe
    close(pipefd[0]); // Close original read end after dup2

    execlp("nl", "nl", NULL);
    fprintf(stderr, "execlp for nl failed\n");
    exit(1);
  }

// Parent process
close(pipefd[0]); // Close read end of the pipe
close(pipefd[1]); // Close write end of the pipe (both ends are now closed)

// Wait for both children
wait(NULL); // Wait for first child
wait(NULL); // Wait for second child

printf("Both children completed.\n");
return 0;
}
