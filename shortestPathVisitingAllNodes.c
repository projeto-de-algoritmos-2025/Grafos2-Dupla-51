#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXN 12
#define MAXMASK (1 << MAXN)

typedef struct {
    int node;
    int mask;
} State;

typedef struct {
    State *arr;
    int front, rear, size, capacity;
} Queue;

Queue* createQueue(int capacity) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->capacity = capacity;
    q->front = q->size = 0;
    q->rear = capacity - 1;
    q->arr = (State*)malloc(capacity * sizeof(State));
    return q;
}
bool isEmpty(Queue* q) { return q->size == 0; }
void enqueue(Queue* q, State s) {
    q->rear = (q->rear + 1) % q->capacity;
    q->arr[q->rear] = s;
    q->size++;
}
State dequeue(Queue* q) {
    State s = q->arr[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return s;
}

int shortestPathLength(int** graph, int graphSize, int* graphColSize) {
    if (graphSize == 1) return 0;

    int goal = (1 << graphSize) - 1;

    bool visited[MAXN][MAXMASK] = {false};

    Queue* q = createQueue(graphSize * MAXMASK);
    for (int i = 0; i < graphSize; i++) {
        int mask = 1 << i;
        State s = {i, mask};
        enqueue(q, s);
        visited[i][mask] = true;
    }

    int steps = 0;
    while (!isEmpty(q)) {
        int sz = q->size;
        for (int k = 0; k < sz; k++) {
            State cur = dequeue(q);
            int u = cur.node, mask = cur.mask;

            if (mask == goal) return steps;

            for (int j = 0; j < graphColSize[u]; j++) {
                int v = graph[u][j];
                int newMask = mask | (1 << v);
                if (!visited[v][newMask]) {
                    visited[v][newMask] = true;
                    State next = {v, newMask};
                    enqueue(q, next);
                }
            }
        }
        steps++;
    }
    return -1; 
}