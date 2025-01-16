#include <iostream>
#include <string>

using namespace std;

/**
 * @file
 * @brief Implementacja algorytmu Knutha–Morrisa–Pratta (KMP) z wizualizacją kroków.
 *
 * Algorytm KMP służy do wyszukiwania wzorca w tekście w czasie liniowym O(n),
 * gdzie n to długość tekstu. Wykorzystuje tablicę LPS (Longest Prefix Suffix),
 * aby unikać ponownego porównywania znaków.
 */

/**
 * @brief Wyświetla aktualny stan obliczania tablicy LPS.
 *
 * Funkcja pomocnicza wykorzystywana w \ref computeLPSArray.  
 * Prezentuje krok po kroku, jak zmienia się wartość `length` (długość najdłuższego
 * prefiksu będącego jednocześnie sufiksem) oraz sama tablica LPS po kolejnych operacjach.
 *
 * @param pattern   Wzorzec, dla którego budowana jest tablica LPS
 * @param i         Bieżący indeks w pattern (wyświetlany w trybie 1-indeksowanym)
 * @param length    Aktualna długość najdłuższego właściwego prefiksu-sufiksu
 * @param lps       Tablica LPS (Longest Prefix Suffix)
 */
void printLPSState(const string &pattern, int i, int length, int *lps) {
    cout << "i = " << i + 1 << " (P[i] = " << pattern[i] << "), "
         << "dlugosc = " << length << ", LPS: [";
    for (int k = 0; k <= i; k++) {
        cout << lps[k];
        if (k < i) cout << " ";
    }
    cout << "]\n";
}

/**
 * @brief Oblicza tablicę LPS (Longest Prefix Suffix) dla zadanego wzorca.
 *
 * Algorytm buduje tablicę LPS w czasie liniowym względem długości wzorca.
 * Dla każdego indeksu i tablica LPS przechowuje długość najdłuższego właściwego
 * prefiksu wzorca, który jest jednocześnie jego sufiksem do pozycji i włącznie.
 *
 * @param pattern   Wzorzec, dla którego wyliczamy tablicę LPS
 * @param lps       Tablica (o rozmiarze równym długości wzorca) do wypełnienia wartościami LPS
 *
 * @note Funkcja wyświetla kolejne kroki obliczania LPS, wraz ze zmianami wartości `length`.
 */
void computeLPSArray(const string &pattern, int *lps) {
    int m = (int)pattern.size();
    int length = 0;  // Długość najdłuższego prefiksu-sufiksu
    lps[0] = 0;      // Pierwszy element tablicy LPS to zawsze 0
    int i = 1;

    cout << "Tworzenie tablicy LPS (Longest Prefix Suffix):\n";
    cout << "Wzorzec: " << pattern << "\n";
    cout << "Indeksowanie i od 1 dla czytelnosci:\n\n";
    cout << "Poczatkowo: i=1, lps[1]=0\n";

    while (i < m) {
        if (pattern[i] == pattern[length]) {
            // Jeśli znaki się pokrywają, wydłużamy długość dopasowania
            length++;
            lps[i] = length;
            printLPSState(pattern, i, length, lps);
            i++;
        } else {
            // Jeśli znaki się nie pokrywają
            if (length != 0) {
                // Używamy wcześniej obliczonej wartości lps[length - 1]
                length = lps[length - 1];
            } else {
                // Jeśli nie możemy się cofnąć, to ustawiamy lps[i] na 0 i idziemy dalej
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

/**
 * @brief Wyświetla aktualny stan dopasowywania wzorca w tekście (dla algorytmu KMP).
 *
 * Funkcja pomocnicza wykorzystywana w \ref KMPSearchSteps, aby zobrazować:
 * - pozycję w tekście,
 * - aktualne dopasowanie wzorca,
 * - bieżące wartości zmiennych \p s (przesunięcie) i \p q (liczba dopasowanych znaków).
 *
 * @param text      Tekst, w którym wyszukujemy
 * @param pattern   Wzorzec, który dopasowujemy
 * @param s         Aktualne przesunięcie wzorca względem tekstu (i - j w kodzie)
 * @param q         Bieżąca długość zgodnego dopasowania (liczba kolejnych pasujących znaków)
 */
void printMatchState(const string &text, const string &pattern, int s, int q) {
    cout << "Tekst:    ";
    for (int i = 0; i < (int)text.size(); i++) {
        cout << text[i] << " ";
    }
    cout << "\nWzorzec: ";
    for (int i = 0; i < s; i++) cout << "  "; // Przesunięcie wizualne
    for (int i = 0; i < (int)pattern.size(); i++) {
        cout << pattern[i] << " ";
    }
    cout << "\nAktualne przesuniecie s = " << s << ", q = " << q 
         << " (dlugosc dopasowania)\n\n";
}

/**
 * @brief Wyszukuje wystąpienia wzorca w tekście metodą KMP, wyświetlając kolejne kroki.
 *
 * Przechodzi liniowo po tekście (indeks \p i) i dopasowuje znaki wzorca (indeks \p j).
 * Gdy wystąpi niezgodność:
 * - Jeśli j != 0, to wartość j zostaje zaktualizowana na lps[j-1].
 * - W przeciwnym wypadku zwiększamy i (przesuwamy się w tekście o 1).
 *
 * @param text      Tekst, w którym wyszukujemy wzorzec
 * @param pattern   Wzorzec, który chcemy dopasować
 * @param lps       Tablica LPS obliczona dla \p pattern przez \ref computeLPSArray
 *
 * @note Funkcja wyświetla krok po kroku stan dopasowania, włącznie z przesunięciem s = i - j.
 */
void KMPSearchSteps(const string &text, const string &pattern, int *lps) {
    int n = (int)text.size();
    int m = (int)pattern.size();

    cout << "Kroki wyszukiwania wzorca metoda KMP:\n";
    cout << "Tekst: " << text << "\nWzorzec: " << pattern << "\n\n";

    int i = 0; // Indeks w tekście
    int j = 0; // Indeks w wzorcu

    // Wyświetlamy stan początkowy (przesunięcie s = i - j = 0)
    printMatchState(text, pattern, i - j, j);

    while (i < n) {
        if (text[i] == pattern[j]) {
            // Znaki się pokrywają
            i++;
            j++;
            printMatchState(text, pattern, i - j, j);

            if (j == m) {
                // Pełne dopasowanie znalezione
                cout << "Dopasowanie znalezione na pozycji " << (i - j) << "\n\n";
                j = lps[j - 1];
                printMatchState(text, pattern, i - j, j);
            }
        } else {
            // Znaki się nie pokrywają
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

/**
 * @brief Funkcja główna programu – demonstruje działanie algorytmu KMP.
 *
 * Wewnątrz funkcji:
 * 1. Definiowany jest tekst oraz wzorzec,
 * 2. Alokowana jest tablica \p lps o rozmiarze równym długości wzorca,
 * 3. Wywoływana jest funkcja \ref computeLPSArray w celu obliczenia tablicy LPS,
 * 4. Wywoływana jest funkcja \ref KMPSearchSteps, aby pokazać kolejne kroki wyszukiwania,
 * 5. Zwalniana jest pamięć tablicy \p lps.
 *
 * @return Kod zakończenia programu (0 oznacza sukces).
 */
int main() {
    string text = "bacbabbaabab";   // Tekst, w którym szukamy
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
