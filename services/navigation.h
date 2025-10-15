#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "../src/data_structures/linked_list.h"
#include "../data_structures/graph.h"
#include <time.h>

typedef struct {
    Graph *city_map;
    LinkedList *favorite_locations;  
} NavigationSystem;

NavigationSystem* create_navigation_system();
void free_navigation_system(NavigationSystem *nav);

LinkedList* find_route(NavigationSystem *nav, GraphNode *start, GraphNode *end);         
LinkedList* find_shortest_route(NavigationSystem *nav, GraphNode *start, GraphNode *end); 
LinkedList* find_alternative_routes(NavigationSystem *nav, GraphNode *start, GraphNode *end, int max_routes); 

double estimate_travel_time(double distance);
double estimate_travel_time_with_traffic(double distance, double traffic_factor); 

void update_traffic(GraphNode *node, double congestion_level);
double get_traffic_level(GraphNode *node);

void add_favorite_location(NavigationSystem *nav, GraphNode *location);
void remove_favorite_location(NavigationSystem *nav, GraphNode *location);
LinkedList* get_favorite_locations(NavigationSystem *nav);

#endif
