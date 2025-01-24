// Ethan Johnson
// gcc lab5.c -std=c99

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 25

typedef struct edge {
    int tail, head, weight;
} edgeType;

typedef struct {
    char **keys;
    int *values;
    bool *used;
    int size;
    int count;
} HashTable;

int numVertices, numEdges, numTrees;
int *parent, *rank;
char **vertexNames;
edgeType *edgeTab;

HashTable* createHashTable(int size) {
    HashTable *ht = malloc(sizeof(HashTable));
    size = size*2+1;
    ht->size = size;
    ht->count = 0;
    ht->keys = calloc(size, sizeof(char*));
    ht->values = calloc(size, sizeof(int));
    ht->used = calloc(size, sizeof(bool));
    return ht;
}

char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}

int hash(const char *key, int size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash % size;
}

int hashInsert(HashTable *ht, const char *key) {
    int index = hash(key, ht->size);
    int originalIndex = index;
    
    while (ht->used[index]) {
        if (strcmp(ht->keys[index], key) == 0)
            return ht->values[index];
            
        index = (index + 1) % ht->size;
        if (index == originalIndex)
            return -1;
    }
    
    ht->keys[index] = strdup(key);
    ht->values[index] = ht->count;
    ht->used[index] = true;
    vertexNames[ht->count] = strdup(key);
    return ht->count++;
}

void initializeDisjointSets() {
    parent = malloc(numVertices * sizeof(int));
    rank = calloc(numVertices, sizeof(int));
    for (int i = 0; i < numVertices; i++)
        parent[i] = i;
}

int find(int x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]);
    return parent[x];
}

void unionSets(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return;
    
    if (rank[rx] < rank[ry])
        parent[rx] = ry;
    else if (rank[rx] > rank[ry])
        parent[ry] = rx;
    else {
        parent[ry] = rx;
        rank[rx]++;
    }
    numTrees--;
}

int compareEdges(const void *a, const void *b) {
    return ((edgeType*)a)->weight - ((edgeType*)b)->weight;
}

int main() {
    scanf("%d %d", &numVertices, &numEdges);
    
    edgeTab = malloc(numEdges * sizeof(edgeType));
    vertexNames = malloc(numVertices * sizeof(char*));
    HashTable *ht = createHashTable(numEdges);
    
    char tail[MAX_NAME_LENGTH], head[MAX_NAME_LENGTH];
    int weight;
    
    for (int i = 0; i < numEdges; i++) {
        scanf("%s %s %d", tail, head, &weight);
        int tailIdx = hashInsert(ht, tail);
        int headIdx = hashInsert(ht, head);
        edgeTab[i] = (edgeType){tailIdx, headIdx, weight};
    }
    
    numVertices = ht->count;  // Update with actual number of vertices
    numTrees = numVertices;
    
    qsort(edgeTab, numEdges, sizeof(edgeType), compareEdges);
    
    initializeDisjointSets();
    
    edgeType *mstEdges = malloc(numVertices * sizeof(edgeType));
    int mstEdgeCount = 0;
    int totalWeight = 0;
    
    for (int i = 0; i < numEdges; i++) {
        if (find(edgeTab[i].tail) != find(edgeTab[i].head)) {
            unionSets(edgeTab[i].tail, edgeTab[i].head);
            mstEdges[mstEdgeCount++] = edgeTab[i];
            totalWeight += edgeTab[i].weight;
        }
    }
    
    printf("Sum of weights of spanning edges %d\n", totalWeight);
    
    // Find all tree leaders
    bool *isLeader = calloc(numVertices, sizeof(bool));
    int *vertexToLeader = malloc(numVertices * sizeof(int));
    
    for (int i = 0; i < numVertices; i++) {
        vertexToLeader[i] = find(i);
        isLeader[vertexToLeader[i]] = true;
    }
    
    // Print each tree
    for (int i = 0; i < numVertices; i++) {
        if (isLeader[i]) {
            printf("Outputting tree with leader %d\n", i);
            for (int j = 0; j < mstEdgeCount; j++) {
                if (vertexToLeader[mstEdges[j].tail] == i) {
                    printf("%s %s %d\n", 
                           vertexNames[mstEdges[j].tail],
                           vertexNames[mstEdges[j].head],
                           mstEdges[j].weight);
                }
            }
        }
    }
    
    // Cleanup
    free(edgeTab);
    free(parent);
    free(rank);
    free(mstEdges);
    free(isLeader);
    free(vertexToLeader);
    
    for (int i = 0; i < ht->size; i++)
        if (ht->used[i])
            free(ht->keys[i]);
    
    free(ht->keys);
    free(ht->values);
    free(ht->used);
    free(ht);
    
    for (int i = 0; i < numVertices; i++)
        free(vertexNames[i]);
    free(vertexNames);
    
    return 0;
}