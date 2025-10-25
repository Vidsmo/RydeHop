#ifndef RIDE_MATCH_H
#define RIDE_MATCH_H

#include "../src/data_structures/linked_list.h"
#include "../src/data_structures/hash_table.h"
#include "../models/ride.h"
#include "../models/driver.h"
#include "../models/hopper.h"
#include "../models/location.h"

#define MAX 100

struct RideMatch
{
    LinkedList *available_drivers;
    HashTable *active_rides;
    int next_ride_id;
};

typedef struct RideMatch RideMatch;

RideMatch *create_ride_match();
Ride *request_ride(RideMatch *system, struct Uset *rider, struct Location *pickup, struct Location *dropoff);
struct Driver *find_best_driver(RideMatch *system, struct Location *pickup);
void complete_ride(RideMatch *system, int ride_id);
int cancel_ride(RideMatch *system, int ride_id);
void update_driver_location(RideMatch *system, int driver_id, struct Location *new_location);
void print_active_rides(RideMatch *system);
void free_ride_match(RideMatch *system);

#endif 
