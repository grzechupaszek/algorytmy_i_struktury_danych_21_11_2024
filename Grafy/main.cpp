#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/**
 *  Zmienna globalna V - faktyczna liczba wierzchołków w grafie.
 *  Ustalamy ją dopiero po wczytaniu z pliku (readGraphFromFile).
 */
int V = 0;

/** 
 *  INF - oznaczenie braku krawędzi (duża liczba).
 */
static const int INF = 999999;

/**
 *  Zmienne globalne, alokowane dynamicznie w czasie działania:
 *    - graph: macierz sąsiedztwa (tworzona jako tablica wskaźników),
 *    - parent, rnk: tablice używane w Union-Find.
 */
int** graph = nullptr; 
int* parent = nullptr;
int* rnk = nullptr;

/**
 *  Struktura krawędzi używana w algorytmie Kruskala.
 */
struct Edge {
    int src;     // wierzchołek źródłowy (indeks 0..V-1)
    int dest;    // wierzchołek docelowy (indeks 0..V-1)
    int weight;  // waga krawędzi
};

/**
 *  readGraphFromFile(filename)
 *  --------------------------------------------------------
 *  Funkcja:
 *   1) Otwiera plik.
 *   2) Wczytuje liczbę wierzchołków n.
 *   3) Alokuje dynamicznie macierz graph (n x n).
 *   4) Wczytuje wagi krawędzi do macierzy.
 *      - 0 na przekątnej zostaje 0 (odległość do samego siebie),
 *      - 0 poza przekątną → ustawiamy INF (brak krawędzi),
 *      - dodatnia wartość to normalna waga krawędzi.
 *  Zwraca:
 *     true  - jeśli udało się poprawnie wczytać,
 *     false - w razie błędu (lub niezgodności danych).
 */
bool readGraphFromFile(const string &filename) {
    ifstream file(filename.c_str());
    if(!file) {
        cerr << "Blad otwarcia pliku: " << filename << endl;
        return false;
    }

    // 1) Wczytujemy liczbę wierzchołków
    int n;
    file >> n;
    if(!file || n <= 0) {
        cerr << "Nieprawidlowa liczba wierzcholkow: " << n << endl;
        return false;
    }

    // Ustawiamy globalne V (liczbę wierzchołków)
    V = n;

    // 2) Alokujemy dynamicznie macierz graph (n x n)
    graph = new int*[V];
    for(int i=0; i<V; i++){
        graph[i] = new int[V];
    }

    // 3) Wczytujemy wagi krawędzi do graph[i][j]
    for(int i=0; i<V; i++){
        for(int j=0; j<V; j++){
            int w;
            file >> w;
            if(!file) {
                cerr << "Blad podczas wczytywania wagi krawedzi (" << i << "," << j << ")" << endl;
                return false;
            }
            if(i == j) {
                // przekątna = 0
                graph[i][j] = 0;
            } else if(w == 0) {
                // brak krawędzi
                graph[i][j] = INF;
            } else {
                graph[i][j] = w;
            }
        }
    }
    file.close();
    return true;
}

/**
 *  Inicjalizacja Union-Find:
 *   - parent[i] = i
 *   - rnk[i] = 0
 */
void initUnionFind() {
    // Alokujemy dynamicznie parent i rnk (jeśli jeszcze nie są zaalokowane)
    parent = new int[V];
    rnk = new int[V];
    for(int i=0; i<V; i++){
        parent[i] = i;
        rnk[i] = 0;
    }
}

/**
 *  findSet(x)
 *  -----------------
 *  Funkcja zwraca reprezentanta zbioru (wierzchołka) x
 *  w strukturze Union-Find (z optymalizacją path compression).
 */
int findSet(int x) {
    if(parent[x] != x) {
        parent[x] = findSet(parent[x]);
    }
    return parent[x];
}

/**
 *  unionSet(x, y)
 *  -----------------
 *  Łączy dwa zbiory w Union-Find, jeśli są różne.
 *  Stosuje union by rank.
 */
void unionSet(int x, int y) {
    int rx = findSet(x);
    int ry = findSet(y);
    if(rx != ry) {
        if(rnk[rx] < rnk[ry]) {
            parent[rx] = ry;
        } else if(rnk[rx] > rnk[ry]) {
            parent[ry] = rx;
        } else {
            parent[ry] = rx;
            rnk[rx]++;
        }
    }
}

/**
 *  getAllEdges(edges)
 *  -----------------
 *  Funkcja pomocnicza do algorytmu Kruskala:
 *   - przechodzi po macierzy graph (tylko dla j > i, bo graf nieskierowany),
 *   - zbiera krawędzie (i,j) o wadze != INF i != 0
 *     do tablicy edges.
 *  Zwraca liczbę zebranych krawędzi.
 */
int getAllEdges(Edge edges[]) {
    int k = 0;
    for(int i = 0; i < V; i++){
        for(int j = i+1; j < V; j++){
            if(graph[i][j] != 0 && graph[i][j] != INF) {
                edges[k].src = i;
                edges[k].dest = j;
                edges[k].weight = graph[i][j];
                k++;
            }
        }
    }
    return k;
}

/**
 *  sortEdges(edges, n)
 *  -----------------
 *  Prosty bubble sort (bez <algorithm>) sortujący krawędzie po wadze.
 */
void sortEdges(Edge edges[], int n) {
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-1-i; j++){
            if(edges[j].weight > edges[j+1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j+1];
                edges[j+1] = temp;
            }
        }
    }
}

/**
 *  kruskalMST()
 *  -----------------
 *  Implementacja algorytmu Kruskala do wyznaczenia MST (Minimalnego Drzewa Rozpinającego).
 */
