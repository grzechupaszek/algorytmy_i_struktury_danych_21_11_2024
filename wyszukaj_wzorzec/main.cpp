/**
 * @file
 * @brief Przykład wyszukiwania wzorca w tekście za pomocą różnych algorytmów.
 *
 * W pliku przedstawiono trzy metody wyszukiwania wzorca w tekście:
 * - Algorytm naiwny,
 * - Algorytm Rabina-Karpa,
 * - (Wspomniany, lecz wykomentowany: automat skończony - brak implementacji w tym kodzie).
 *
 * \n\n
 * Kod ilustruje sposób implementacji podstawowych podejść do problemu 
 * wyszukiwania łańcucha wzorca w tekście.  
 * Używany przykładowy tekst i wzorzec:
 * - text = "abcabaabcabac"
 * - pattern = "aba"
 */

#include <iostream>
#include <string>
using namespace std;

/**
 * @brief Wyszukuje wzorzec w tekście metodą naiwną.
 *
 * Porównuje wzorzec ze wszystkimi możliwymi podciągami tekstu (po kolei) 
 * i w przypadku pełnej zgodności wypisuje znalezioną pozycję.
 *
 * @param text     Tekst, w którym szukamy
 * @param pattern  Wzorzec, którego szukamy
 *
 * @note Złożoność w najgorszym przypadku to O(n*m), gdzie:
 *       - n = długość tekstu,
 *       - m = długość wzorca.
 */
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

/**
 * @brief Wyszukuje wzorzec w tekście metodą Rabina-Karpa.
 *
 * Algorytm Rabina-Karpa wykorzystuje tzw. hashowanie „okienkowe” (rolling hash).
 * Przyspiesza porównywanie podciągów tekstu z wzorcem, 
 * jednak w przypadku kolizji (te same wartości hash) 
 * konieczne jest dodatkowe sprawdzenie znaków.
 *
 * @param text     Tekst, w którym szukamy
 * @param pattern  Wzorzec, którego szukamy
 * @param d        Baza systemu liczbowego (zwykle 256 dla ASCII)
 * @param q        Liczba pierwsza (zapobiegająca nadmiernym kolizjom)
 *
 * ### Wyjaśnienie działania:
 * - Obliczany jest hash wzorca (p) oraz hash pierwszego podciągu tekstu (t),
 * - Następnie dla każdego kolejnego „okna” w tekście obliczany jest nowy hash (t),
 * - Jeśli hashe się zgadzają, porównujemy znak po znaku (aby uniknąć fałszywych trafień),
 * - Gdy wzorzec zostaje znaleziony, wypisywana jest pozycja w tekście.
 *
 * @note Złożoność w przeciętnym przypadku to O(n + m). 
 *       W najgorszym przypadku może być O(n*m), jeśli występuje wiele kolizji.
 */
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

/**
 * @brief Punkt wejścia do programu.
 *
 * W funkcji main:
 * 1. Definiowany jest przykładowy tekst i wzorzec,
 * 2. Wywoływany jest algorytm naiwny,
 * 3. Następnie wywoływany jest algorytm Rabina-Karpa (z parametrami `d=256` i `q=101`),
 * 4. Wyświetlane są informacje o znalezionych pozycjach wzorca w tekście.
 *
 * @return Kod zakończenia (0 oznacza sukces).
 */
int main() {
    string text = "abcabaabcabac";
    string pattern = "aba";

    cout << "Algorytm naiwny:" << endl;
    naiveSearch(text, pattern);

    cout << "\nAlgorytm Rabina-Karpa:" << endl;
    rabinKarp(text, pattern, 256, 101); // d = 256 (dla ASCII), q = 101 (liczba pierwsza)

    return 0;
}

/**
 * @mainpage Dokumentacja - Algorytmy wyszukiwania wzorca
 * 
 * W tej dokumentacji znajdują się opisy funkcji implementujących:
 * - Wyszukiwanie **naiwne** (funkcja \ref naiveSearch),
 * - Wyszukiwanie **Rabin-Karp** (funkcja \ref rabinKarp).
 * 
 * \n
 * Aby zobaczyć kod, przejdź do pliku 
 * \ref file "z przykładowym kodem i dokumentacją".
 *
 * \n\n
 * ### Dodatkowe wyjaśnienie do Rabina-Karpa:
 * - Wykorzystuje hashowanie, aby przyspieszyć porównywanie podciągów tekstu z wzorcem,
 * - d = baza systemu liczbowego (np. 256 dla ASCII),
 * - q = liczba pierwsza, która zmniejsza prawdopodobieństwo kolizji hashów,
 * - Kiedy kolizja się zdarzy (hash podciągu tekstu == hash wzorca), dopiero wtedy sprawdzamy 
 *   dokładnie znaki, aby wykluczyć fałszywe trafienie.
 */
