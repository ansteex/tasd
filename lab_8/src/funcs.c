#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "consts.h"

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

Edge* edge_create(int start, int finish, int cost)
{
    Edge *edge = malloc(sizeof(Edge));
    if (!edge)
        return NULL;
    
    edge->start = start;
    edge->finish = finish;
    edge->cost = cost;
    edge->next = NULL;
    
    return edge;
}

void edge_destroy(Edge *edge)
{
    free(edge);
}

Node* node_create(int id)
{
    Node *node = malloc(sizeof(Node));
    if (!node)
        return NULL;
    
    node->id = id;
    node->edges = NULL;
    
    return node;
}

void node_destroy(Node *node)
{
    Edge *edge = node->edges;
    while (edge)
    {
        Edge *next = edge->next;
        edge_destroy(edge);
        edge = next;
    }
    free(node);
}

void node_add_edge(Node *node, Edge *edge)
{
    if (!node || !edge)
        return;
    
    edge->next = node->edges;
    node->edges = edge;
}

Graph* graph_create(size_t size, bool directed)
{
    Graph *graph = malloc(sizeof(Graph));
    if (!graph)
        return NULL;
    
    graph->size = size;
    graph->directed = directed;
    graph->nodes = malloc(size * sizeof(Node*));
    
    if (!graph->nodes)
    {
        free(graph);
        return NULL;
    }
    
    for (size_t i = 0; i < size; i++)
    {
        graph->nodes[i] = node_create(i);
        if (!graph->nodes[i])
        {
            for (size_t j = 0; j < i; j++)
                node_destroy(graph->nodes[j]);
            free(graph->nodes);
            free(graph);
            return NULL;
        }
    }
    
    return graph;
}

void graph_destroy(Graph *graph)
{
    if (!graph)
        return;
    
    for (size_t i = 0; i < graph->size; i++)
        node_destroy(graph->nodes[i]);
    
    free(graph->nodes);
    free(graph);
}

size_t graph_count_edges(Graph *graph)
{
    if (!graph)
        return 0;
    
    size_t count = 0;
    for (size_t i = 0; i < graph->size; i++)
    {
        Edge *edge = graph->nodes[i]->edges;
        while (edge)
        {
            count++;
            edge = edge->next;
        }
    }
    
    return count;
}

size_t graph_memory_usage(Graph *graph)
{
    if (!graph)
        return 0;
    
    size_t usage = sizeof(Graph) + graph->size * sizeof(Node*);
    
    for (size_t i = 0; i < graph->size; i++)
    {
        usage += sizeof(Node);
        Edge *edge = graph->nodes[i]->edges;
        while (edge)
        {
            usage += sizeof(Edge);
            edge = edge->next;
        }
    }
    
    return usage;
}

bool graph_has_edge(Graph *graph, int from, int to, int *cost)
{
    if (!graph || from < 0 || to < 0 || (size_t)from >= graph->size || (size_t)to >= graph->size)
        return false;
    
    Edge *edge = graph->nodes[from]->edges;
    while (edge)
    {
        if (edge->finish == to)
        {
            if (cost)
                *cost = edge->cost;
            return true;
        }
        edge = edge->next;
    }
    
    return false;
}

void graph_print_info(Graph *graph)
{
    if (!graph)
        return;
    
    printf("\nИНФОРМАЦИЯ О ГРАФЕ\n");
    printf("Количество вершин: %zu\n", graph->size);
    printf("Количество ребер: %zu\n", graph_count_edges(graph));
    printf("Тип графа: %s\n", graph->directed ? "ОРИЕНТИРОВАННЫЙ" : "НЕОРИЕНТИРОВАННЫЙ");
    printf("Допускает разные длины в разные стороны: %s\n", graph->directed ? "ДА" : "НЕТ");
    
    if (graph_is_connected(graph))
    {
        printf("Граф: СВЯЗНЫЙ\n");
    }
    else
    {
        printf("Граф: НЕСВЯЗНЫЙ \n");
    }
    
    printf("\nИспользуемая память: ~%zu байт\n", graph_memory_usage(graph));
    printf("Структура: Список смежности\n");
}

static void dfs_visit(Graph *graph, int vertex, bool visited[])
{
    visited[vertex] = true;
    
    Edge *edge = graph->nodes[vertex]->edges;
    while (edge)
    {
        if (!visited[edge->finish])
            dfs_visit(graph, edge->finish, visited);
        edge = edge->next;
    }
}

