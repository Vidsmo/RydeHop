#include "pricing_engine.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

PricingEngine* create_pricing_engine() {
    PricingEngine *engine = (PricingEngine*)malloc(sizeof(PricingEngine));
    if (!engine) return NULL;

    engine->base_fare = 30.0;
    engine->per_km_rate = 10.0;
    engine->per_minute_rate = 2.0;
    engine->surge_multiplier = 1.0;
    engine->discount_rate = 0.0;
    engine->peak_hour_multiplier = 1.5;
    engine->tax_rate = 5.0; // 5% GST
    strcpy(engine->ride_type, "Car");

    return engine;
}

void set_surge_multiplier(PricingEngine *engine, double surge) {
    if (engine && surge > 0)
        engine->surge_multiplier = surge;
}

void set_discount(PricingEngine *engine, double discount) {
    if (engine && discount >= 0 && discount <= 100)
        engine->discount_rate = discount;
}

void set_peak_hour_multiplier(PricingEngine *engine, double multiplier) {
    if (engine && multiplier >= 1)
        engine->peak_hour_multiplier = multiplier;
}

void set_ride_type(PricingEngine *engine, const char *type) {
    if (engine && type)
        strncpy(engine->ride_type, type, sizeof(engine->ride_type) - 1);
}

void set_tax_rate(PricingEngine *engine, double tax) {
    if (engine && tax >= 0)
        engine->tax_rate = tax;
}

double apply_discount(double fare, double discount_rate) {
    return fare - (fare * discount_rate / 100.0);
}

double apply_tax(double fare, double tax_rate) {
    return fare + (fare * tax_rate / 100.0);
}

int is_peak_hour_now() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int hour = tm_info->tm_hour;
    return ((hour >= 8 && hour <= 10) || (hour >= 18 && hour <= 21));
}

double calculate_fare(PricingEngine *engine, double distance, double duration, int is_peak_hour) {
    if (!engine) return 0.0;

    double fare = engine->base_fare;
    fare += (distance * engine->per_km_rate);
    fare += (duration * engine->per_minute_rate);

    double surge = engine->surge_multiplier;
    if (is_peak_hour)
        surge *= engine->peak_hour_multiplier;
    fare *= surge;

    fare = apply_discount(fare, engine->discount_rate);

    fare = apply_tax(fare, engine->tax_rate);

    return fare;
}

double get_final_fare(PricingEngine *engine, double base_fare, int is_peak_hour) {
    double fare = base_fare;
    if (is_peak_hour)
        fare *= engine->peak_hour_multiplier;
    fare = apply_discount(fare, engine->discount_rate);
    fare = apply_tax(fare, engine->tax_rate);
    return fare;
}

void print_pricing_details(PricingEngine *engine) {
    if (!engine) return;

    printf("---- Pricing Engine Details ----\n");
    printf("Ride Type: %s\n", engine->ride_type);
    printf("Base Fare: %.2f\n", engine->base_fare);
    printf("Per KM Rate: %.2f\n", engine->per_km_rate);
    printf("Per Minute Rate: %.2f\n", engine->per_minute_rate);
    printf("Surge Multiplier: %.2f\n", engine->surge_multiplier);
    printf("Peak Hour Multiplier: %.2f\n", engine->peak_hour_multiplier);
    printf("Discount: %.2f%%\n", engine->discount_rate);
    printf("Tax Rate: %.2f%%\n", engine->tax_rate);
    printf("--------------------------------\n");
}


void free_pricing_engine(PricingEngine *engine) {
    if (engine)
        free(engine);
}
