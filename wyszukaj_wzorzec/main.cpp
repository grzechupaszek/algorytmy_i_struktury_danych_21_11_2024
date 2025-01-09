//wyszukiwanie wzorca - algorytm naiwny 
//Algorytm Rabina Karpa
// Automat skończony

#include <iostream>
#include <string>
using namespace std;

void naiveSearch(const string& text, const string& pattern) {
    int n = text.size();     // Długość tekstu
    int m = pattern.size();  // Długość wzorca

    // Przechodzimy przez tekst, przesuwając wzorzec
    for (int i = 0; i <= n - m; i++) {
        bool found = true;

        // Porównujemy wzorzec z bieżącym podciągiem tekstu
        for (int j = 0; j < m; j++) {
            if (text[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }

        // Jeśli znaleziono wzorzec, wypisujemy pozycję
        if (found) {
            cout << "Znaleziono wzorzec (naiwny) na " << i << " literce" << endl;
        }
    }
}

void rabinKarp(const string& text, const string& pattern, int d, int q) {
    int n = text.size();     // Długość tekstu
    int m = pattern.size();  // Długość wzorca

    int h = 1; // Wartość d^(m-1) mod q
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    int p = 0; // Hash wzorca
    int t = 0; // Hash bieżącego podciągu tekstu

    // Obliczenie hasha wzorca i pierwszego podciągu tekstu
    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    // Przesuwamy wzorzec po tekście
    for (int i = 0; i <= n - m; i++) {
        // Jeśli hash pasuje, sprawdzamy znaki
        if (p == t) {
            bool found = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                cout << "Znaleziono wzorzec (Rabin-Karp) na " << i << " literce" << endl;
            }
        }

        // Obliczenie hasha dla kolejnego podciągu tekstu
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            // Upewniamy się, że hash jest dodatni
            if (t < 0) {
                t += q;
            }
        }
    }
}

int main() {
    string text = "abcabaabcabac";
    string pattern = "aba";

    cout << "Algorytm naiwny:" << endl;
    naiveSearch(text, pattern);

    cout << "\nAlgorytm Rabina-Karpa:" << endl;
    rabinKarp(text, pattern, 256, 101); // d = 256 (dla ASCII), q = 101 (liczba pierwsza)

    return 0;
}
/*rabinKarp - wyjaśnienie:
Wykorzystuje hashowanie, aby przyspieszyć porównywanie podciągów tekstu z wzorcem.
d – baza systemu liczbowego 256 dla ASCII.
q – liczba pierwsza zapobiega kolizjom hashów.
Oblicza hash wzorca i pierwszego podciągu tekstu.
Porównuje hashe, a w przypadku zgodności dodatkowo znaki, aby uniknąć fałszywego trafienia.
*/