bool graph_is_connected(Graph *graph)
{
    if (!graph || graph->size == 0)
        return false;
    if (!graph->directed)
    {
        bool *visited = calloc(graph->size, sizeof(bool));
        if (!visited)
            return false;
        
        dfs_visit(graph, 0, visited);
        
        bool connected = true;
        for (size_t i = 0; i < graph->size; i++)
        {
            if (!visited[i])
            {
                connected = false;
                break;
            }
        }
        
        free(visited);
        return connected;
    }
    else
    {      
        bool *visited = malloc(graph->size * sizeof(bool));
        if (!visited)
            return false;
        
        
        for (size_t start = 0; start < graph->size; start++)
        {
            
            for (size_t i = 0; i < graph->size; i++)
                visited[i] = false;
            
            
            dfs_visit(graph, start, visited);
            
            
            for (size_t i = 0; i < graph->size; i++)
            {
                if (!visited[i])
                {
                    free(visited);
                    return false;  
                }
            }
        }
        
        free(visited);
        return true;
    }
}

int graph_read(FILE *f, Graph **graph)
{
    if (!f)
        return ERR_IO;
    
    size_t vertex_count, edge_count;
    
    if (f == stdin)
        printf("Введите количество вершин и ребер: ");
    
    if (fscanf(f, "%zu %zu", &vertex_count, &edge_count) != 2)
        return ERR_IO;
    
    if (vertex_count == 0)
    {
        printf("Количество вершин должно быть > 0\n");
        return ERR_IO;
    }
    
    if (vertex_count > MAX_VERTICES)
    {
        printf("Слишком много вершин! Максимум: %d\n", MAX_VERTICES);
        return ERR_IO;
    }
    
    
    Graph *tmp = graph_create(vertex_count, true);
    if (!tmp)
        return ERR_MEM;
    
    if (f == stdin)
    {
        printf("\nВведите ребра (формат: город1 город2 расстояние)\n");
        printf("Нумерация с 1, граф ОРИЕНТИРОВАННЫЙ\n");
        printf("Для двусторонней дороги введите два ребра в обоих направлениях\n");
        printf("Пример: 1 4 25  и  4 1 2\n");
    }
    
    int rc = OK;
    int start, finish, weight;
    
    for (size_t i = 0; i < edge_count && rc == OK; i++)
    {
        if (f == stdin)
            printf("Ребро %zu (ориентированное): ", i + 1);
        
        if (fscanf(f, "%d %d %d", &start, &finish, &weight) != 3)
        {
            rc = ERR_IO;
            break;
        }
        
        if (start < 1 || (size_t)start > vertex_count ||
            finish < 1 || (size_t)finish > vertex_count)
        {
            if (f == stdin)
                printf("Ошибка: номера городов должны быть от 1 до %zu\n", vertex_count);
            rc = ERR_IO;
            break;
        }
        
        if (start == finish)
        {
            if (f == stdin)
                printf("Ошибка: петли не допускаются (город не может быть связан сам с собой)\n");
            rc = ERR_IO;
            break;
        }
        
        if (weight <= 0)
        {
            if (f == stdin)
                printf("Ошибка: расстояние должно быть > 0\n");
            rc = ERR_IO;
            break;
        }
        
        start--;
        finish--;
        
        
        Edge *edge = edge_create(start, finish, weight);
        if (!edge)
        {
            rc = ERR_MEM;
            break;
        }
        node_add_edge(tmp->nodes[start], edge);
        
        if (f == stdin)
            printf("  Добавлена дорога: %d -> %d (расстояние: %d)\n", 
                   start+1, finish+1, weight);
    }
    
    if (f == stdin)
        clear_input_buffer();
    
    if (rc == OK)
    {
        *graph = tmp;
        return OK;
    }
    else
    {
        graph_destroy(tmp);
        return rc;
    }
}


