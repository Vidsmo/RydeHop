// ride.c
#include "ride.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BASE_FARE 30.0
#define COST_PER_KM 10.0
#define WAITING_COST_PER_MIN 1.0
#define AVG_SPEED_KMPH 40.0 // used to estimate duration

// Helper: calculate estimated duration (in minutes)
static double estimate_duration(double distance_km) {
    return (distance_km / AVG_SPEED_KMPH) * 60.0;
}

// Helper: calculate fare with surge and waiting
static double calculate_fare(double distance, double waiting, double surge) {
    return BASE_FARE + (distance * COST_PER_KM * surge) + (waiting * WAITING_COST_PER_MIN);
}

Ride* create_ride(int id, User *rider, Location *pickup, Location *dropoff, double distance) {
    Ride *ride = (Ride*)malloc(sizeof(Ride));
    ride->id = id;
    ride->rider = rider;
    ride->driver = NULL;
    ride->pickup = pickup;
    ride->dropoff = dropoff;
    ride->distance = distance;
    ride->waiting_time = 0;
    ride->surge_multiplier = 1.0;
    ride->estimated_time = estimate_duration(distance);
    ride->fare = calculate_fare(distance, 0, 1.0);
    ride->status = RIDE_REQUESTED;
    ride->request_time = time(NULL);
    ride->start_time = 0;
    ride->end_time = 0;
    ride->rider_rating = 0;
    ride->driver_rating = 0;

    printf("🚕 Ride #%d created. Estimated fare: ₹%.2f | ETA: %.1f min\n", 
           ride->id, ride->fare, ride->estimated_time);
    return ride;
}

void ride_start(Ride *ride, Driver *driver) {
    if (!ride || ride->status != RIDE_REQUESTED) {
        printf("⚠️ Cannot start ride: invalid state.\n");
        return;
    }

    ride->driver = driver;
    ride->status = RIDE_IN_PROGRESS;
    ride->start_time = time(NULL);
    driver->is_available = 0;

    printf("🛣️ Ride #%d started by driver %s.\n", ride->id, driver->user_info->name);
}

void ride_complete(Ride *ride) {
    if (!ride || ride->status != RIDE_IN_PROGRESS) {
        printf("⚠️ Cannot complete ride: not in progress.\n");
        return;
    }

    ride->status = RIDE_COMPLETED;
    ride->end_time = time(NULL);

    if (ride->driver) {
        driver_add_earning(ride->driver, ride->fare);
        ride->driver->is_available = 1;
    }
    if (ride->rider) {
        user_add_ride(ride->rider, ride);
    }

    printf("✅ Ride #%d completed. Fare: ₹%.2f\n", ride->id, ride->fare);
}

void ride_cancel(Ride *ride) {
    if (!ride || ride->status == RIDE_COMPLETED) {
        printf("⚠️ Cannot cancel this ride.\n");
        return;
    }
    ride->status = RIDE_CANCELLED;
    ride->end_time = time(NULL);
    printf("❌ Ride #%d cancelled by rider.\n", ride->id);
}

void ride_rate(Ride *ride, int rider_rating, int driver_rating) {
    if (!ride || ride->status != RIDE_COMPLETED) {
        printf("⚠️ Can only rate completed rides.\n");
        return;
    }

    ride->rider_rating = rider_rating;
    ride->driver_rating = driver_rating;

    printf("⭐ Ride #%d rated: Rider → %d★ | Driver → %d★\n",
           ride->id, rider_rating, driver_rating);
}

void print_ride_summary(const Ride *ride) {
    if (!ride) return;

    char start_buf[30], end_buf[30];
    strftime(start_buf, sizeof(start_buf), "%H:%M:%S", localtime(&ride->start_time));
    strftime(end_buf, sizeof(end_buf), "%H:%M:%S", localtime(&ride->end_time));

    printf("\n===== RIDE SUMMARY =====\n");
    printf("Ride ID     : %d\n", ride->id);
    printf("Rider       : %s\n", ride->rider ? ride->rider->name : "Unknown");
    printf("Driver      : %s\n", ride->driver ? ride->driver->user_info->name : "Not Assigned");
    printf("Distance    : %.2f km\n", ride->distance);
    printf("Fare        : ₹%.2f\n", ride->fare);
    printf("Status      : %s\n",
           ride->status == RIDE_REQUESTED ? "Requested" :
           ride->status == RIDE_IN_PROGRESS ? "In Progress" :
           ride->status == RIDE_COMPLETED ? "Completed" : "Cancelled");
    printf("Start Time  : %s\n", ride->start_time ? start_buf : "N/A");
    printf("End Time    : %s\n", ride->end_time ? end_buf : "N/A");
    printf("Ratings     : Rider → %d★ | Driver → %d★\n",
           ride->rider_rating, ride->driver_rating);
    printf("=========================\n\n");
}

void log_ride(const Ride *ride) {
    if (!ride) return;
    FILE *file = fopen("rides.log", "a");
    if (!file) return;

    fprintf(file, "ID:%d,Fare:%.2f,Distance:%.2f,Status:%d,Driver:%s,Rider:%s,Time:%ld\n",
            ride->id,
            ride->fare,
            ride->distance,
            ride->status,
            ride->driver ? ride->driver->user_info->name : "N/A",
            ride->rider ? ride->rider->name : "N/A",
            ride->end_time);
    fclose(file);
}

void free_ride(Ride *ride) {
    if (!ride) return;
    free(ride);
}
