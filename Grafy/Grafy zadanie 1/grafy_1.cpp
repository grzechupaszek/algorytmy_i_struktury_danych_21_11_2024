#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/** 
 *  INF - ewentualnie przydatne, jeśli chcemy traktować 
 *        "brak krawędzi" jako dużą liczbę.
 *  Niektóre grafy używają 0 = brak krawędzi poza przekątną,
 *  a w kodzie interpretujemy to jako "brak" i nie zapisujemy do LE/LN.
 */
static const int INF = 999999;

/**
 *  Struktura do przechowywania listy krawędzi (LE)
 */
struct Edge {
    int from;   // wierzchołek początkowy (0..n-1)
    int to;     // wierzchołek końcowy   (0..n-1)
    int w;      // waga krawędzi
};

/**
 *  Struktura pojedynczego węzła listy sąsiedztwa (LN)
 */
struct Node {
    int v;        // numer sąsiada
    int w;        // waga krawędzi (this -> v)
    Node* next;   // wskaźnik na kolejny sąsiad
};

/**
 *  1) Wczytanie macierzy sąsiedztwa z pliku (MN)
 *  
 *  Z pliku (filename):
 *  - pierwsza linia: liczba wierzchołków n
 *  - kolejne n wierszy, każdy z n liczbami:
 *      0 = brak krawędzi (o ile i != j)
 *      > 0 = waga krawędzi
 *      i == j => 0 na przekątnej
 *  
 *  Zwraca rzeczywistą liczbę wierzchołków w n (przez parametr),
 *  a jako wynik - wskaźnik na tablicę MN (n×n).
 */
int** readMNFromFile(const string &filename, int &n) {
    ifstream fin(filename.c_str());
    if(!fin) {
        cerr << "Blad otwarcia pliku: " << filename << endl;
        n = 0;
        return nullptr;
    }
    fin >> n;
    if(!fin || n <= 0) {
        cerr << "Nieprawidlowa liczba wierzcholkow: " << n << endl;
        n = 0;
        return nullptr;
    }

    // Alokacja macierzy n×n
    int** MN = new int*[n];
    for(int i=0; i<n; i++){
        MN[i] = new int[n];
    }

    // Wczytanie wag do MN[i][j]
    // Zakładamy, że 0 (poza przekątną) -> brak krawędzi
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int w;
            fin >> w;
            if(!fin) {
                cerr << "Blad wczytywania wagi (" << i << "," << j << ")" << endl;
                // zwolnienie i return
                for(int k=0; k<=i; k++) delete[] MN[k];
                delete[] MN;
                n=0;
                return nullptr;
            }
            MN[i][j] = w; 
        }
    }
    fin.close();
    return MN;
}

/**
 *  Pomocnicza funkcja wypisująca macierz MN o rozmiarze n×n.
 *  Zakładamy, że 0 poza przekątną oznacza brak krawędzi.
 */
void printMN(int** MN, int n) {
    cout << "\n=== Macierz sasiedztwa (MN) ===\n";
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << MN[i][j] << " ";
        }
        cout << "\n";
    }
}

/**
 *  2) Wygeneruj LE (listę krawędzi) w oparciu o MN (macierz sąsiedztwa).
 *     Graf nieskierowany => dodajemy krawędź (i,j) tylko wtedy, gdy j > i
 *     i MN[i][j] != 0.
 *  
 *  Parametry:
 *    MN  - macierz n×n
 *    n   - liczba wierzchołków
 *    edgesCount - parametr WYJŚCIOWY, zapisujemy w nim liczbę krawędzi
 *  
 *  Zwraca wskaźnik na dynamicznie zaalokowaną tablicę Edge[].
 */
Edge* generateLEfromMN(int** MN, int n, int &edgesCount) {
    // Policz, ile krawędzi (i<j i MN[i][j]!=0)
    int count = 0;
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            if(MN[i][j] != 0) {
                count++;
            }
        }
    }

    // Alokujemy tablicę krawędzi
    Edge* LE = new Edge[count];
    int idx = 0;
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){
            if(MN[i][j] != 0) {
                LE[idx].from = i;
                LE[idx].to   = j;
                LE[idx].w    = MN[i][j];
                idx++;
            }
        }
    }
    edgesCount = count;
    return LE;
}

/**
 *  Pomocnicza funkcja wypisująca listę krawędzi (LE)
 */
void printLE(Edge* LE, int edgesCount) {
    cout << "\n=== Lista krawedzi (LE) ===\n";
    for(int i=0; i<edgesCount; i++){
        cout << "(" << LE[i].from+1 << "," 
             << LE[i].to+1 << ") = " 
             << LE[i].w << "\n";
    }
}

