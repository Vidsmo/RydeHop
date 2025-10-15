#include "ride_matching.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

RideMatch *create_ride_match()
{
    RideMatch *system = (RideMatch *)malloc(sizeof(RideMatch));
    if (system == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    system->available_drivers = create_linked_list();
    system->active_rides = create_hashtable(100);
    system->next_ride_id = 1;
    return system;
}

Ride *request_ride(RideMatch *system, struct User *rider, struct Location *pickup, struct Location *dropoff)
{
    if (system == NULL || rider == NULL || pickup == NULL || dropoff == NULL)
    {
        printf("Invalid ride request data\n");
        return NULL;
    }

    double distance = calculate_distance(pickup, dropoff);
    Ride *ride = create_ride(system->next_ride_id++, rider, pickup, dropoff, distance);

    struct Driver *driver = find_best_driver(system, pickup);
    if (driver)
    {
        ride_start(ride, driver);
        driver->is_available = 0;
        insert_hashtable(system->active_rides, ride->ride_id, ride);
        printf("Ride %d started with driver %s\n", ride->ride_id, driver->name);
    }
    else
    {
        printf("No available drivers nearby.\n");
    }

    return ride;
}

struct Driver *find_best_driver(RideMatch *system, struct Location *pickup)
{
    if (system == NULL || pickup == NULL || system->available_drivers == NULL)
    {
        printf("Invalid system or location data\n");
        return NULL;
    }

    ListNode *curr = system->available_drivers->head;
    if (curr == NULL)
    {
        printf("No drivers available\n");
        return NULL;
    }

    struct Driver *best_driver = NULL;
    double min_distance = 1e9;

    while (curr != NULL)
    {
        struct Driver *driver = curr->data;
        if (driver->is_available)
        {
            double dist = calculate_distance(driver->current_location, pickup);
            if (dist < min_distance)
            {
                min_distance = dist;
                best_driver = driver;
            }
        }
        curr = curr->next;
    }

    return best_driver;
}

void complete_ride(RideMatch *system, int ride_id)
{
    if (system == NULL || system->active_rides == NULL)
    {
        printf("System not initialized properly\n");
        return;
    }

    Ride *ride = get_from_hashtable(system->active_rides, ride_id);
    if (ride == NULL)
    {
        printf("Ride not found\n");
        return;
    }

    ride->status = "Completed";
    if (ride->driver)
        ride->driver->is_available = 1;

    printf("Ride %d completed successfully.\n", ride_id);
}

int cancel_ride(RideMatch *system, int ride_id)
{
    if (system == NULL || system->active_rides == NULL)
    {
        printf("System not initialized properly\n");
        return 0;
    }

    Ride *ride = get_from_hashtable(system->active_rides, ride_id);
    if (ride == NULL)
    {
        printf("Ride not found.\n");
        return 0;
    }

    ride->status = "Cancelled";
    if (ride->driver)
        ride->driver->is_available = 1;

    remove_from_hashtable(system->active_rides, ride_id);
    printf("Ride %d cancelled successfully.\n", ride_id);
    return 1;
}

void update_driver_location(RideMatch *system, int driver_id, struct Location *new_location)
{
    if (system == NULL || new_location == NULL)
        return;

    ListNode *curr = system->available_drivers->head;
    while (curr != NULL)
    {
        struct Driver *driver = curr->data;
        if (driver->driver_id == driver_id)
        {
            driver->current_location = new_location;
            printf("Driver %s location updated.\n", driver->name);
            break;
        }
        curr = curr->next;
    }
}

void print_active_rides(RideMatch *system)
{
    if (system == NULL || system->active_rides == NULL)
    {
        printf("No active rides\n");
        return;
    }

    printf("\n--- ACTIVE RIDES ---\n");

    for (int i = 0; i < system->active_rides->size; i++)
    {
        ListNode *curr = system->active_rides->buckets[i];
        while (curr != NULL)
        {
            Ride *ride = curr->data;
            printf("Ride ID: %d | Rider: %s | Driver: %s | Status: %s\n",
                   ride->ride_id,
                   ride->rider->name,
                   ride->driver ? ride->driver->name : "None",
                   ride->status);
            curr = curr->next;
        }
    }
    printf("---------------------\n");
}

void free_ride_match(RideMatch *system)
{
    if (system == NULL)
        return;

    free_linked_list(system->available_drivers);
    free_hashtable(system->active_rides);
    free(system);
}
