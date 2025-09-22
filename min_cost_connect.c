#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Edge {
    int u, v;
    int weight;
} Edge;

typedef struct UnionFind {
    int* parent;
    int* rank;
} UnionFind;

UnionFind* createUnionFind(int n) {
    UnionFind* uf = (UnionFind*)malloc(sizeof(UnionFind));
    uf->parent = (int*)malloc(n * sizeof(int));
    uf->rank = (int*)calloc(n, sizeof(int));
    
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
    }
    
    return uf;
}

int find(UnionFind* uf, int x) {
    if (uf->parent[x] != x) {
        uf->parent[x] = find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

bool unionSets(UnionFind* uf, int x, int y) {
    int rootX = find(uf, x);
    int rootY = find(uf, y);
    
    if (rootX == rootY) {
        return false; 
    }
    
    if (uf->rank[rootX] < uf->rank[rootY]) {
        uf->parent[rootX] = rootY;
    } else if (uf->rank[rootX] > uf->rank[rootY]) {
        uf->parent[rootY] = rootX;
    } else {
        uf->parent[rootY] = rootX;
        uf->rank[rootX]++;
    }
    
    return true;
}

int manhattanDistance(int* point1, int* point2) {
    return abs(point1[0] - point2[0]) + abs(point1[1] - point2[1]);
}

int compareEdges(const void* a, const void* b) {
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    return edgeA->weight - edgeB->weight;
}

int minCostConnectPoints(int** points, int pointsSize, int* pointsColSize) {
    if (pointsSize <= 1) return 0;
    
    int numEdges = pointsSize * (pointsSize - 1) / 2;
    Edge* edges = (Edge*)malloc(numEdges * sizeof(Edge));
    
    int edgeIndex = 0;
    for (int i = 0; i < pointsSize; i++) {
        for (int j = i + 1; j < pointsSize; j++) {
            edges[edgeIndex].u = i;
            edges[edgeIndex].v = j;
            edges[edgeIndex].weight = manhattanDistance(points[i], points[j]);
            edgeIndex++;
        }
    }
    
    qsort(edges, numEdges, sizeof(Edge), compareEdges);
    
    UnionFind* uf = createUnionFind(pointsSize);
    int totalCost = 0;
    int edgesUsed = 0;
    
    for (int i = 0; i < numEdges && edgesUsed < pointsSize - 1; i++) {
        if (unionSets(uf, edges[i].u, edges[i].v)) {
            totalCost += edges[i].weight;
            edgesUsed++;
        }
    }
    
    free(edges);
    free(uf->parent);
    free(uf->rank);
    free(uf);
    
    return totalCost;
}