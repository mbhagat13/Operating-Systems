#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void parse(char *line, char **argv) {
    while (*line != '\0') {
        while (*line == ' ')
            *line++ = '\0';
        *argv++ = line;
        while (*line != '\0' && *line != ' ')
            line++;
    }
    *argv = '\0';

}


main(int argc, char **argv) {
    int pid;
    int fd[2];
    int i = 0;
    int j = 0;
    int ch;
    char a[100];
    char b[100];
    char *cmd1[100]; //Output goes to pipe input
    char *cmd2[100]; //Input comes from pipe output
    //char *cmd1[] = {"ls", 0}; //Output goes to pipe input
    //char *cmd2[] = {"tr", "a-z", "A-Z", 0}; //Input comes from pipe output
    printf("Your command> ");
    
    while ((ch = getchar()) != '|') {
        a[i] = ch;
        i++;
    }
    a[i - 1] = '\0';
    parse(a, cmd1);


    while ((ch = getchar()) != '\n') {
        b[j] = ch;
        j++;
    }
    b[j] = '\0';
    parse(b, cmd2);
    
    printf("Command 1: %s %s %s \n", cmd1[0], cmd1[1], cmd1[2]);
    printf("Command 2: %s %s %s \n", cmd2[0], cmd2[1], cmd2[2]);
    //execvp(cmd1[0], cmd1);
    pipe(fd); //Create a pipe; fd[0] is input, fd[1] is output


    pid = fork();

    if (pid != 0) {//parent process
        dup2(fd[1], 1);
        close(fd[0]); /* the parent does not need this end of the pipe */
        execvp(cmd1[0], cmd1);
        perror(cmd1[0]); //SHOULD NOT GET HERE (exec failure)
        exit(1);

    } else {
        dup2(fd[0], 0); //pipe input is now stdin for child
        close(fd[1]); //child does not use pipe output
        execvp(cmd2[0], cmd2); //child executes "tr" command
        perror(cmd2[0]); //SHOULD NOT GET HERE!
        exit(1);
    }

    exit(0);
}


