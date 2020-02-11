/**
 *
 * COE 628 - Operating Systems
 * Â© Ebrahim Bagheri
 * Lab 6
 *
 **/
#include <stdlib.h>     /* NULL */
#include  <stdio.h> /* printf */
#include  <sys/types.h> /* pid_t */
#include <unistd.h> /* get_pid */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <sys/wait.h>  /* wait */
#include <pthread.h>

#define PRODUCER_NO 5 //Number of producers
#define NUM_PRODUCED 20 //Number of items to be produced

void *generator_function(void);
void *print_function(void);
long sum; /* Sum of generated values*/
long finished_producers; /* number of the producer that finished producing */

//C: Mutex declaration and initialization
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;
//F: Condition variable declaration and initialization
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

int main(void) {
    /* initialize random seed: */
    srand(time(NULL));
    sum = 0;
    int i;
    pthread_t gen_threads[PRODUCER_NO];
    pthread_t print_thread;
    //A: Creates five generator thread
    for (i = 0; i < PRODUCER_NO; i++) {
        pthread_create(&gen_threads[i], NULL, generator_function, NULL);
    }


    //D: Creates print thread
    pthread_create(&print_thread, NULL, print_function, NULL);

    //B: Makes sure that all generator threads has finished before proceeding
    for (i = 0; i < PRODUCER_NO; i++) {
        pthread_join(gen_threads[i], NULL);
    }
    //print_function();
    //E: Makes sure that print thread has finished before proceeding
    pthread_join(print_thread, NULL);

    return (0);

}

void *generator_function(void) {

    long counter = 0;
    long sum_this_generator = 0;

    while (counter < NUM_PRODUCED) {
        pthread_mutex_lock(&mut);
        long tmpNumber = sum;
        long rnd_number = rand() % 10;
        printf("current sum of the generated number up to now is %ld going to add %ld to it.\n", tmpNumber, rnd_number);
        sum = tmpNumber + rnd_number;
        pthread_mutex_unlock(&mut);
        counter++;
        sum_this_generator += rnd_number;
        usleep(1000);
    }
    pthread_mutex_lock(&mut);
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    printf("The sum of produced items for this number generator at the end is: %ld \n", sum_this_generator);
    printf("--+---+----+----------+---------+---+--+---+------+----\n");
    pthread_mutex_unlock(&mut);
    finished_producers++;
    //H: If all generator has finished fire signal for condition variable
    pthread_mutex_lock(&mut2);
    if (finished_producers >= 5) {
        pthread_cond_signal(&cond_var);
    }
    pthread_mutex_unlock(&mut2);
    return (0);
}

void *print_function(void) {
    //G: Wait until all generator has finished
    pthread_mutex_lock(&mut2);
    while (finished_producers < 5) {
        pthread_cond_wait(&cond_var, &mut2);
    }
    pthread_mutex_unlock(&mut2);

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("The value of counter at the end is: %ld \n", sum);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    return (0);
}