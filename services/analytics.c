//*#include "analytics.h"
#include <stdlib.h>

typedef struct {
    int ride_id;
    Ride *ride;
} AnalyticsEntry;

int analytics_compare(void *a, void *b) {
    AnalyticsEntry *entry_a = (AnalyticsEntry*)a;
    AnalyticsEntry *entry_b = (AnalyticsEntry*)b;
    return entry_a->ride_id - entry_b->ride_id;
}

AnalyticsSystem* create_analytics_system() {
    AnalyticsSystem *system = (AnalyticsSystem*)malloc(sizeof(AnalyticsSystem));
    system->ride_data = create_bst(analytics_compare);
    system->total_earnings = 0.0;
    system->total_rides = 0;
    system->total_distance = 0.0;
    return system;
}

void add_ride_analytics(AnalyticsSystem *system, Ride *ride) {
    AnalyticsEntry *entry = (AnalyticsEntry*)malloc(sizeof(AnalyticsEntry));
    entry->ride_id = ride->id;
    entry->ride = ride;
    
    bst_insert(system->ride_data, entry);
    
    system->total_earnings += ride->fare;
    system->total_rides++;
    system->total_distance += ride->distance;
}

Ride* find_ride_by_id(AnalyticsSystem *system, int ride_id) {
    AnalyticsEntry search_key = {ride_id, NULL};
    AnalyticsEntry *found = (AnalyticsEntry*)bst_search(system->ride_data, &search_key);
    return found ? found->ride : NULL;
}

double get_total_earnings(AnalyticsSystem *system) {
    return system->total_earnings;
}

int get_total_rides(AnalyticsSystem *system) {
    return system->total_rides;
}

double get_average_fare(AnalyticsSystem *system) {
    return system->total_rides > 0 ? system->total_earnings / system->total_rides : 0.0;
}

void free_analytics_system(AnalyticsSystem *system) {
    free_bst(system->ride_data);
    free(system);
}        */




#include <stdio.h>
#include <string.h>
#include <time.h>

// Estimated CO2 saved per km ride-sharing compared to solo travel
#define CO2_SAVING_FACTOR 0.180

int get_peak_hour(AnalyticsSystem *system) {
    int hour_freq[24] = {0};

    void count_hours(Node *node) {
        if (!node) return;
        AnalyticsEntry *entry = (AnalyticsEntry*)node->data;
        struct tm *timeinfo = localtime(&entry->ride->timestamp);
        hour_freq[timeinfo->tm_hour]++;
        count_hours(node->left);
        count_hours(node->right);
    }
    count_hours(system->ride_data->root);

    int peak_hour = 0;
    for (int i = 1; i < 24; i++)
        if (hour_freq[i] > hour_freq[peak_hour])
            peak_hour = i;
    return peak_hour;
}

int get_route_frequency(AnalyticsSystem *system, const char *from, const char *to) {
    int count = 0;

    void count_routes(Node *node) {
        if (!node) return;
        AnalyticsEntry *entry = (AnalyticsEntry*)node->data;
        if (strcmp(entry->ride->from, from) == 0 &&
            strcmp(entry->ride->to, to) == 0)
            count++;
        count_routes(node->left);
        count_routes(node->right);
    }
    count_routes(system->ride_data->root);
    return count;
}

double get_average_driver_rating(AnalyticsSystem *system, int driver_id) {
    double total_rating = 0;
    int rating_count = 0;

    void sum_ratings(Node *node) {
        if (!node) return;
        AnalyticsEntry *entry = (AnalyticsEntry*)node->data;
        if (entry->ride->driver_id == driver_id && entry->ride->rating > 0) {
            total_rating += entry->ride->rating;
            rating_count++;
        }
        sum_ratings(node->left);
        sum_ratings(node->right);
    }
    sum_ratings(system->ride_data->root);
    return rating_count > 0 ? total_rating / rating_count : 0.0;
}

double get_total_co2_saved(AnalyticsSystem *system) {
    return system->total_distance * CO2_SAVING_FACTOR;
}

void export_analytics_csv(AnalyticsSystem *system, const char *filename) {
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "RideID,DriverID,Distance,Fare,Rating\n");

    void write(Node *node) {
        if (!node) return;
        AnalyticsEntry *entry = (AnalyticsEntry*)node->data;
        fprintf(fp, "%d,%d,%.2f,%.2f,%.1f\n",
            entry->ride->id,
            entry->ride->driver_id,
            entry->ride->distance,
            entry->ride->fare,
            entry->ride->rating
        );
        write(node->left);
        write(node->right);
    }
    write(system->ride_data->root);
    fclose(fp);
}
int get_peak_hour(AnalyticsSystem *system);
int get_route_frequency(AnalyticsSystem *system, const char *from, const char *to);
double get_average_driver_rating(AnalyticsSystem *system, int driver_id);
double get_total_co2_saved(AnalyticsSystem *system);
void export_analytics_csv(AnalyticsSystem *system, const char *filename);




