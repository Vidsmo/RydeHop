#include "navigation.h"
#include <stdlib.h>

NavigationSystem* create_navigation_system() {
    NavigationSystem *nav = (NavigationSystem*)malloc(sizeof(NavigationSystem));
    if(!nav)
         return NULL;
    nav->city_map = create_graph();
    nav->favorite_locations = create_linked_list();
    return nav;
}

void free_navigation_system( NavigationSystem* nav)
{
    if (!nav)
         return ;
    free_graph(nav->city_map);
    free_linked_list(nav->favorite_locations);
    free(nav) ;
}

void add_favorite_location( NavigationSystem* nav , GraphNode location )
{
    if(!nav || !location )
          return ;
     append_to_linked_list(nav->favorite_locations,location);
 }
void remove_favorite_location( NavigationSystem* nav , GraphNode location )
{
    if(!nav || !location )
          return ;
     remove_from_linked_list(nav->favorite_locations,location);
 }
 LinkedList* get_favorite_locations(NavigationSystem *nav)
{
    return nav ? nav->favorite_locations : NULL;
}

double estimate_travel_time(double distance)
{
    const double AVERAGE_SPEED = 50.0 ;
    return distance / AVERAGE_SPEED ;
}
double estimate_travel_time_with_traffic(double distance, double traffic_factor) {
    return estimate_travel_time(distance) * traffic_factor; 
}

void update_traffic(GraphNode *node, double congestion_level) {
    if (!node) 
        return;
    node->traffic_level = congestion_level;  
}

double get_traffic_level(GraphNode *node) {
    return node ? node->traffic_level : 1.0; 
}

LinkedList* find_shortest_route(NavigationSystem *nav, GraphNode *start, GraphNode *end) {
    if (!nav || !start || !end) 
           return NULL;
    int n = nav->city_map->node_count;
    double dist[n];
    GraphNode* prev[n];
    int visited[n];

    for (int i = 0; i < n; i++) {
        dist[i] = DBL_MAX;
        prev[i] = NULL;
        visited[i] = 0;
    }

    dist[start->id] = 0;

    for (int count = 0; count < n; count++) {
        double min = DBL_MAX;
        int u = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        GraphNode *node = nav->city_map->nodes[u];
        for (int i = 0; i < node->neighbor_count; i++) {
            GraphEdge *edge = node->neighbors[i];
            int v = edge->to->id;
            double weight = edge->distance * get_traffic_level(edge->to);
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = node;
            }
        }
    }

    LinkedList *path = create_linked_list();
    GraphNode *curr = end;
    while (curr) {
        prepend_to_linked_list(path, curr); 
        curr = prev[curr->id];
    }

    return path;
}


LinkedList* find_alternative_routes(NavigationSystem *nav, GraphNode *start, GraphNode *end, int max_routes) {
    
    LinkedList *routes = create_linked_list();
    for (int i = 0; i < max_routes; i++) {
        LinkedList *path = find_shortest_route(nav, start, end);
        append_to_linked_list(routes, path);
    }
    return routes;
}

