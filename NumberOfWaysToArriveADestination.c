#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MOD 1000000007

typedef struct AdjNode {
    int vertex;
    long long weight;
    struct AdjNode* next;
} AdjNode;

typedef struct HeapNode {
    int vertex;
    long long distance;
} HeapNode;

typedef struct MinHeap {
    HeapNode* nodes;
    int size;
    int capacity;
} MinHeap;

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

void insertHeap(MinHeap* heap, int vertex, long long distance) {
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

int countPaths(int n, int** roads, int roadsSize, int* roadsColSize) {
    AdjNode** adjList = (AdjNode**)calloc(n, sizeof(AdjNode*));
    
    for (int i = 0; i < roadsSize; i++) {
        int u = roads[i][0];
        int v = roads[i][1];
        long long time = roads[i][2];
        
        AdjNode* newEdge = (AdjNode*)malloc(sizeof(AdjNode));
        newEdge->vertex = v;
        newEdge->weight = time;
        newEdge->next = adjList[u];
        adjList[u] = newEdge;
        
        newEdge = (AdjNode*)malloc(sizeof(AdjNode));
        newEdge->vertex = u;
        newEdge->weight = time;
        newEdge->next = adjList[v];
        adjList[v] = newEdge;
    }
    
    long long* dist = (long long*)malloc(n * sizeof(long long));
    long long* ways = (long long*)malloc(n * sizeof(long long));
    bool* visited = (bool*)calloc(n, sizeof(bool));
    
    for (int i = 0; i < n; i++) {
        dist[i] = LLONG_MAX;
        ways[i] = 0;
    }
    
    dist[0] = 0;
    ways[0] = 1;
    
    MinHeap* heap = createMinHeap(n * n);
    insertHeap(heap, 0, 0);
    
    while (!isEmpty(heap)) {
        HeapNode current = extractMin(heap);
        int u = current.vertex;
        long long currentDist = current.distance;
        
        if (visited[u]) continue;
        visited[u] = true;
        
        AdjNode* edge = adjList[u];
        while (edge != NULL) {
            int v = edge->vertex;
            long long newDist = currentDist + edge->weight;
            
            if (newDist < dist[v]) {
                dist[v] = newDist;
                ways[v] = ways[u];
                insertHeap(heap, v, newDist);
            } else if (newDist == dist[v]) {
                ways[v] = (ways[v] + ways[u]) % MOD;
            }
            
            edge = edge->next;
        }
    }
    
    int result = (int)(ways[n - 1] % MOD);
    
    for (int i = 0; i < n; i++) {
        AdjNode* current = adjList[i];
        while (current != NULL) {
            AdjNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(adjList);
    free(dist);
    free(ways);
    free(visited);
    free(heap->nodes);
    free(heap);
    
    return result;
}

