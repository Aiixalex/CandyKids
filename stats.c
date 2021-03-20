#include "stats.h"
#include <stdio.h>
#include <stdlib.h>

int num_factories = 0;
int factory_made[PRODUCER_MAX];
int factory_eaten[PRODUCER_MAX];
double min_delay[PRODUCER_MAX];
double avg_delay[PRODUCER_MAX];
double max_delay[PRODUCER_MAX];

void stats_init(int num_producers) {
    num_factories = num_producers;
    for (int i=0; i<num_factories; i++) {
        factory_made[i] = 0;
        factory_eaten[i] = 0;
        min_delay[i] = 0;
        avg_delay[i] = 0;
        max_delay[i] = 0;
    }
}

void stats_record_produced(int factory_number) {
    factory_made[factory_number] += 1;
}
void stats_record_consumed(int factory_number, double delay_in_ms) {
    factory_eaten[factory_number] += 1;

    if (min_delay[factory_number] == 0) {
        min_delay[factory_number] = delay_in_ms;
    }

    if (delay_in_ms < min_delay[factory_number]) {
        min_delay[factory_number] = delay_in_ms;
    }
    if (delay_in_ms > max_delay[factory_number]) {
        max_delay[factory_number] = delay_in_ms;
    }
    avg_delay[factory_number] = (avg_delay[factory_number] * (factory_eaten[factory_number] - 1) + delay_in_ms) 
                                    / factory_eaten[factory_number];
}
void stats_display(void) {
    printf("Statistics:\nFactory#   #Made  #Eaten  Min Delay[ms]  Avg Delay[ms]  Max Delay[ms]\n");
    for (int i=0; i<num_factories; i++) {
        printf("%8d%8d%8d%15.5f%15.5f%15.5f\n", i, factory_made[i], factory_eaten[i], min_delay[i], avg_delay[i], max_delay[i]);
    }
    for (int i=0; i<num_factories; i++) {
        if (factory_made[i] != factory_eaten[i]) {
            printf("ERROR: Mismatch between number made and eaten.\n");
        }
    }
}
