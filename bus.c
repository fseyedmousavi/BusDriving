// Fill in empty functions and defintions

#include "pintos_headers.h"

struct station {
    struct condition cond;
    struct condition cond_pass;
    struct lock lock;
    int passenger_count;
};

void station_setup(struct station *station) {
    cond_init(&station->cond);
    lock_init(&station->lock);
    cond_init(&station->cond_pass);

    station->passenger_count = 0;
}

void bus_load_passengers(struct station *station, int count) {
    lock_acquire(&station->lock);
    if (count == 0) {
        lock_release(&station->lock);
        return;
    }
    if (station->passenger_count == 0) {
        lock_release(&station->lock);
        return;
    }
    while (1) {
        if (count == 0 || station->passenger_count == 0)
            break;
        count -= 1;
        cond_signal(&station->cond_pass, &station->lock);
        cond_wait(&station->cond, &station->lock);
    }
    lock_release(&station->lock);
}


void passenger_waitfor_bus(struct station *station) {

    lock_acquire(&station->lock);
    station->passenger_count +=1;
    cond_wait(&station->cond_pass, &station->lock);
    lock_release(&station->lock);

}

void passenger_on_board(struct station *station) {
    lock_acquire(&station->lock);
    station->passenger_count -= 1;
    cond_signal(&station->cond, &station->lock);
    lock_release(&station->lock);
}
