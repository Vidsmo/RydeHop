/*#ifndef VEHICLE_H
#define VEHICLE_H

typedef struct {
    char *license_plate;
    char *make;
    char *model;
    int year;
    int capacity;
    char *color;
} Vehicle;

Vehicle* create_vehicle(char *license_plate, char *make, char *model, int year, int capacity);
void free_vehicle(Vehicle *vehicle);

#endif
*/


#ifndef VEHICLE_H
#define VEHICLE_H

#include <stdbool.h>

/*
 * RideUnity Vehicle Management Module
 * -----------------------------------
 * Defines the vehicle data structure and core ride-sharing operations
 * such as vehicle creation, route assignment, fare calculation,
 * and ride matching (foundation for the full RideUnity system).
 */

/* Structure to store route information */
typedef struct {
    char *start_location;
    char *destination;
    float distance_km;
} Route;

/* Structure to store vehicle information */
typedef struct {
    char *license_plate;
    char *driver_name;
    char *make;
    char *model;
    char *color;
    int year;
    int capacity;
    int passengers_onboard;
    float fare_per_km;
    bool available;
    Route *current_route;
} Vehicle;

/* ==== VEHICLE CREATION & MANAGEMENT ==== */
Vehicle* create_vehicle(char *license_plate, char *driver_name,
                        char *make, char *model, int year,
                        int capacity, float fare_per_km);

void free_vehicle(Vehicle *vehicle);
void display_vehicle_info(const Vehicle *vehicle);

/* ==== ROUTE & STATUS OPERATIONS ==== */
void assign_route(Vehicle *vehicle, char *start_location,
                  char *destination, float distance_km);

void update_availability(Vehicle *vehicle, bool available);

/* ==== RIDE OPERATIONS ==== */
float calculate_fare(const Vehicle *vehicle, int passengers);
bool match_route(const Vehicle *vehicle, const char *start, const char *dest);

#endif



