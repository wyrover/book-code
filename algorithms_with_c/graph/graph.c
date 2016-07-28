#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"list.h"
#include"set.h"

void graph_init(Graph *graph, int (*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
    graph->vcount = 0;
    graph->ecount = 0;
    graph->match = match;
    graph->destroy = destroy;
    list_init(&graph->adjlists, NULL);
    return ;
}

void graph_destroy(Graph *graph)
{
    AdjList *adjlist;

    while (list_size(&graph->adjlists) > 0) {
        if (list_rem_next(&graph->adjlists, NULL, (void **)&adjlist) == 0) {
            set_destroy(&adjlist->adjacent);

            if (graph->destroy != NULL)
                graph->destroy(adjlist->vertex);

            free(adjlist);
        }
    }

    list_destroy(&graph->adjlists);
    memset(graph, 0, sizeof(Graph));
    return ;
}

int graph_ins_vertex(Graph *graph, const void *data)
{
    ListElmt *element;
    AdjList *adjlist;
    int retval;

    for (element = list_head(&graph->adjlists); element != NULL; element = list_next(element)) {
        if (graph->match(data, ((AdjList *)list_data(element))->vertex))
            return 1;
    }

    if ((adjlist = (AdjList *)malloc(sizeof(AdjList))) == NULL)
        return -1;

    adjlist->vertex = (void *)data;
    set_init(&adjlist->adjacent, graph->match, NULL);

    if ((retval = list_ins_next(&graph->adjlists, list_tail(&graph->adjlists), adjlist)) != 0) {
        return retval;
    }

    graph->vcount++;
    return 0;
}

int graph_ins_edge(Graph *graph, const void *data1, const void *data2)
{
    ListElmt *element;
    int retval;

    for (element = list_head(&graph->adjlists); element != NULL; element = list_next(element)) {
        if (graph->match(data2, ((AdjList *)list_data(element))->vertex))
            break;
    }

    if (element == NULL)
        return -1;

    for (element = list_head(&graph->adjlists); element != NULL; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex))
            break;
    }

    if (element == NULL)
        return -1;

    if ((retval = set_insert(&((AdjList *)list_data(element))->adjacent, data2)) != 0) {
        return retval;
    }

    graph->ecount++;
    return 0;
}

int graph_rem_vertex(Graph *graph, void **data)
{
    ListElmt *element, *temp, *prev;
    AdjList *adjlist;
    int found;
    prev = NULL;
    found = 0;

    for (element = list_head(&graph->adjlists); element != NULL; element = list_next(element)) {
        if (set_is_member(&((AdjList *)list_data(element))->adjacent, *data))
            return -1;

        if (graph->match(*data, ((AdjList *)list_data(element))->vertex)) {
            temp = element;
            found = 1;
        }

        if (!found)
            prev = element;
    }

    if (!found)
        return -1;

    if (set_size(&((AdjList *)list_data(temp))->adjacent) > 0)
        return -1;

    if (list_rem_next(&graph->adjlists, prev, (void **)&adjlist) != 0)
        return -1;

    *data = adjlist->vertex;
    free(adjlist);
    graph->vcount--;
    return 0;
}

int graph_rem_edge(Graph *graph, void *data1, void **data2)
{
    ListElmt *element;

    for (element = list_head(&graph->adjlists); element != NULL; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex))
            break;
    }

    if (element == NULL)
        return -1;

    if (set_remove(&((AdjList *)list_data(element))->adjacent, data2) != 0)
        return -1;

    graph->ecount--;
    return 0;
}

int graph_adjlist(const Graph *graph, const void *data, AdjList **adjlist)
{
    ListElmt *element, *prev;
    prev = NULL;

    for (element = list_head(&graph->adjlists); element != NULL; element = list_next(element)) {
        if (graph->match(data, ((AdjList *)list_data(element))->vertex))
            break;

        prev = element;
    }

    if (element == NULL)
        return -1;

    *adjlist = list_data(element);
    return 0;
}

int graph_is_adjacent(const Graph *graph, const void *data1, const void *data2)
{
    ListElmt *element, *prev;
    prev = NULL;

    for (element = list_head(&graph->adjlists); element != NULL; element = list_next(element)) {
        if (graph->match(data1, ((AdjList *)list_data(element))->vertex))
            break;

        prev = element;
    }

    if (element == NULL)
        return 0;

    return set_is_member(&((AdjList *)list_data(element))->adjacent, data2);
}
