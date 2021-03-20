#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include "bbuff.h"
#include "stats.h"


_Bool stop_thread = false;

_Bool flag[PRODUCER_MAX + CONSUMER_MAX] = {false};
int turn = 0;


double current_time_in_ms(void) {
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

void* factory_thread_function(void* param) {
    int factory_index = *(int *)param;
    while (!stop_thread) {
        // Do the work of the thread
        srand(time(NULL));
        int wait_sec = rand() % 3 + 1; // randomly selected between 0 and 3 (inclusive)
        printf("Factory %d ships candy & waits %ds\n", factory_index, wait_sec);

        candy_t *candy = &(candy_t) {.factory_number = factory_index, .creation_ts_ms = current_time_in_ms()};

        flag[factory_index] = true;
        while(turn != factory_index) {
            while (flag[turn] == false) {
                turn = (turn + 1) % (PRODUCER_MAX + CONSUMER_MAX);
            }
        }

        bbuff_blocking_insert(candy); // critical section
        stats_record_produced(factory_index);

        flag[factory_index] = false;

        sleep(wait_sec);
    }
    printf("Candy-factory %d done.\n", factory_index);
    pthread_exit(0);
}

void* kid_thread_function(void* param) {
    int kid_index = *(int *)param;
    // loop forever
    while (true) {
        flag[kid_index + PRODUCER_MAX] = true;
        while(turn != kid_index + PRODUCER_MAX) {
            while (flag[turn] == false) {
                turn = (turn + 1) % (PRODUCER_MAX + CONSUMER_MAX);
            }
        }

        void* extracted_candy = bbuff_blocking_extract();
        if(extracted_candy != NULL) {
            stats_record_consumed(((candy_t *)extracted_candy)->factory_number, current_time_in_ms() - 
                                    ((candy_t *)extracted_candy)->creation_ts_ms);
        }
        
        flag[kid_index + PRODUCER_MAX] = false;

        srand(time(NULL));
        int wait_sec = rand() % 2;
        sleep(wait_sec); // sleep for either 0 or 1 seconds (randomly selected)
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    // 1. Extract arguments
    int num_of_factories = 0; // Number of candy-factory threads to spawn
    int num_of_kids = 0;      // Number of kids threads to spawn
    int num_of_seconds = 0;   // Number of seconds to allow the factory threads to run for

    if (argc != 4) {
        perror("The number of args is invalid.\n");
    } else {
        num_of_factories = atoi(argv[1]);
        num_of_kids = atoi(argv[2]);
        num_of_seconds = atoi(argv[3]);
    }

    // All arguments should be greater than 0.
    if (num_of_factories <= 0 || num_of_kids <= 0 || num_of_seconds <= 0) {
        perror("All arguments should be greater than 0.");
    }

    // 2. Initialize modules
    stats_init(num_of_factories);

    // 3. Launch candy-factory threads
    pthread_t factory_tids[num_of_factories];
    int factory_thread_args[num_of_factories];
    for (int i=0; i<num_of_factories; i++) {
        factory_thread_args[i] = i;
        pthread_create(&factory_tids[i], NULL, factory_thread_function, &factory_thread_args[i]);
    }

    // 4. Launch kid threads
    pthread_t kid_tids[num_of_kids];
    int kid_thread_args[num_of_kids];
    for (int i=0; i<num_of_kids; i++) {
        kid_thread_args[i] = i;
        pthread_create(&kid_tids[i], NULL, kid_thread_function, &kid_thread_args[i]);
    }

    // 5. Wait for requested time
    sleep(num_of_seconds);

    // 6. Stop candy-factory threads
    stop_thread = true;
    for (int i=0; i<num_of_factories; i++) {
        pthread_join(factory_tids[i], NULL);
    }
    
    // 7. Wait until no more candy
    while (!bbuff_is_empty()) {
        printf("Waiting for all candy to be consumed.\n");
        sleep(1);
    }

    // 8. Stop kid threads
    for (int i=0; i<num_of_kids; i++) {
        pthread_cancel(kid_tids[i]);
        pthread_join(kid_tids[i], NULL);
    }

    // 9. Print statistics
    stats_display();
}