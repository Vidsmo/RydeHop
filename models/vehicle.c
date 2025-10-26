/*#include "vehicle.h"
#include <stdlib.h>
#include <string.h>

Vehicle* create_vehicle(char *license_plate, char *make, char *model, int year, int capacity) {
    Vehicle *vehicle = (Vehicle*)malloc(sizeof(Vehicle));
    vehicle->license_plate = strdup(license_plate);
    vehicle->make = strdup(make);
    vehicle->model = strdup(model);
    vehicle->year = year;
    vehicle->capacity = capacity;
    vehicle->color = strdup("Black"); // Default color
    return vehicle;
}

void free_vehicle(Vehicle *vehicle) {
    free(vehicle->license_plate);
    free(vehicle->make);
    free(vehicle->model);
    free(vehicle->color);
    free(vehicle);
}*/
#include "vehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create and initialize a vehicle */
Vehicle* create_vehicle(char *license_plate, char *driver_name,
                        char *make, char *model, int year,
                        int capacity, float fare_per_km) {
    Vehicle *vehicle = (Vehicle*)malloc(sizeof(Vehicle));
    if (!vehicle) return NULL;

    vehicle->license_plate = strdup(license_plate);
    vehicle->driver_name = strdup(driver_name);
    vehicle->make = strdup(make);
    vehicle->model = strdup(model);
    vehicle->color = strdup("Black");  // Default color
    vehicle->year = year;
    vehicle->capacity = capacity;
    vehicle->passengers_onboard = 0;
    vehicle->fare_per_km = fare_per_km;
    vehicle->available = true;
    vehicle->current_route = NULL;

    return vehicle;
}

/* Free all allocated memory for a vehicle */
void free_vehicle(Vehicle *vehicle) {
    if (!vehicle) return;
    free(vehicle->license_plate);
    free(vehicle->driver_name);
    free(vehicle->make);
    free(vehicle->model);
    free(vehicle->color);

    if (vehicle->current_route) {
        free(vehicle->current_route->start_location);
        free(vehicle->current_route->destination);
        free(vehicle->current_route);
    }

    free(vehicle);
}

/* Assign a route to a vehicle */
void assign_route(Vehicle *vehicle, char *start_location,
                  char *destination, float distance_km) {
    if (!vehicle) return;

    if (vehicle->current_route)
        free_vehicle(vehicle); // clean old route if any

    vehicle->current_route = (Route*)malloc(sizeof(Route));
    vehicle->current_route->start_location = strdup(start_location);
    vehicle->current_route->destination = strdup(destination);
    vehicle->current_route->distance_km = distance_km;
}

/* Update vehicle availability (true = available, false = in transit) */
void update_availability(Vehicle *vehicle, bool available) {
    if (vehicle)
        vehicle->available = available;
}

/* Calculate fare for a given number of passengers */
float calculate_fare(const Vehicle *vehicle, int passengers) {
    if (!vehicle || passengers <= 0 || !vehicle->current_route)
        return 0.0;
    return (vehicle->current_route->distance_km * vehicle->fare_per_km) / passengers;
}

/* Match route based on start and destination (exact match for now) */
bool match_route(const Vehicle *vehicle, const char *start, const char *dest) {
    if (!vehicle || !vehicle->current_route) return false;

    return strcmp(vehicle->current_route->start_location, start) == 0 &&
           strcmp(vehicle->current_route->destination, dest) == 0;
}

/* Display all vehicle details */
void display_vehicle_info(const Vehicle *vehicle) {
    if (!vehicle) return;

    printf("\n--- Vehicle Info ---\n");
    printf("Driver: %s\n", vehicle->driver_name);
    printf("License Plate: %s\n", vehicle->license_plate);
    printf("Make/Model: %s %s (%d)\n", vehicle->make, vehicle->model, vehicle->year);
    printf("Color: %s\n", vehicle->color);
    printf("Capacity: %d | Passengers Onboard: %d\n", vehicle->capacity, vehicle->passengers_onboard);
    printf("Fare per km: %.2f | Available: %s\n",
           vehicle->fare_per_km, vehicle->available ? "Yes" : "No");

    if (vehicle->current_route)
        printf("Route: %s → %s (%.2f km)\n",
               vehicle->current_route->start_location,
               vehicle->current_route->destination,
               vehicle->current_route->distance_km);
    else
        printf("No route assigned.\n");
}