void dijkstra(Graph *graph, int start, int *dist)
{
    bool visited[MAX_VERTICES] = {false};
    
    
    for (size_t i = 0; i < graph->size; i++)
        dist[i] = INF;
    dist[start] = 0;
    
    
    for (size_t count = 0; count < graph->size; count++)
    {
        
        int min_dist = INF;
        int min_vertex = -1;
        
        for (size_t v = 0; v < graph->size; v++)
        {
            if (!visited[v] && dist[v] < min_dist)
            {
                min_dist = dist[v];
                min_vertex = v;
            }
        }
        
        
        if (min_vertex == -1)
            break;
        
        visited[min_vertex] = true;
        
        
        Edge *edge = graph->nodes[min_vertex]->edges;
        while (edge)
        {
            int neighbor = edge->finish;
            int new_dist = dist[min_vertex] + edge->cost;
            
            if (!visited[neighbor] && new_dist < dist[neighbor])
            {
                dist[neighbor] = new_dist;
            }
            edge = edge->next;
        }
    }
}


void all_pairs_dijkstra(Graph *graph, int dist[MAX_VERTICES][MAX_VERTICES])
{
    for (size_t i = 0; i < graph->size; i++)
    {
        dijkstra(graph, i, dist[i]);
    }
}


int find_city_min_total_distance(Graph *graph, int *city, int *total_distance)
{
    if (!graph || !city || !total_distance)
        return ERR_IO;
    
    if (!graph_is_connected(graph))
    {
        printf("Внимание: граф не является связным!\n");
        printf("По условию задачи граф должен быть связным.\n");
    }
    
    int dist[MAX_VERTICES][MAX_VERTICES];
    
    
    all_pairs_dijkstra(graph, dist);
    
    int min_sum = INF * graph->size;
    int min_city = -1;
    
    for (size_t i = 0; i < graph->size; i++)
    {
        int sum = 0;
        bool reachable = true;
        
        for (size_t j = 0; j < graph->size; j++)
        {
            if (i != j)
            {
                if (dist[i][j] >= INF)
                {
                    reachable = false;
                    break;
                }
                sum += dist[i][j];
            }
        }
        
        if (reachable && sum < min_sum)
        {
            min_sum = sum;
            min_city = i;
        }
    }
    
    if (min_city == -1)
    {
        printf("Не удалось найти город с минимальной суммой расстояний.\n");
        printf("Возможно, граф не является связным.\n");
        return ERR_IO;
    }
    
    *city = min_city;
    *total_distance = min_sum;
    
    return OK;
}


void print_shortest_paths_matrix(Graph *graph)
{
    if (!graph)
        return;
    
    int dist[MAX_VERTICES][MAX_VERTICES];
    
    
    all_pairs_dijkstra(graph, dist);
    
    printf("\n=== МАТРИЦА КРАТЧАЙШИХ РАССТОЯНИЙ (алгоритм Дейкстры) ===\n\n");
    
    
    printf("      ");
    for (size_t i = 0; i < graph->size; i++)
        printf("%4zu  ", i + 1);
    printf("\n");
    
    printf("     ");
    for (size_t i = 0; i < graph->size; i++)
        printf("------");
    printf("\n");
    
    
    for (size_t i = 0; i < graph->size; i++)
    {
        printf("%4zu |", i + 1);
        for (size_t j = 0; j < graph->size; j++)
        {
            if (i == j)
                printf("    0 ");
            else if (dist[i][j] >= INF)
                printf("   ∞ ");
            else
                printf("%5d ", dist[i][j]);
        }
        printf("\n");
    }
    
    
    printf("\n=== СУММА РАССТОЯНИЙ ДО ДРУГИХ ГОРОДОВ ===\n\n");
    for (size_t i = 0; i < graph->size; i++)
    {
        int sum = 0;
        bool reachable = true;
        
        for (size_t j = 0; j < graph->size; j++)
        {
            if (i != j)
            {
                if (dist[i][j] >= INF)
                {
                    reachable = false;
                    break;
                }
                sum += dist[i][j];
            }
        }
        
        if (reachable)
            printf("Город %zu: сумма = %d, среднее = %.2f\n", 
                   i + 1, sum, (float)sum / (graph->size - 1));
        else
            printf("Город %zu: НЕСВЯЗАН со всеми городами\n", i + 1);
    }
}


