#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 50
#define MAX_NAME_LEN 50
#define INF 1000000000

// ----------------------
// Struktur EDGE (sisi)
// ----------------------
typedef struct Edge {
    int dest;               // index vertex tujuan
    int weight;             // jarak
    struct Edge *next;      // next edge (linked list)
} Edge;

// ----------------------
// Struktur VERTEX (lokasi)
// ----------------------
typedef struct {
    char name[MAX_NAME_LEN];
    Edge *head;             // head dari adjacency list
} Vertex;

// ----------------------
// Struktur GRAPH
// ----------------------
typedef struct {
    Vertex vertices[MAX_VERTICES];
    int vertex_count;
} Graph;

// ----------------------
// Struktur QUEUE (untuk BFS)
// ----------------------
typedef struct {
    int data[MAX_VERTICES];
    int front;
    int rear;
} Queue;

// ----------------------
// Priority Queue (MIN-HEAP) untuk Dijkstra
// ----------------------
typedef struct {
    int vertex;
    int dist;
} PQNode;

typedef struct {
    PQNode data[MAX_VERTICES];
    int size;
} PriorityQueue;


// Inisialisasi graph
void init_graph(Graph *g) {
    g->vertex_count = 0;
}

// Mencari index vertex berdasarkan nama
int find_vertex(Graph *g, const char *name) {
    for (int i = 0; i < g->vertex_count; i++) {
        if (strcmp(g->vertices[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // tidak ditemukan
} 

// Menambah vertex baru (lokasi)
int add_vertex(Graph *g, const char *name) {
    if (g->vertex_count >= MAX_VERTICES) {
        printf("Maksimum jumlah lokasi tercapai!\n");
        return -1;
    }

    // Cek kalau sudah ada
    int idx = find_vertex(g, name);
    if (idx != -1) {
        // sudah ada, cukup kembalikan index
        return idx;
    }

    strcpy(g->vertices[g->vertex_count].name, name);
    g->vertices[g->vertex_count].head = NULL;

    g->vertex_count++;
    return g->vertex_count - 1;
}

// Menambah edge (koneksi) antar dua lokasi
// Untuk kesederhanaan: graph dianggap BIDIRECTIONAL (dua arah)
void add_edge(Graph *g, int src, int dest, int weight) {
    // edge dari src ke dest
    Edge *newEdge = (Edge *)malloc(sizeof(Edge));
    newEdge->dest = dest;
    newEdge->weight = weight;
    newEdge->next = g->vertices[src].head;
    g->vertices[src].head = newEdge;

    // edge dari dest ke src (karena dua arah)
    Edge *newEdge2 = (Edge *)malloc(sizeof(Edge));
    newEdge2->dest = src;
    newEdge2->weight = weight;
    newEdge2->next = g->vertices[dest].head;
    g->vertices[dest].head = newEdge2;
}

// Menampilkan semua lokasi dan koneksi
void print_graph(Graph *g) {
    printf("\n=== Daftar Lokasi & Koneksi ===\n");
    for (int i = 0; i < g->vertex_count; i++) {
        printf("%s:", g->vertices[i].name);
        Edge *curr = g->vertices[i].head;
        while (curr != NULL) {
            printf(" -> %s(%d)", g->vertices[curr->dest].name, curr->weight);
            curr = curr->next;
        }
        printf("\n");
    }
    printf("===============================\n");
}


// ----------------------
// Queue untuk BFS
// ----------------------
void init_queue(Queue *q) {
    q->front = 0;
    q->rear = -1;
}

int is_empty_queue(Queue *q) {
    return q->rear < q->front;
}

void enqueue(Queue *q, int value) {
    if (q->rear + 1 >= MAX_VERTICES) {
        printf("Queue penuh!\n");
        return;
    }
    q->rear++;
    q->data[q->rear] = value;
}

int dequeue(Queue *q) {
    if (is_empty_queue(q)) {
        return -1;
    }
    int value = q->data[q->front];
    q->front++;
    return value;
}

// Rekonstruksi dan print path dari parent[]
void print_path_bfs(Graph *g, int parent[], int start, int goal) {
    int path[MAX_VERTICES];
    int length = 0;

    int current = goal;
    while (current != -1) {
        path[length++] = current;
        current = parent[current];
    }

    // path tersusun dari goal -> start, jadi dibalik
    printf("Rute termudah (paling sedikit langkah):\n");
    for (int i = length - 1; i >= 0; i--) {
        printf("%s", g->vertices[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\nJumlah langkah: %d\n", length - 1);
}

// BFS untuk rute termudah (tanpa bobot jarak)
void bfs_route(Graph *g, int start, int goal) {
    int visited[MAX_VERTICES] = {0};
    int parent[MAX_VERTICES];

    for (int i = 0; i < MAX_VERTICES; i++) {
        parent[i] = -1;
    }

    Queue q;
    init_queue(&q);

    visited[start] = 1;
    enqueue(&q, start);

    while (!is_empty_queue(&q)) {
        int u = dequeue(&q);

        if (u == goal) {
            break;
        }

        Edge *curr = g->vertices[u].head;
        while (curr != NULL) {
            int v = curr->dest;
            if (!visited[v]) {
                visited[v] = 1;
                parent[v] = u;
                enqueue(&q, v);
            }
            curr = curr->next;
        }
    }

    if (parent[goal] == -1 && start != goal) {
        printf("Tidak ada rute dari %s ke %s\n",
               g->vertices[start].name, g->vertices[goal].name);
    } else {
        print_path_bfs(g, parent, start, goal);
    }
}


// ----------------------
// Priority Queue (Min-Heap)
// ----------------------
void init_pq(PriorityQueue *pq) {
    pq->size = 0;
}

int is_empty_pq(PriorityQueue *pq) {
    return pq->size == 0;
}

void swap_pq(PQNode *a, PQNode *b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

void push_pq(PriorityQueue *pq, int vertex, int dist) {
    if (pq->size >= MAX_VERTICES) {
        printf("Priority queue penuh!\n");
        return;
    }
    int i = pq->size;
    pq->data[i].vertex = vertex;
    pq->data[i].dist = dist;
    pq->size++;

    // percolate up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->data[parent].dist <= pq->data[i].dist) break;
        swap_pq(&pq->data[parent], &pq->data[i]);
        i = parent;
    }
}

PQNode pop_pq(PriorityQueue *pq) {
    PQNode result = {-1, -1};
    if (pq->size == 0) return result;

    result = pq->data[0];
    pq->data[0] = pq->data[pq->size - 1];
    pq->size--;

    // percolate down
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < pq->size && pq->data[left].dist < pq->data[smallest].dist)
            smallest = left;
        if (right < pq->size && pq->data[right].dist < pq->data[smallest].dist)
            smallest = right;

        if (smallest == i) break;

        swap_pq(&pq->data[i], &pq->data[smallest]);
        i = smallest;
    }

    return result;
}

// Rekonstruksi dan cetak path dari prev[]
void print_path_dijkstra(Graph *g, int prev[], int start, int goal, int dist[]) {
    int path[MAX_VERTICES];
    int length = 0;

    int current = goal;
    while (current != -1) {
        path[length++] = current;
        current = prev[current];
    }

    printf("Rute tersingkat (berdasarkan jarak):\n");
    for (int i = length - 1; i >= 0; i--) {
        printf("%s", g->vertices[path[i]].name);
        if (i > 0) printf(" -> ");
    }
    printf("\nTotal jarak: %d\n", dist[goal]);
}

// Dijkstra untuk cari jarak terpendek
void dijkstra(Graph *g, int start, int goal) {
    int dist[MAX_VERTICES];
    int prev[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};

    for (int i = 0; i < MAX_VERTICES; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }

    dist[start] = 0;

    PriorityQueue pq;
    init_pq(&pq);
    push_pq(&pq, start, 0);

    while (!is_empty_pq(&pq)) {
        PQNode node = pop_pq(&pq);
        int u = node.vertex;

        if (visited[u]) continue;
        visited[u] = 1;

        if (u == goal) break;

        Edge *curr = g->vertices[u].head;
        while (curr != NULL) {
            int v = curr->dest;
            int w = curr->weight;

            if (!visited[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                push_pq(&pq, v, dist[v]);
            }
            curr = curr->next;
        }
    }

    if (dist[goal] == INF) {
        printf("Tidak ada rute dari %s ke %s\n",
               g->vertices[start].name, g->vertices[goal].name);
    } else {
        print_path_dijkstra(g, prev, start, goal, dist);
    }
}


// Fungsi helper untuk input nama lokasi
void input_string(char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    scanf(" %49[^\n]", buffer);   // baca sampai newline, max 49 char
}

int main() {
    Graph g;
    init_graph(&g);

    int pilihan;
    char nama1[MAX_NAME_LEN], nama2[MAX_NAME_LEN];
    int jarak;

    while (1) {
        printf("\n=== SISTEM REKOMENDASI RUTE ===\n");
        printf("1. Tambah lokasi\n");
        printf("2. Tambah koneksi (jarak antar lokasi)\n");
        printf("3. Tampilkan graph\n");
        printf("4. Cari rute termudah (BFS - paling sedikit langkah)\n");
        printf("5. Cari rute tersingkat (Dijkstra - jarak terkecil)\n");
        printf("0. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        if (pilihan == 0) {
            printf("Keluar...\n");
            break;
        }

        switch (pilihan) {
        case 1:
            input_string("Nama lokasi baru: ", nama1, MAX_NAME_LEN);
            if (add_vertex(&g, nama1) != -1) {
                printf("Lokasi '%s' ditambahkan.\n", nama1);
            }
            break;

        case 2:
            input_string("Lokasi asal : ", nama1, MAX_NAME_LEN);
            input_string("Lokasi tujuan : ", nama2, MAX_NAME_LEN);
            printf("Jarak (angka): ");
            scanf("%d", &jarak);

            {
                int idx1 = add_vertex(&g, nama1);
                int idx2 = add_vertex(&g, nama2);
                if (idx1 != -1 && idx2 != -1) {
                    add_edge(&g, idx1, idx2, jarak);
                    printf("Koneksi %s <-> %s dengan jarak %d ditambahkan.\n",
                           nama1, nama2, jarak);
                }
            }
            break;

        case 3:
            print_graph(&g);
            break;

        case 4:
            input_string("Lokasi awal : ", nama1, MAX_NAME_LEN);
            input_string("Lokasi tujuan : ", nama2, MAX_NAME_LEN);
            {
                int s = find_vertex(&g, nama1);
                int t = find_vertex(&g, nama2);
                if (s == -1 || t == -1) {
                    printf("Lokasi tidak ditemukan!\n");
                } else {
                    bfs_route(&g, s, t);
                }
            }
            break;

        case 5:
            input_string("Lokasi awal : ", nama1, MAX_NAME_LEN);
            input_string("Lokasi tujuan : ", nama2, MAX_NAME_LEN);
            {
                int s = find_vertex(&g, nama1);
                int t = find_vertex(&g, nama2);
                if (s == -1 || t == -1) {
                    printf("Lokasi tidak ditemukan!\n");
                } else {
                    dijkstra(&g, s, t);
                }
            }
            break;

        default:
            printf("Pilihan tidak valid!\n");
        }
    }

    return 0;
}
