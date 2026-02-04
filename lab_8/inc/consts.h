#ifndef LIB_H__
#define LIB_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define OK 0
#define ERR_IO 1
#define ERR_MEM 2
#define ERR_FILE 3
#define MAX_VERTICES 100
#define INF 1000000

typedef struct Edge {
    int start;           
    int finish;          
    int cost;            
    struct Edge *next;   
} Edge;

typedef struct Node {
    int id;              
    Edge *edges;         
} Node;

typedef struct Graph {
    size_t size;         
    Node **nodes;        
    bool directed;       
} Graph;

Edge* edge_create(int start, int finish, int cost);
void edge_destroy(Edge *edge);
Node* node_create(int id);
void node_destroy(Node *node);
Graph* graph_create(size_t size, bool directed);
void graph_destroy(Graph *graph);

int graph_read(FILE *f, Graph **graph);
size_t graph_count_edges(Graph *graph);
bool graph_is_connected(Graph *graph);
size_t graph_memory_usage(Graph *graph);
bool graph_has_edge(Graph *graph, int from, int to, int *cost);

int find_city_min_total_distance(Graph *graph, int *city, int *total_distance);
void graph_print_dot(FILE *f, Graph *graph);
void graph_print_info(Graph *graph);
void graph_print_min_city_dot(FILE *f, Graph *graph, int min_city);
void dijkstra(Graph *graph, int start, int dist[]);
void all_pairs_dijkstra(Graph *graph, int dist[MAX_VERTICES][MAX_VERTICES]);
void print_shortest_paths_matrix(Graph *graph);

#endif