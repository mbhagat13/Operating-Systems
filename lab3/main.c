/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: mbhagat
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * 
 */
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

int main(int argc, char** argv) {
    pid_t pid;
    int ch;
    int check = 1;
    int i = 0;
    int x = 0;
    char a[100];
    char* start[100];

    start[0] = &a[0];


    while (x == 0) {
        i = 0;
        wait(&pid);
        printf("Your command> ");
        while ((ch = getchar()) != '\n') {
            a[i] = ch;
            i++;
        }
        a[i] = '\0';
        if (a[i - 1] == '&') {
            printf("PARENT EXECUTING FIRST.\n");
            a[i - 1] = '\0';
            check = 0;
         }

        parse(a, start);
        pid=fork();
        if (pid != 0) {//parent process
            if (check != 0) {//only waits if user entered &
                wait(&pid);
            }

        } else {//child process
            execvp(start[0], start);
            fprintf(stderr, "ERROR\n");
            exit(1);

        }
        if (check == 0) {
            check = 1;
            printf("PARENT EXECUTED.\n");
        }
    }
    return (EXIT_SUCCESS);
}