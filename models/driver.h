/*
#ifndef DRIVER_H
#define DRIVER_H
#include "../src/data_structures/linked_list.h"

#include "user.h"
#include "vehicle.h"

typedef struct {
    User *user_info;
    Vehicle *vehicle;
    double earnings;
    int is_available;
    LinkedList *current_rides;
} Driver;

Driver* create_driver(User *user, Vehicle *vehicle);
void driver_add_earning(Driver *driver, double amount);
void free_driver(Driver *driver);

#endif
*/
#ifndef DRIVER_H
#define DRIVER_H

#include "../src/data_structures/linked_list.h"
#include "user.h"
#include "vehicle.h"

typedef struct {
    User *user_info;          // Pointer to driver's personal info
    Vehicle *vehicle;         // Vehicle details
    double earnings;          // Total earnings
    int is_available;         // 1 = available, 0 = not available
    LinkedList *current_rides;// List of active rides
} Driver;

// Constructor-like functions
Driver* create_driver(User *user, Vehicle *vehicle);
void driver_add_earning(Driver *driver, double amount);
void free_driver(Driver *driver);

// Authentication & Management
void validateLicense(char license[]);
void saveDriver(Driver d);
Driver registerDriver();
int loginDriver(Driver *loggedDriver);
void showAvailableDrivers();

#endif

