#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "consts.h"
void menu()
{
    printf("1 - Ввести граф из файла\n");
    printf("2 - Ввести граф вручную\n");
    printf("3 - Найти город с минимальной суммой расстояний\n");
    printf("4 - Экспортировать граф в DOT формат\n");
    printf("5 - Показать информацию о графе\n");
    printf("0 - Выход\n");
    printf("Выбор: ");
}

int main()
{
    FILE *f;
    Graph *graph = NULL;
    char filename[256];
    int action, rc = OK;
    bool is_end = false;
    clock_t beg, end;
    double time_used;
    while (!is_end)
    {
        menu();
        
        if (scanf("%d", &action) != 1)
        {
            printf("Ошибка: Введите число.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        
        rc = OK;  
        
        switch(action)
        {
            case 1:
                if (graph)
                {
                    graph_destroy(graph);
                    graph = NULL;
                }
                printf("Введите имя файла: ");
                if (scanf("%255s", filename) != 1)
                {
                    printf("Ошибка чтения имени файла\n");
                    break;
                }
                while (getchar() != '\n');

                f = fopen(filename, "r");
                if (!f)
                {
                    printf("Ошибка открытия файла %s!\n", filename);
                    break;
                }

                rc = graph_read(f, &graph);
                if (rc == OK)
                {
                    printf("Граф загружен из файла\n");
                    graph_print_info(graph);
                }
                else if (rc == ERR_IO)
                {
                    printf("Ошибка чтения графа\n");
                }
                else
                {
                    printf("Ошибка памяти при чтении графа\n");
                }

                fclose(f);
                break;

            case 2:
                if (graph)
                {
                    graph_destroy(graph);
                    graph = NULL;
                }
                printf("Введите количество городов и дорог: ");
                
                rc = graph_read(stdin, &graph);
                if (rc == OK)
                {
                    printf("Граф создан\n");
                    graph_print_info(graph);
                }
                else if (rc == ERR_IO)
                {
                    printf("Ошибка ввода графа\n");
                }
                else
                {
                    printf("Ошибка памяти при создании графа\n");
                }
                break;

            case 3:
                if (!graph)
                {
                    printf("Ошибка: граф не загружен\n");
                    break;
                }
                
                printf("Введите имя файла для сохранения результата: ");
                if (scanf("%255s", filename) != 1)
                {
                    printf("Ошибка чтения имени файла\n");
                    break;
                }
                while (getchar() != '\n');
                
                f = fopen(filename, "w");
                if (!f)
                {
                    printf("Ошибка создания файла %s\n", filename);
                    break;
                }
                
                printf("\nПоиск города с минимальной суммой расстояний\n");
                printf("Тип графа: %s\n", graph->directed ? "ОРИЕНТИРОВАННЫЙ" : "НЕОРИЕНТИРОВАННЫЙ");
                printf("Алгоритм: ДЕЙКСТРА (запускается из каждой вершины)\n");
                
                int min_city, total_distance;
                
                
                print_shortest_paths_matrix(graph);
                
                beg = clock();
                rc = find_city_min_total_distance(graph, &min_city, &total_distance);
                end = clock();
                
                time_used = ((double)(end - beg)) / CLOCKS_PER_SEC;
                
                if (rc == OK)
                {
                    printf("\n=== РЕЗУЛЬТАТ ===\n");
                    printf("Оптимальный город: %d\n", min_city + 1);
                    printf("Сумма расстояний до всех остальных городов: %d\n", total_distance);
                    printf("Среднее расстояние до других городов: %.2f\n", 
                           (float)total_distance / (graph->size - 1));
                    
                    
                    int dist_from_min[MAX_VERTICES];
                    dijkstra(graph, min_city, dist_from_min);
                    
                    printf("\nРасстояния от города %d до других городов:\n", min_city + 1);
                    for (size_t i = 0; i < graph->size; i++)
                    {
                        if (i != (size_t)min_city)
                        {
                            printf("  До города %zu: %d\n", i + 1, dist_from_min[i]);
                        }
                    }
                    
                    
                    graph_print_min_city_dot(f, graph, min_city);
                    
                    printf("\nРезультат сохранен в %s\n", filename);
                    printf("Время выполнения: %.6f секунд\n", time_used);
                    
                    size_t mem_used = graph_memory_usage(graph);
                    printf("Использовано памяти: ~%zu байт\n", mem_used);
                    
                    printf("\nДля визуализации выполните команду:\n");
                    printf("  dot -Tpng %s -o result.png\n", filename);
                }
                else if (rc == ERR_IO)
                {
                    printf("Ошибка: граф не является связным или другая ошибка ввода\n");
                }
                else
                {
                    printf("Ошибка при поиске оптимального города\n");
                }

                fclose(f);
                break;

            case 4:
                if (!graph)
                {
                    printf("Граф не загружен\n");
                    break;
                }

                printf("Введите имя файла для экспорта: ");
                if (scanf("%255s", filename) != 1)
                {
                    printf("Ошибка чтения имени файла\n");
                    break;
                }
                while (getchar() != '\n');

                f = fopen(filename, "w");
                if (!f)
                {
                    printf("Ошибка создания файла %s!\n", filename);
                    break;
                }

                graph_print_dot(f, graph);
                printf("Граф экспортирован в %s\n", filename);
                printf("Для визуализации выполните: dot -Tpng %s -o graph.png\n", filename);
                
                fclose(f);
                break;

            case 5:
                if (!graph)
                {
                    printf("Граф не загружен!\n");
                    break;
                }

                graph_print_info(graph);
                break;

            case 0:
                is_end = true;
                break;

            default:
                printf("Неверный выбор! Попробуйте снова.\n");
                break;
        }
    }

    if (graph)
        graph_destroy(graph);
    
    return 0;  
}