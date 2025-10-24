#ifndef GRAPH_H
#define GRAPH_H

#include "linked_list.h"
#include "hash_table.h"

typedef struct GraphNode {
    int id;
    char *name;                 
    double latitude;
    double longitude;
    LinkedList *neighbors;      
} GraphNode;

typedef struct GraphEdge {
    GraphNode *source;
    GraphNode *destination;
    double distance;           
    double time;                
    double traffic_factor;      
} GraphEdge;

typedef struct {
    HashTable *nodes;           
    LinkedList *edges;          
    int next_node_id;           
} Graph;

Graph* create_graph();

GraphNode* graph_add_node(Graph *graph, const char *name, double lat, double lon);

void graph_add_edge(Graph *graph, GraphNode *src, GraphNode *dest, double distance, double traffic_factor);

LinkedList* find_shortest_path(Graph *graph, GraphNode *start, GraphNode *end, int use_traffic_factor);

LinkedList* find_nearby_locations(Graph *graph, double latitude, double longitude, double radius_km);

void graph_remove_node(Graph *graph, GraphNode *node);
void graph_remove_edge(Graph *graph, GraphNode *src, GraphNode *dest);

void free_graph(Graph *graph);

#endif 
