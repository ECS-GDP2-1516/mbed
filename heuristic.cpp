#include "heuristic.h"

int heur_rear = -1;
int last_value = -1;
int heur_buffer[HEUR_SIZE]; //store the latest values from the classifier
int max_not_ex = 0;

void init_heur() {
    for(int i=0; i<HEUR_SIZE; i++) {
        heur_buffer[i] = NOT_EX;
    }
}

void insert_value(int value) {
    if(value != last_value) {
        heur_rear=(heur_rear+1)%HEUR_SIZE;
        heur_buffer[heur_rear]=value;
        last_value = value;
    }
}

int heur_classify(int value) {
    insert_value(value);
    int not_ex = 0;
    for(int i=0; i<HEUR_SIZE; i++) {
        if(heur_buffer[i] == NOT_EX) {
            not_ex++;
            if(not_ex > max_not_ex) {
                return 0;
            }
        }
    }
    return 1;
}
