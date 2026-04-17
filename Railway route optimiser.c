/*
 * ============================================================
 *  RAILWAY ROUTE OPTIMIZATION SYSTEM
 *  Data Structures Mini-Project | First Year Engineering
 * ============================================================
 *  Data Structures Used:
 *   - Graph       : Adjacency List (Array of Linked Lists)
 *   - Linked List : For each station's neighbor list
 *   - Queue       : For BFS (minimum stops)
 *   - Stack       : For path reconstruction
 *   - Min-Heap    : For Dijkstra's priority queue
 *   - Arrays      : For distance/visited/parent tables
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
int i;

#define MAX_STATIONS   50
#define INF            INT_MAX
#define DATA_FILE      "railway_data.txt"

   /* STRUCTURES */

/* Adjacency list node ||| represents one neighbor */
typedef struct AdjNode {
    int   dest;          /* Index of destination station */
    int   weight;        /* Distance in km                */
    struct AdjNode *next;
} AdjNode;

/* A single railway station */
typedef struct {
    char     name[50];   /* Station name                  */
    int      active;     /* 1 = exists, 0 = deleted       */
    AdjNode *head;       /* Head of its neighbor list     */
} Station;

/* The graph – holds all stations */
typedef struct {
    Station stations[MAX_STATIONS];
    int     count;       /* Total stations (including deleted slots) */
} Graph;

/* Min-Heap node for Dijkstra */
typedef struct {
    int vertex;
    int dist;
} HeapNode;

/* Min-Heap */
typedef struct {
    HeapNode data[MAX_STATIONS];
    int      size;
} MinHeap;

/* Simple stack for path reconstruction */
typedef struct {
    int  items[MAX_STATIONS];
    int  top;
} Stack;

/* Simple queue for BFS */
typedef struct {
    int  items[MAX_STATIONS];
    int  front, rear;
} Queue;

Graph g;

void stackInit(Stack *s)           { s->top = -1; }
int  stackEmpty(Stack *s)          { return s->top == -1; }
void stackPush(Stack *s, int v)    { s->items[++(s->top)] = v; }
int  stackPop(Stack *s)            { return s->items[(s->top)--]; }

void queueInit(Queue *q)           { q->front = 0; q->rear = 0; }
int  queueEmpty(Queue *q)          { return q->front == q->rear; }
void enqueue(Queue *q, int v)      { q->items[q->rear++] = v; }
int  dequeue(Queue *q)             { return q->items[q->front++]; }

void heapInit(MinHeap *h)          { h->size = 0; }

/* Swap two heap nodes */
void heapSwap(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a; *a = *b; *b = tmp;
}

/* Bubble up to maintain min-heap property */
void heapPush(MinHeap *h, int vertex, int dist) {
    int i = h->size++;
    h->data[i].vertex = vertex;
    h->data[i].dist   = dist;
    /* Sift up */
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->data[parent].dist > h->data[i].dist) {
            heapSwap(&h->data[parent], &h->data[i]);
            i = parent;
        } else break;
    }
}

