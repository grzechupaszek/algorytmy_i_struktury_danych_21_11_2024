#include <iostream>
#include <string>

using namespace std;
/*Opis działania algorytmu Knutha–Morrisa–Pratta (KMP)
Algorytm Knutha–Morrisa–Pratta (KMP) służy do wyszukiwania wzorca (ang. pattern) w tekście w czasie liniowym 
𝑂(𝑛) O(n), gdzie 𝑛 to długość tekstu. Głównym celem jest unikanie ponownego porównywania znaków, które już zostały dopasowane. 
KMP korzysta z dodatkowej tablicy nazywanej LPS (Longest Prefix Suffix), w której przechowywane są informacje o tym, 
jak „duży” jest najdłuższy właściwy prefiks wzorca, 
który jest jednocześnie sufiksem fragmentu wzorca zakończonego na danej pozycji.
W załączonym kodzie można wyróżnić dwa zasadnicze etapy:
Obliczanie tablicy LPS (funkcja computeLPSArray).
Właściwe wyszukiwanie wzorca w tekście (funkcja KMPSearchSteps).*/
// Wyswietla aktualny stan obliczania tablicy LPS (Longest Prefix Suffix)
void printLPSState(const string &pattern, int i, int length, int *lps) {
    cout << "i = " << i + 1 << " (P[i] = " << pattern[i] << "), "
         << "dlugosc = " << length << ", LPS: [";
    for (int k = 0; k <= i; k++) {
        cout << lps[k];
        if (k < i) cout << " ";
    }
    cout << "]\n";
}

// Oblicza tablice LPS krok po kroku
void computeLPSArray(const string &pattern, int *lps) {
    int m = (int)pattern.size();
    int length = 0;  // Dlugosc najdluzszego prefiksu sufiksu
    lps[0] = 0;      // Pierwszy element tablicy LPS to zawsze 0
    int i = 1;

    cout << "Tworzenie tablicy LPS (Longest Prefix Suffix):\n";
    cout << "Wzorzec: " << pattern << "\n";
    cout << "Indeksowanie i od 1 dla czytelnosci:\n\n";
    cout << "Poczatkowo: i=1, lps[1]=0\n";

    while (i < m) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            printLPSState(pattern, i, length, lps);
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                printLPSState(pattern, i, length, lps);
                i++;
            }
        }
    }
    cout << "\nGotowa tablica LPS:\n[ ";
    for (int idx = 0; idx < m; idx++)
        cout << lps[idx] << " ";
    cout << "]\n\n";
}

// Wyswietla aktualny stan dopasowywania wzorca w tekscie
void printMatchState(const string &text, const string &pattern, int s, int q) {
    cout << "Tekst:    ";
    for (int i = 0; i < (int)text.size(); i++) {
        cout << text[i] << " ";
    }
    cout << "\nWzorzec: ";
    for (int i = 0; i < s; i++) cout << "  ";
    for (int i = 0; i < (int)pattern.size(); i++) {
        cout << pattern[i] << " ";
    }
    cout << "\nAktualne przesuniecie s = " << s << ", q = " << q << " (dlugosc dopasowania)\n\n";
}

// Wyszukiwanie wzorca w tekscie metoda KMP
void KMPSearchSteps(const string &text, const string &pattern, int *lps) {
    int n = (int)text.size();
    int m = (int)pattern.size();

    cout << "Kroki wyszukiwania wzorca metoda KMP:\n";
    cout << "Tekst: " << text << "\nWzorzec: " << pattern << "\n\n";

    int i = 0; // Indeks w tekscie
    int j = 0; // Indeks w wzorcu

    printMatchState(text, pattern, i - j, j);

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
            printMatchState(text, pattern, i - j, j);

            if (j == m) {
                // Pelne dopasowanie znalezione
                cout << "Dopasowanie znalezione na pozycji " << (i - j) << "\n\n";
                j = lps[j - 1];
                printMatchState(text, pattern, i - j, j);
            }
        } else {
            if (j != 0) {
                j = lps[j - 1];
                printMatchState(text, pattern, i - j, j);
            } else {
                i++;
                printMatchState(text, pattern, i - j, j);
            }
        }
    }
}

int main() {
    string text = "bacbabbaabab";   // Tekst, w ktorym szukamy
    string pattern = "ababbabca";   // Wzorzec do znalezienia

    int m = (int)pattern.size();
    int *lps = new int[m];

    // Obliczanie tablicy LPS krok po kroku
    computeLPSArray(pattern, lps);

    // Wyswietlanie krokow dopasowywania wzorca
    KMPSearchSteps(text, pattern, lps);

    delete[] lps;
    return 0;
}
