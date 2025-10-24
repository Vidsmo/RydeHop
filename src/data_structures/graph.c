#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define EARTH_RADIUS_KM 6371.0

static double calculate_distance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}

Graph* create_graph() {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;

    graph->nodes = create_hash_table(100);
    graph->edges = create_linked_list();
    graph->next_node_id = 1;

    return graph;
}

GraphNode* graph_add_node(Graph *graph, const char *name, double lat, double lon) {
    if (!graph || !name) return NULL;

    GraphNode *node = (GraphNode*)malloc(sizeof(GraphNode));
    node->id = graph->next_node_id++;
    node->name = strdup(name);
    node->latitude = lat;
    node->longitude = lon;
    node->neighbors = create_linked_list();

    hash_table_insert(graph->nodes, node->name, node);
    return node;
}

void graph_add_edge(Graph *graph, GraphNode *src, GraphNode *dest, double distance, double traffic_factor) {
    if (!graph || !src || !dest) return;

    GraphEdge *edge = (GraphEdge*)malloc(sizeof(GraphEdge));
    edge->source = src;
    edge->destination = dest;
    edge->distance = distance;
    edge->time = distance / (traffic_factor > 0 ? traffic_factor : 1.0);
    edge->traffic_factor = traffic_factor;

    linked_list_append(graph->edges, edge);
    linked_list_append(src->neighbors, edge);
}

LinkedList* find_shortest_path(Graph *graph, GraphNode *start, GraphNode *end, int use_traffic_factor) {
    if (!graph || !start || !end) return NULL;

    HashTable *dist = create_hash_table(100);
    HashTable *prev = create_hash_table(100);
    LinkedList *unvisited = create_linked_list();

    HashTableIterator it = hash_table_iterator(graph->nodes);
    while (hash_table_iterator_has_next(&it)) {
        GraphNode *node = hash_table_iterator_next_value(&it);
        hash_table_insert(dist, node->name, (void*)(long)(node == start ? 0 : 1e9));
        linked_list_append(unvisited, node);
    }

    while (unvisited->head) {
        GraphNode *min_node = NULL;
        double min_dist = 1e9;
        LinkedListNode *ln = unvisited->head;
        while (ln) {
            GraphNode *n = (GraphNode*)ln->data;
            double d = (double)(long)hash_table_get(dist, n->name);
            if (d < min_dist) {
                min_dist = d;
                min_node = n;
            }
            ln = ln->next;
        }

        if (!min_node) break;
        linked_list_remove(unvisited, min_node);

        if (min_node == end) break;

        LinkedListNode *edge_node = min_node->neighbors->head;
        while (edge_node) {
            GraphEdge *edge = (GraphEdge*)edge_node->data;
            GraphNode *neighbor = edge->destination;
            double weight = use_traffic_factor ? edge->time : edge->distance;
            double alt = min_dist + weight;
            double neighbor_dist = (double)(long)hash_table_get(dist, neighbor->name);

            if (alt < neighbor_dist) {
                hash_table_insert(dist, neighbor->name, (void*)(long)alt);
                hash_table_insert(prev, neighbor->name, min_node);
            }
            edge_node = edge_node->next;
        }
    }

    
    LinkedList *path = create_linked_list();
    for (GraphNode *at = end; at; at = (GraphNode*)hash_table_get(prev, at->name)) {
        linked_list_prepend(path, at);
        if (at == start) break;
    }

    free_hash_table(dist);
    free_hash_table(prev);
    free_linked_list(unvisited, 0);

    return path;
}


LinkedList* find_nearby_locations(Graph *graph, double latitude, double longitude, double radius_km) {
    if (!graph) return NULL;
    LinkedList *results = create_linked_list();

    HashTableIterator it = hash_table_iterator(graph->nodes);
    while (hash_table_iterator_has_next(&it)) {
        GraphNode *node = hash_table_iterator_next_value(&it);
        double dist = calculate_distance(latitude, longitude, node->latitude, node->longitude);
        if (dist <= radius_km)
            linked_list_append(results, node);
    }
    return results;
}


void graph_remove_edge(Graph *graph, GraphNode *src, GraphNode *dest) {
    if (!graph || !src || !dest) return;

    LinkedListNode *ln = src->neighbors->head;
    while (ln) {
        GraphEdge *edge = (GraphEdge*)ln->data;
        if (edge->destination == dest) {
            linked_list_remove(src->neighbors, edge);
            linked_list_remove(graph->edges, edge);
            free(edge);
            return;
        }
        ln = ln->next;
    }
}

void graph_remove_node(Graph *graph, GraphNode *node) {
    if (!graph || !node) return;

    LinkedListNode *e = graph->edges->head;
    while (e) {
        GraphEdge *edge = (GraphEdge*)e->data;
        if (edge->source == node || edge->destination == node) {
            linked_list_remove(graph->edges, edge);
            linked_list_remove(edge->source->neighbors, edge);
            free(edge);
        }
        e = e->next;
    }

    hash_table_remove(graph->nodes, node->name);
    free_linked_list(node->neighbors, 0);
    free(node->name);
    free(node);
}

void free_graph(Graph *graph) {
    if (!graph) return;

    HashTableIterator it = hash_table_iterator(graph->nodes);
    while (hash_table_iterator_has_next(&it)) {
        GraphNode *node = hash_table_iterator_next_value(&it);
        free_linked_list(node->neighbors, 0);
        free(node->name);
        free(node);
    }

    LinkedListNode *e = graph->edges->head;
    while (e) {
        free(e->data);
        e = e->next;
    }

    free_linked_list(graph->edges, 0);
    free_hash_table(graph->nodes);
    free(graph);
}
