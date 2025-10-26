/*#include "driver.h"
#include <stdlib.h>

Driver* create_driver(User *user, Vehicle *vehicle) {
    Driver *driver = (Driver*)malloc(sizeof(Driver));
    driver->user_info = user;
    driver->vehicle = vehicle;
    driver->earnings = 0.0;
    driver->is_available = 1;
    driver->current_rides = create_linked_list();
    return driver;
}

void driver_add_earning(Driver *driver, double amount) {
    driver->earnings += amount;
}

void free_driver(Driver *driver) {
    free_linked_list(driver->current_rides);
    free(driver);
}
*/

#include "driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void validateLicense(char license[])
{
    if (strlen(license) < 5)
    {
        printf("Invalid license number! Should be at least 5 characters.\n");
        exit(1);
    }
}

Driver* create_driver(User *user, Vehicle *vehicle)
{
    Driver *d = (Driver*)malloc(sizeof(Driver));
    d->user_info = user;
    d->vehicle = vehicle;
    d->earnings = 0.0;
    d->is_available = 1;
    d->current_rides = NULL;
    return d;
}

void driver_add_earning(Driver *driver, double amount)
{
    driver->earnings += amount;
}

void free_driver(Driver *driver)
{
    if (!driver) return;
    free(driver->user_info);
    free(driver->vehicle);
    // Free rides list if needed
    free(driver);
}

void saveDriver(Driver d)
{
    FILE *p = fopen("drivers.txt", "a");
    if (p != NULL)
    {
        fprintf(p, "%s %s %s %s %s %s %s %s %.2f %d\n",
                d.user_info->username,
                d.user_info->password,
                d.user_info->phone,
                d.user_info->email,
                d.vehicle->license_no,
                d.vehicle->model,
                d.vehicle->color,
                d.vehicle->plate_no,
                d.earnings,
                d.is_available);
        printf("Driver registered successfully!\n");
        fclose(p);
    }
    else
    {
        printf("Error opening drivers file!\n");
    }
}

Driver registerDriver()
{
    User *u = (User*)malloc(sizeof(User));
    Vehicle *v = (Vehicle*)malloc(sizeof(Vehicle));
    Driver d;

    printf("Enter username: ");
    scanf("%s", u->username);

    printf("Enter password: ");
    scanf("%s", u->password);

    printf("Enter phone: ");
    scanf("%s", u->phone);

    printf("Enter email: ");
    scanf("%s", u->email);

    printf("Enter license number: ");
    scanf("%s", v->license_no);
    validateLicense(v->license_no);

    printf("Enter vehicle model: ");
    scanf("%s", v->model);

    printf("Enter vehicle color: ");
    scanf("%s", v->color);

    printf("Enter vehicle plate number: ");
    scanf("%s", v->plate_no);

    d.user_info = u;
    d.vehicle = v;
    d.earnings = 0.0;
    d.is_available = 1;
    d.current_rides = NULL;

    saveDriver(d);
    return d;
}

int loginDriver(Driver *loggedDriver)
{
    char username[50], password[50];
    FILE *p = fopen("drivers.txt", "r");
    if (!p)
    {
        printf("No drivers registered yet.\n");
        return 0;
    }

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    Driver d;
    User *u = (User*)malloc(sizeof(User));
    Vehicle *v = (Vehicle*)malloc(sizeof(Vehicle));

    while (fscanf(p, "%s %s %s %s %s %s %s %s %lf %d",
                  u->username,
                  u->password,
                  u->phone,
                  u->email,
                  v->license_no,
                  v->model,
                  v->color,
                  v->plate_no,
                  &d.earnings,
                  &d.is_available) != EOF)
    {
        if (strcmp(u->username, username) == 0 && strcmp(u->password, password) == 0)
        {
            d.user_info = u;
            d.vehicle = v;
            d.current_rides = NULL;
            *loggedDriver = d;
            fclose(p);
            printf("Login successful! Welcome, %s.\n", username);
            return 1;
        }
    }

    fclose(p);
    printf("Invalid username or password.\n");
    free(u);
    free(v);
    return 0;
}

void showAvailableDrivers()
{
    FILE *p = fopen("drivers.txt", "r");
    if (!p)
    {
        printf("No drivers found.\n");
        return;
    }

    User u;
    Vehicle v;
    double earnings;
    int is_available;
    int found = 0;

    printf("\nAvailable Drivers:\n");
    printf("------------------------------------------\n");
    while (fscanf(p, "%s %s %s %s %s %s %s %s %lf %d",
                  u.username, u.password, u.phone, u.email,
                  v.license_no, v.model, v.color, v.plate_no,
                  &earnings, &is_available) != EOF)
    {
        if (is_available)
        {
            printf("Driver: %s | Vehicle: %s (%s) | Plate: %s | Phone: %s\n",
                   u.username, v.model, v.color, v.plate_no, u.phone);
            found = 1;
        }
    }

    if (!found)
        printf("No available drivers currently.\n");

    fclose(p);
}
