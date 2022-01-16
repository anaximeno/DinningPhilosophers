/* Code by Anaxímeno Brito */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.c"

/* SYNCHRONOUS Concurrent Philosophers Version:
 * In the synchrounous version the philosopher that didn't get the two 
 * hashis will skip eatting and go directly to think, instead of waiting
 * until a hashi is released to eat and then thinking as on async.
 */


void* action(void* __phil) {
    phil_t* phil = (phil_t*) __phil;
    long left, right;

    do {
        #if SHOW_INTENTION
            printf(" %s is trying to eat!\n\n", phil->name);
        #endif

        left = pthread_mutex_trylock(phil->left_hashi);
        right = pthread_mutex_trylock(phil->right_hashi);

        if (!left && !right) {
            eat(rand() % 6 + 3, phil);
        }

        pthread_mutex_unlock(phil->left_hashi);
        pthread_mutex_unlock(phil->right_hashi);

        think(rand() % 7 + 2, phil);
    } while (!halt);

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    pthread_mutex_t hashis[N_DINNING_PHILOSOPHERS];
    phil_t philosophers[N_DINNING_PHILOSOPHERS];
    pthread_t threads[N_DINNING_PHILOSOPHERS + 1];

    time_t t;
    int j, k;

    srand((unsigned) time(&t));
    init_mutexes(hashis, N_DINNING_PHILOSOPHERS);
    init_philosophers(N_DINNING_PHILOSOPHERS, philosophers, hashis);
    printf("\n Type: Synchronous\n");
    printf(" Execution Time: %d seconds\n", SECONDS_TO_WAIT_BEFORE_HALT);
    printf("\n ---------------------Init---------------------\n\n");

    long status;
    for (j = 0 ; j < N_DINNING_PHILOSOPHERS ; ++j) {
        status = pthread_create(&threads[j], NULL, action, (void*)(philosophers + j));
        if (status) {
            perror("pthread_create");
            exit(status);
        } else continue;
    }

    if ((status = pthread_create(&threads[j], NULL, timer, NULL))) {
        perror("pthread_create");
        exit(status);
    }

    for (k = 0 ; k < N_DINNING_PHILOSOPHERS + 1 ; ++k) {
        pthread_join(threads[k], NULL);
    }

    printf("\n ---------------------End---------------------\n\n");
    show_action_status(philosophers, N_DINNING_PHILOSOPHERS);
    destroy_mutexes(hashis, N_DINNING_PHILOSOPHERS);
    return 0;
}