/**
 *  3) Wygeneruj LN (listę sąsiedztwa) w oparciu o LE (listę krawędzi).
 *     Graf nieskierowany => każdą krawędź (u,v,w) zapisujemy do LN[u] i LN[v].
 *  
 *  Zwraca tablicę wskaźników Node*[n], gdzie LN[u] to początek listy sąsiedztwa u.
 */
Node** generateLNfromLE(Edge* LE, int edgesCount, int n) {
    // Alokujemy tablicę wskaźników na listy
    Node** LN = new Node*[n];
    for(int i=0; i<n; i++){
        LN[i] = nullptr;
    }
    // Funkcja pomocnicza, dodająca węzeł (v,w) do listy LN[u]
    auto addEdge = [&](int u, int v, int w) {
        Node* tmp = new Node;
        tmp->v = v;
        tmp->w = w;
        tmp->next = LN[u];
        LN[u] = tmp;
    };

    // Przejście po wszystkich krawędziach
    for(int i=0; i<edgesCount; i++){
        int u = LE[i].from;
        int v = LE[i].to;
        int w = LE[i].w;
        // Dodajemy do LN[u] i LN[v] 
        addEdge(u, v, w);
        addEdge(v, u, w);
    }

    return LN;
}

/**
 *  Pomocnicza funkcja wypisująca listę sąsiedztwa LN.
 *  Dla wierzchołka i wypisujemy jego sąsiadów i wagi krawędzi.
 */
void printLN(Node** LN, int n) {
    cout << "\n=== Lista sasiedztwa (LN) ===\n";
    for(int i=0; i<n; i++){
        cout << "Wierzcholek " << (i+1) << ": ";
        Node* p = LN[i];
        while(p) {
            cout << "(" << p->v+1 << "," << p->w << ") ";
            p = p->next;
        }
        cout << "\n";
    }
}

/**
 *  4) Wygeneruj LE (listę krawędzi) w oparciu o LN (listę sąsiedztwa).
 *     Graf nieskierowany => żeby nie dublować (u,v) oraz (v,u), 
 *     przyjmujemy zasadę: bierzemy krawędź (u,v) tylko, gdy u < v.
 *
 *  Zwraca nową tablicę krawędzi i zapisuje liczbę w edgesCount.
 */
Edge* generateLEfromLN(Node** LN, int n, int &edgesCount) {
    // Najpierw policzymy ile będzie krawędzi (u < v)
    int count = 0;
    for(int u=0; u<n; u++){
        Node* p = LN[u];
        while(p){
            int v = p->v;
            if(u < v) {
                count++;
            }
            p = p->next;
        }
    }
    Edge* LE = new Edge[count];
    int idx = 0;
    for(int u=0; u<n; u++){
        Node* p = LN[u];
        while(p){
            int v = p->v;
            int w = p->w;
            if(u < v) {
                LE[idx].from = u;
                LE[idx].to = v;
                LE[idx].w = w;
                idx++;
            }
            p = p->next;
        }
    }
    edgesCount = count;
    return LE;
}

int main() {
    ios::sync_with_stdio(false);

    // 1. Wczytanie macierzy MN z pliku graf.txt
    int n = 0;
    int** MN = readMNFromFile("graf.txt", n);
    if(!MN || n == 0) {
        cerr << "Nie udalo sie wczytac macierzy.\n";
        return 1;
    }
    printMN(MN, n);

    // 2. Wygenerowanie LE z MN
    int edgesCount = 0;
    Edge* LE = generateLEfromMN(MN, n, edgesCount);
    printLE(LE, edgesCount);

    // 3. Wygenerowanie LN z LE
    Node** LN = generateLNfromLE(LE, edgesCount, n);
    printLN(LN, n);

    // 4. (dla demonstracji) ponowne wygenerowanie LE z LN
    int edgesCount2 = 0;
    Edge* LE2 = generateLEfromLN(LN, n, edgesCount2);
    cout << "\n=== Lista krawedzi wygenerowana z LN (LE2) ===\n";
    for(int i=0; i<edgesCount2; i++){
        cout << "(" << LE2[i].from+1 << "," 
             << LE2[i].to+1 << ") = " 
             << LE2[i].w << "\n";
    }

    // Zwolnienie pamięci:
    //  - macierz MN
    for(int i=0; i<n; i++){
        delete[] MN[i];
    }
    delete[] MN;

    //  - tablica LE i LE2
    delete[] LE;
    delete[] LE2;

    //  - lista LN
    for(int i=0; i<n; i++){
        Node* p = LN[i];
        while(p) {
            Node* temp = p;
            p = p->next;
            delete temp;
        }
    }
    delete[] LN;

    return 0;
}
