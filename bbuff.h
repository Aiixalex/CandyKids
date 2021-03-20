#ifndef BBUFF_H
#define BBUFF_H


#define BUFFER_SIZE 10


typedef struct {
    int factory_number;
    double creation_ts_ms;
} candy_t;

void bbuff_blocking_insert(void* item);
void* bbuff_blocking_extract(void);
_Bool bbuff_is_empty(void);


#endif