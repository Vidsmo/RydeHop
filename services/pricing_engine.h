#ifndef PRICING_ENGINE_H
#define PRICING_ENGINE_H

#include "../models/ride.h"
#include <time.h>

typedef struct {
    double base_fare;
    double per_km_rate;
    double per_minute_rate;
    double surge_multiplier;
    double discount_rate;      
    double peak_hour_multiplier;
    double tax_rate;           
    char ride_type[20];       
} PricingEngine;

PricingEngine* create_pricing_engine();

void set_surge_multiplier(PricingEngine *engine, double surge);
void set_discount(PricingEngine *engine, double discount);
void set_peak_hour_multiplier(PricingEngine *engine, double multiplier);
void set_ride_type(PricingEngine *engine, const char *type);
void set_tax_rate(PricingEngine *engine, double tax);

double calculate_fare(PricingEngine *engine, double distance, double duration, int is_peak_hour);

double apply_discount(double fare, double discount_rate);
double apply_tax(double fare, double tax_rate);
double get_final_fare(PricingEngine *engine, double base_fare, int is_peak_hour);

void print_pricing_details(PricingEngine *engine);

void free_pricing_engine(PricingEngine *engine);

#endif