void kruskalMST() {
    // Musimy mieć zainicjalizowany Union-Find
    initUnionFind();

    // Potencjalnie maksymalna liczba krawędzi w grafie nieskierowanym
    // to V*(V-1)/2
    Edge* edges = new Edge[V*(V-1)/2];

    // Pobranie istniejących krawędzi
    int totalEdges = getAllEdges(edges);

    // Sortowanie krawędzi po wadze
    sortEdges(edges, totalEdges);

    cout << "\n--- MST (Kruskal) ---\n";
    int edgesUsed = 0;
    for(int i=0; i<totalEdges && edgesUsed < V-1; i++){
        int s = edges[i].src;
        int d = edges[i].dest;
        // Jeśli wierzchołki w różnych zbiorach, łączymy
        if(findSet(s) != findSet(d)) {
            unionSet(s, d);
            edgesUsed++;
            // Wypis krawędzi (zamieniamy indeksy 0.. na 1..)
            cout << "(" << s+1 << "," << d+1 << ") = "
                 << edges[i].weight << "\n";
        }
    }

    // zwalniamy pamięć tymczasowej tablicy edges
    delete[] edges;
}

/**
 *  primMST()
 *  -----------------
 *  Implementacja algorytmu Prima przy użyciu macierzy sąsiedztwa.
 */
void primMST() {
    // tablice pomocnicze
    bool* inMST = new bool[V];
    int* key = new int[V];
    int* parentPrim = new int[V];

    // Inicjalizacja
    for(int i=0; i<V; i++){
        inMST[i] = false;
        key[i] = INF;
        parentPrim[i] = -1;
    }
    // Zaczynamy od wierzchołka 0
    key[0] = 0;

    // Główna pętla
    for(int _=0; _<V-1; _++){
        // Wybór wierzchołka o najmniejszym key spoza MST
        int u = -1;
        int minKey = INF;
        for(int i=0; i<V; i++){
            if(!inMST[i] && key[i] < minKey) {
                minKey = key[i];
                u = i;
            }
        }
        if(u == -1) break; // graf może być niespójny

        // Dodajemy u do MST
        inMST[u] = true;

        // Aktualizujemy key sąsiadów
        for(int v=0; v<V; v++){
            if(graph[u][v] != INF && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parentPrim[v] = u;
            }
        }
    }

    // Wypisanie MST
    cout << "\n--- MST (Prim) ---\n";
    for(int v=1; v<V; v++){
        if(parentPrim[v] != -1) {
            cout << "(" << parentPrim[v]+1 << "," << v+1 << ") = "
                 << graph[v][parentPrim[v]] << "\n";
        }
    }

    // zwalniamy pamięć tablic tymczasowych
    delete[] inMST;
    delete[] key;
    delete[] parentPrim;
}

/**
 *  dijkstra(start)
 *  -----------------
 *  Implementacja algorytmu Dijkstry dla macierzy sąsiedztwa.
 */
void dijkstra(int start) {
    bool* visited = new bool[V];
    int* dist = new int[V];

    for(int i=0; i<V; i++){
        visited[i] = false;
        dist[i] = INF;
    }
    dist[start] = 0;

    // pętla (V-1) razy
    for(int _=0; _<V-1; _++){
        // wybieramy wierzchołek o najmniejszej dist spośród nieodwiedzonych
        int u = -1;
        int minDist = INF;
        for(int i=0; i<V; i++){
            if(!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }
        if(u == -1) break; // brak osiągalnych wierzchołków

        visited[u] = true;

        // Relaksacja krawędzi wychodzących z u
        for(int v=0; v<V; v++){
            if(!visited[v] && graph[u][v] != INF) {
                int alt = dist[u] + graph[u][v];
                if(alt < dist[v]) {
                    dist[v] = alt;
                }
            }
        }
    }

    // Wypisanie uzyskanych odległości
    cout << "\n--- Odleglosci Dijkstra od wierzcholka " << (start+1) << " ---\n";
    for(int i=0; i<V; i++){
        if(dist[i] == INF) {
            cout << "Do wierzcholka " << (i+1) << ": brak dojscia\n";
        } else {
            cout << "Do wierzcholka " << (i+1) << ": " << dist[i] << "\n";
        }
    }

    delete[] visited;
    delete[] dist;
}

/**
 *  Funkcja main()
 *  -----------------
 *  1. Wczytuje graf z pliku "graf.txt" do dynamicznie alokowanej macierzy 'graph'.
 *  2. Uruchamia algorytm Kruskala (kruskalMST).
 *  3. Uruchamia algorytm Prima (primMST).
 *  4. Uruchamia algorytm Dijkstry (dijkstra) dla startu = 0 (pierwszy wierzchołek).
 *  5. Zwalnia pamięć (kasuje macierz 'graph', tablice 'parent' i 'rnk').
 */
int main() {
    ios::sync_with_stdio(false);

    // Wczytujemy graf z pliku
    if(!readGraphFromFile("graf.txt")) {
        cerr << "Nie udalo sie wczytac grafu z pliku.\n";
        return 1;
    }

    // Kruskal
    kruskalMST();

    // Prim
    primMST();

    // Dijkstra - od wierzchołka 1 (indeks 0)
    dijkstra(0);

    // Po zakończeniu wszystkich działań zwalniamy pamięć:
    // 1) macierz graph
    if(graph) {
        for(int i=0; i<V; i++){
            delete[] graph[i];
        }
        delete[] graph;
        graph = nullptr;
    }
    // 2) tablice Union-Find
    if(parent) {
        delete[] parent;
        parent = nullptr;
    }
    if(rnk) {
        delete[] rnk;
        rnk = nullptr;
    }

    return 0;
}