void graph_print_min_city_dot(FILE *f, Graph *graph, int min_city)
{
    if (!graph || !f || min_city < 0 || (size_t)min_city >= graph->size)
        return;
    
    fprintf(f, "digraph G {\n");
    fprintf(f, "    label=\"Город с минимальной суммой расстояний\\n");
    fprintf(f, "Город %d: сумма расстояний до всех городов минимальна\\n", min_city + 1);
    fprintf(f, "Алгоритм: Дейкстра\";\n");
    fprintf(f, "    labelloc=\"t\";\n");
    fprintf(f, "    fontsize=14;\n");
    fprintf(f, "    node [shape=circle, style=filled];\n");
    fprintf(f, "    edge [fontsize=10];\n\n");
    
    
    int dist_from_min[MAX_VERTICES];
    dijkstra(graph, min_city, dist_from_min);
    
    
    for (size_t i = 0; i < graph->size; i++)
    {
        if (i == (size_t)min_city)
        {
            fprintf(f, "    %zu [label=\"Город %zu\\n(оптимальный)\", fillcolor=yellow, style=\"filled,bold\"];\n", 
                    i + 1, i + 1);
        }
        else
        {
            fprintf(f, "    %zu [label=\"Город %zu\\nрасст.:%d\"];\n", 
                    i + 1, i + 1, dist_from_min[i]);
        }
    }
    
    fprintf(f, "\n");
    
    
    for (size_t i = 0; i < graph->size; i++)
    {
        Edge *edge = graph->nodes[i]->edges;
        while (edge)
        {
            
            bool is_on_shortest_path = false;
            
            
            
            if (dist_from_min[i] < INF && edge->cost < INF)
            {
                if (dist_from_min[i] + edge->cost == dist_from_min[edge->finish])
                {
                    is_on_shortest_path = true;
                }
            }
            
            if (is_on_shortest_path)
            {
                fprintf(f, "    %zu -> %d [label=\"%d\", color=red, penwidth=2];\n", 
                        i + 1, edge->finish + 1, edge->cost);
            }
            else
            {
                fprintf(f, "    %zu -> %d [label=\"%d\", color=black];\n", 
                        i + 1, edge->finish + 1, edge->cost);
            }
            edge = edge->next;
        }
    }
    
    fprintf(f, "}\n");
}

void graph_print_dot(FILE *f, Graph *graph)
{
    if (!graph || !f)
        return;
    
    if (graph->directed)
    {
        fprintf(f, "digraph G {\n");
        fprintf(f, "    label=\"Ориентированный граф\\nВершин: %zu, Ребер: %zu\\n", 
                graph->size, graph_count_edges(graph));
        fprintf(f, "Допускает разные длины в разные стороны\";\n");
        fprintf(f, "    labelloc=\"t\";\n");
        fprintf(f, "    fontsize=16;\n");
        fprintf(f, "    node [shape=circle];\n\n");
        
        for (size_t i = 0; i < graph->size; i++)
        {
            fprintf(f, "    %zu;\n", i + 1);
        }
        
        fprintf(f, "\n");
        
        for (size_t i = 0; i < graph->size; i++)
        {
            Edge *edge = graph->nodes[i]->edges;
            while (edge)
            {
                fprintf(f, "    %zu -> %d [label=\"%d\"];\n", 
                        i + 1, edge->finish + 1, edge->cost);
                edge = edge->next;
            }
        }
    }
    else
    {
        fprintf(f, "graph G {\n");
        fprintf(f, "    label=\"Неориентированный граф\\nВершин: %zu, Ребер: %zu\";\n", 
                graph->size, graph_count_edges(graph));
        fprintf(f, "    labelloc=\"t\";\n");
        fprintf(f, "    fontsize=16;\n");
        fprintf(f, "    node [shape=circle];\n");
        fprintf(f, "    edge [dir=none];\n\n");
        
        for (size_t i = 0; i < graph->size; i++)
        {
            fprintf(f, "    %zu;\n", i + 1);
        }
        
        fprintf(f, "\n");
        
        bool **printed = malloc(graph->size * sizeof(bool*));
        for (size_t i = 0; i < graph->size; i++)
            printed[i] = calloc(graph->size, sizeof(bool));
        
        for (size_t i = 0; i < graph->size; i++)
        {
            Edge *edge = graph->nodes[i]->edges;
            while (edge)
            {
                if (i < (size_t)edge->finish && !printed[i][edge->finish])
                {
                    fprintf(f, "    %zu -- %d [label=\"%d\"];\n", 
                            i + 1, edge->finish + 1, edge->cost);
                    printed[i][edge->finish] = true;
                }
                edge = edge->next;
            }
        }
        
        for (size_t i = 0; i < graph->size; i++)
            free(printed[i]);
        free(printed);
    }
    
    fprintf(f, "}\n");
}