#include  <stdio.h> /* printf */
#include  <sys/types.h> /* pid_t */
#include <unistd.h> /* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h>  /* wait */
#include <pthread.h>
#include <sys/stat.h>


void * count(void *);
int globalNumber = 0;

pthread_mutex_t lockKey = PTHREAD_MUTEX_INITIALIZER;

int main(void) {

    int i;
    pthread_t counterThread[5];
    int returns[5];


    //Create the 5 threads
    for (i = 0; i < 5; i++) {
        returns[i] = pthread_create(&counterThread[i], (void *) 0,
                count, NULL);
    }
    //Allow the threads to finish
    for (i = 0; i < 5; i++) {
        pthread_join(counterThread[i], NULL);
    }
    
    printf("Value of Global Number at the End: %d \n", globalNumber);
    return (0);
}

void * count(void * junk) {
    int loopCount = 0;

    while (loopCount < 10) {
        pthread_mutex_lock(&lockKey);
        int tmpNumber = globalNumber;
        printf("counter: %d, Thread: %ld, PID: %d\n",
                tmpNumber, pthread_self(), getpid());
        tmpNumber = globalNumber;
        tmpNumber++;
        usleep(random() % 2);
        globalNumber = tmpNumber;
        pthread_mutex_unlock(&lockKey);
        loopCount++;
    }
    return (0);
}