/* Extract minimum element */
HeapNode heapPop(MinHeap *h) {
    HeapNode top = h->data[0];
    h->data[0]   = h->data[--(h->size)];
    /* Sift down */
    int i = 0;
    while (1) {
        int l = 2*i+1, r = 2*i+2, smallest = i;
        if (l < h->size && h->data[l].dist < h->data[smallest].dist) smallest = l;
        if (r < h->size && h->data[r].dist < h->data[smallest].dist) smallest = r;
        if (smallest == i) break;
        heapSwap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return top;
}

void initGraph() {
    g.count = 0;
    for ( i = 0; i < MAX_STATIONS; i++) {
        g.stations[i].active = 0;
        g.stations[i].head   = NULL;
    }
}

int findStation(const char *name) {
    for ( i = 0; i < g.count; i++)
        if (g.stations[i].active && strcmp(g.stations[i].name, name) == 0)
            return i;
    return -1;
}

void addStation() {
    if (g.count >= MAX_STATIONS) {
        printf("[!] Maximum station limit reached.\n");
        return;
    }
    char name[50];
    printf("Enter station name: ");
    scanf(" %49[^\n]", name);

    if (findStation(name) != -1) {
        printf("[!] Station '%s' already exists.\n", name);
        return;
    }
    strcpy(g.stations[g.count].name, name);
    g.stations[g.count].active = 1;
    g.stations[g.count].head   = NULL;
    printf(" Station '%s' added at index %d.\n", name, g.count);
    g.count++;
}

void addRoute() {
    char src[50], dst[50];
    int  weight;

    printf("Enter source station: ");      scanf(" %49[^\n]", src);
    printf("Enter destination station: "); scanf(" %49[^\n]", dst);
    printf("Enter distance (km): ");       scanf("%d", &weight);

    int u = findStation(src);
    int v = findStation(dst);

    if (u == -1) { printf("[!] Station '%s' not found.\n", src); return; }
    if (v == -1) { printf("[!] Station '%s' not found.\n", dst); return; }
    if (weight <= 0) { printf("[!] Distance must be positive.\n"); return; }

    /* Add u -> v */
    AdjNode *n1 = (AdjNode *)malloc(sizeof(AdjNode));
    n1->dest = v; n1->weight = weight; n1->next = g.stations[u].head;
    g.stations[u].head = n1;

    /* Add v -> u (undirected) */
    AdjNode *n2 = (AdjNode *)malloc(sizeof(AdjNode));
    n2->dest = u; n2->weight = weight; n2->next = g.stations[v].head;
    g.stations[v].head = n2;

    printf(" Route added: %s <--> %s (%d km)\n", src, dst, weight);
}


void deleteStation() {
    char name[50];
    printf("Enter station name to delete: ");
    scanf(" %49[^\n]", name);
    int idx = findStation(name);
    if (idx == -1) { printf("[!] Station not found.\n"); return; }

    /* Free its adjacency list */
    AdjNode *cur = g.stations[idx].head;
    while (cur) {
        AdjNode *tmp = cur->next;
        /* Remove reverse edges from neighbors */
        AdjNode *prev = NULL, *nb = g.stations[cur->dest].head;
        while (nb) {
            if (nb->dest == idx) {
                if (prev) prev->next = nb->next;
                else g.stations[cur->dest].head = nb->next;
                free(nb); break;
            }
            prev = nb; nb = nb->next;
        }
        free(cur); cur = tmp;
    }
    g.stations[idx].head   = NULL;
    g.stations[idx].active = 0;
    printf(" Station '%s' deleted.\n", name);
}

void deleteRoute() {
    char src[50], dst[50];
    printf("Enter source station: ");      scanf(" %49[^\n]", src);
    printf("Enter destination station: "); scanf(" %49[^\n]", dst);

    int u = findStation(src), v = findStation(dst);
    if (u == -1 || v == -1) { printf("! One or both stations not found.\n"); return; }

    /* Remove u->v */
    AdjNode *prev = NULL, *cur = g.stations[u].head;
    int found = 0;
    while (cur) {
        if (cur->dest == v) {
            if (prev) prev->next = cur->next;
            else g.stations[u].head = cur->next;
            free(cur); found = 1; break;
        }
        prev = cur; cur = cur->next;
    }
    /* Remove v->u */
    prev = NULL; cur = g.stations[v].head;
    while (cur) {
        if (cur->dest == u) {
            if (prev) prev->next = cur->next;
            else g.stations[v].head = cur->next;
            free(cur); break;
        }
        prev = cur; cur = cur->next;
    }
    if (found) printf(" Route between '%s' and '%s' deleted.\n", src, dst);
    else       printf("! Route not found.\n");
}

void displayNetwork() {
    printf("\n========= RAILWAY NETWORK =========\n");
    int any = 0;
    for ( i = 0; i < g.count; i++) {
        if (!g.stations[i].active) continue;
        any = 1;
        printf("  [%d] %s\n", i, g.stations[i].name);
        AdjNode *cur = g.stations[i].head;
        if (!cur) { printf("       (no routes)\n"); continue; }
        while (cur) {
            printf("       --> %s (%d km)\n", g.stations[cur->dest].name, cur->weight);
            cur = cur->next;
        }
    }
    if (!any) printf("  (No stations added yet)\n");
    printf("====================================\n\n");
}

void printPath(int parent[], int src, int dest) {
    (void)src; /* src kept in signature for clarity; not used directly */
    Stack s;
    stackInit(&s);
    int cur = dest;
    while (cur != -1) { stackPush(&s, cur); cur = parent[cur]; }

    printf("  Path: ");
    while (!stackEmpty(&s)) {
        int node = stackPop(&s);
        printf("%s", g.stations[node].name);
        if (!stackEmpty(&s)) printf(" --> ");
    }
    printf("\n");
}

void dijkstra() {
    char src[50], dst[50];
    printf("Enter source station: ");      scanf(" %49[^\n]", src);
    printf("Enter destination station: "); scanf(" %49[^\n]", dst);

    int u = findStation(src), v = findStation(dst);
    if (u == -1) { printf("[!] Station '%s' not found.\n", src); return; }
    if (v == -1) { printf("[!] Station '%s' not found.\n", dst); return; }

    int dist[MAX_STATIONS];       /* Shortest distance array   */
    int parent[MAX_STATIONS];     /* To reconstruct path       */
    int visited[MAX_STATIONS];    /* Visited set               */

    for ( i = 0; i < g.count; i++) {
        dist[i]    = INF;
        parent[i]  = -1;
        visited[i] = 0;
    }
    dist[u] = 0;

    MinHeap heap;
    heapInit(&heap);
    heapPush(&heap, u, 0);

    while (heap.size > 0) {
        HeapNode top = heapPop(&heap);
        int node = top.vertex;

        if (visited[node]) continue;
        visited[node] = 1;

        AdjNode *cur = g.stations[node].head;
        while (cur) {
            int nb = cur->dest;
            if (!visited[nb] && dist[node] != INF &&
                dist[node] + cur->weight < dist[nb]) {
                dist[nb]   = dist[node] + cur->weight;
                parent[nb] = node;
                heapPush(&heap, nb, dist[nb]);
            }
            cur = cur->next;
        }
    }

    printf("\n===== DIJKSTRA RESULT (Shortest Distance) =====\n");
    if (dist[v] == INF) {
        printf("  [!] No path found between '%s' and '%s'.\n", src, dst);
    } else {
        printf("  Source      : %s\n", src);
        printf("  Destination : %s\n", dst);
        printf("  Distance    : %d km\n", dist[v]);
        printPath(parent, u, v);
    }
    printf("================================================\n\n");
}

void bfsMinStops() {
    char src[50], dst[50];
    printf("Enter source station: ");      scanf(" %49[^\n]", src);
    printf("Enter destination station: "); scanf(" %49[^\n]", dst);

    int u = findStation(src), v = findStation(dst);
    if (u == -1) { printf("[!] Station '%s' not found.\n", src); return; }
    if (v == -1) { printf("[!] Station '%s' not found.\n", dst); return; }

    int visited[MAX_STATIONS];
    int parent[MAX_STATIONS];
    int stops[MAX_STATIONS];

    for ( i = 0; i < g.count; i++) {
        visited[i] = 0;
        parent[i]  = -1;
        stops[i]   = INF;
    }

    Queue q;
    queueInit(&q);
    visited[u] = 1;
    stops[u]   = 0;
    enqueue(&q, u);

    while (!queueEmpty(&q)) {
        int node = dequeue(&q);
        AdjNode *cur = g.stations[node].head;
        while (cur) {
            int nb = cur->dest;
            if (!visited[nb]) {
                visited[nb] = 1;
                parent[nb]  = node;
                stops[nb]   = stops[node] + 1;
                if (nb == v) goto bfs_done; /* Found! */
                enqueue(&q, nb);
            }
            cur = cur->next;
        }
    }

bfs_done:
    printf("\n===== BFS RESULT (Minimum Stops) =====\n");
    if (stops[v] == INF) {
        printf("  [!] No path found between '%s' and '%s'.\n", src, dst);
    } else {
        printf("  Source      : %s\n", src);
        printf("  Destination : %s\n", dst);
        printf("  Stops       : %d intermediate station(s)\n", stops[v] - 1);
        printPath(parent, u, v);
    }
    printf("=======================================\n\n");
}



void loadDemoData() {
    initGraph();

    /* Add 7 sample Indian railway stations */
    const char *stations[] = {
        "Mumbai", "Pune", "Nashik", "Surat", "Ahmedabad", "Vadodara", "Nagpur"
    };
    int n = 7;
    for ( i = 0; i < n; i++) {
        strcpy(g.stations[i].name, stations[i]);
        g.stations[i].active = 1;
        g.stations[i].head   = NULL;
    }
    g.count = n;

    /* Routes: {src, dst, distance_km} */
    int routes[][3] = {
        {0,1,150}, {0,2,160}, {0,3,270},
        {1,2,210}, {1,6,700},
        {2,4,550},
        {3,4,260}, {3,5,130},
        {4,5,100}, {4,6,600},
        {5,6,680}
    };
    int r = 11;
    for ( i = 0; i < r; i++) {
        int u = routes[i][0], v = routes[i][1], w = routes[i][2];
        AdjNode *n1 = (AdjNode *)malloc(sizeof(AdjNode));
        n1->dest = v; n1->weight = w; n1->next = g.stations[u].head;
        g.stations[u].head = n1;
        AdjNode *n2 = (AdjNode *)malloc(sizeof(AdjNode));
        n2->dest = u; n2->weight = w; n2->next = g.stations[v].head;
        g.stations[v].head = n2;
    }
    printf(" Demo data loaded (7 stations, 11 routes).\n");
}


void menu() {
    printf("\n");
    printf("   RAILWAY ROUTE OPTIMIZATION SYSTEM     \n");
    printf("  1. Add Station                          \n");
    printf("  2. Add Route                            \n");
    printf("  3. Delete Station                       \n");
    printf("  4. Delete Route                         \n");
    printf("  5. Display Network                      \n");
    printf("  6. Find Shortest Path (Dijkstra)        \n");
    printf("  7. Find Minimum Stops (BFS)             \n");
    printf("  8. Load Demo Data                       \n");
    printf("  0. Exit                                 \n");
    printf(" Enter choice: ");
}

int main(void) {
    initGraph();
    printf("\n  Welcome to the Railway Route Optimization System!\n");

    int choice;
    do {
        menu();
        if (scanf("%d", &choice) != 1) { choice = -1; while(getchar()!='\n'); }
        switch (choice) {
            case 1:  addStation();   break;
            case 2:  addRoute();     break;
            case 3:  deleteStation();break;
            case 4:  deleteRoute();  break;
            case 5:  displayNetwork();break;
            case 6:  dijkstra();     break;
            case 7:  bfsMinStops();  break;
            case 8:  loadDemoData(); break;
            case 0:  printf("  Goodbye!\n"); break;
            default: printf("  [!] Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}
