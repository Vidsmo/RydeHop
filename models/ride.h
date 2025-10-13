// ride.h
#ifndef RIDE_H
#define RIDE_H

#include <time.h>
#include "user.h"
#include "driver.h"
#include "location.h"

typedef enum {
    RIDE_REQUESTED,
    RIDE_IN_PROGRESS,
    RIDE_COMPLETED,
    RIDE_CANCELLED
} RideStatus;

typedef struct {
    int id;
    User *rider;
    Driver *driver;
    Location *pickup;
    Location *dropoff;
    double distance;        // in kilometers
    double fare;
    double waiting_time;    // in minutes
    double estimated_time;  // in minutes
    double surge_multiplier;
    RideStatus status;
    time_t request_time;
    time_t start_time;
    time_t end_time;
    int rider_rating;       // 1–5 stars
    int driver_rating;      // 1–5 stars
} Ride;

Ride* create_ride(int id, User *rider, Location *pickup, Location *dropoff, double distance);
void ride_start(Ride *ride, Driver *driver);
void ride_complete(Ride *ride);
void ride_cancel(Ride *ride);
void ride_rate(Ride *ride, int rider_rating, int driver_rating);
void print_ride_summary(const Ride *ride);
void log_ride(const Ride *ride);
void free_ride(Ride *ride);

#endif
