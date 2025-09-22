#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX

typedef struct AdjNode {
    int vertex;
    int weight;
    struct AdjNode* next;
} AdjNode;

typedef struct HeapNode {
    int vertex;
    int distance;
} HeapNode;

typedef struct MinHeap {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

typedef struct Graph {
    int n;              
    AdjNode** adj;      
} Graph;

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->nodes = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int index) {
    int parent = (index - 1) / 2;
    if (parent >= 0 && heap->nodes[parent].distance > heap->nodes[index].distance) {
        swap(&heap->nodes[parent], &heap->nodes[index]);
        heapifyUp(heap, parent);
    }
}

void heapifyDown(MinHeap* heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    
    if (left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance)
        smallest = left;
    
    if (right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance)
        smallest = right;
    
    if (smallest != index) {
        swap(&heap->nodes[smallest], &heap->nodes[index]);
        heapifyDown(heap, smallest);
    }
}

void insertHeap(MinHeap* heap, int vertex, int distance) {
    if (heap->size >= heap->capacity) return;
    
    heap->nodes[heap->size].vertex = vertex;
    heap->nodes[heap->size].distance = distance;
    heapifyUp(heap, heap->size);
    heap->size++;
}

HeapNode extractMin(MinHeap* heap) {
    HeapNode min = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return min;
}

bool isEmpty(MinHeap* heap) {
    return heap->size == 0;
}

Graph* graphCreate(int n, int** edges, int edgesSize, int* edgesColSize) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->n = n;
    graph->adj = (AdjNode**)calloc(n, sizeof(AdjNode*));
    
    for (int i = 0; i < edgesSize; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];
        
        AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
        newNode->vertex = v;
        newNode->weight = w;
        newNode->next = graph->adj[u];
        graph->adj[u] = newNode;
    }
    
    return graph;
}

void graphAddEdge(Graph* graph, int* edge, int edgeSize) {
    int u = edge[0];
    int v = edge[1];
    int w = edge[2];
    
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->vertex = v;
    newNode->weight = w;
    newNode->next = graph->adj[u];
    graph->adj[u] = newNode;
}

int graphShortestPath(Graph* graph, int node1, int node2) {
    int n = graph->n;
    int* dist = (int*)malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
    }
    dist[node1] = 0;
    
    MinHeap* heap = createMinHeap(n * n);
    insertHeap(heap, node1, 0);
    
    while (!isEmpty(heap)) {
        HeapNode current = extractMin(heap);
        int u = current.vertex;
        int d = current.distance;
        
        if (d > dist[u]) continue;
        if (u == node2) {
            int result = d;
            free(dist);
            free(heap->nodes);
            free(heap);
            return result;
        }
        
        AdjNode* adj = graph->adj[u];
        while (adj != NULL) {
            int v = adj->vertex;
            int w = adj->weight;
            int newDist = d + w;
            
            if (newDist < dist[v]) {
                dist[v] = newDist;
                insertHeap(heap, v, newDist);
            }
            adj = adj->next;
        }
    }
    
    free(dist);
    free(heap->nodes);
    free(heap);
    return -1;
}

void graphFree(Graph* graph) {
    for (int i = 0; i < graph->n; i++) {
        AdjNode* current = graph->adj[i];
        while (current != NULL) {
            AdjNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adj);
    free(graph);
}
