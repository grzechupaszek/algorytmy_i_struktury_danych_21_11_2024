#include <iostream>
#include <string>

using namespace std;

/**
 * @brief Funkcja toLowerCase
 * 
 * Zwraca nowy łańcuch znaków, w którym wszystkie litery z przedziału 'A'..'Z' 
 * zostały przekształcone w odpowiadające im małe litery z przedziału 'a'..'z'.
 * 
 * @param s Oryginalny łańcuch wejściowy
 * @return Nowy łańcuch, w którym wszystkie wielkie litery zamieniono na małe
 */
string toLowerCase(const string& s) {
    string result = s; // Kopiujemy oryginalny łańcuch
    for (int i = 0; i < (int)result.size(); i++) {
        // Jeśli znak jest w przedziale 'A'..'Z', to zamieniamy na 'a'..'z'
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] - 'A' + 'a';
        }
    }
    return result;
}

/**
 * @brief Funkcja buildDFA
 * 
 * Buduje automat skończony (DFA – ang. deterministic finite automaton) dla zadanego wzorca.
 * Tablica DFA ma rozmiar (m+1) x 256, gdzie m = długość wzorca, a 256 to rozmiar
 * alfabetu (zakładamy standardowe znaki ASCII).
 * 
 * @param pattern Łańcuch znaków będący wzorcem do wyszukiwania
 * @return Zwraca wskaźnik na dynamicznie alokowaną tablicę dwuwymiarową reprezentującą DFA
 * 
 * @note Należy pamiętać o późniejszym zwolnieniu zaalokowanej pamięci.
 */
int** buildDFA(const string& pattern) {
    const int ALPHABET_SIZE = 256; // Zakładamy ASCII
    int m = pattern.size();

    // Alokacja dynamiczna dfa: (m+1) wierszy, każdy wiersz ma ALPHABET_SIZE kolumn.
    int** dfa = new int*[m + 1];
    for (int i = 0; i <= m; i++) {
        dfa[i] = new int[ALPHABET_SIZE];
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            dfa[i][c] = 0; // inicjalizacja
        }
    }

    // Podstawowa inicjalizacja:
    // Z stanu 0, po wczytaniu pierwszego znaku pattern[0], przechodzimy do stanu 1.
    dfa[0][(unsigned char)pattern[0]] = 1;

    // fallback - stan, do którego cofa się automat przy niedopasowaniu
    int fallback = 0;

    // Wypełnianie dfa dla kolejnych stanów (1..m)
    for (int state = 1; state <= m; state++) {
        // Przepisujemy przejścia ze stanu "fallback"
        for (int c = 0; c < ALPHABET_SIZE; c++) {
            dfa[state][c] = dfa[fallback][c];
        }
        // Ustawiamy przejście dla kolejnego znaku wzorca (jeśli state < m)
        if (state < m) {
            dfa[state][(unsigned char)pattern[state]] = state + 1;
            // Aktualizujemy fallback
            fallback = dfa[fallback][(unsigned char)pattern[state]];
        }
    }

    return dfa;
}

/**
 * @brief Funkcja searchWithDFA
 * 
 * Wyszukuje wystąpienia zadanego wzorca w danym tekście przy pomocy uprzednio
 * zbudowanego automatu (DFA).
 * 
 * @param text Tekst (łańcuch znaków), w którym dokonujemy wyszukiwania
 * @param pattern Wzorzec (łańcuch znaków), którego szukamy
 * @param dfa Wskaźnik na tablicę przejść automatu zbudowaną przez buildDFA()
 * @return Liczba znalezionych wystąpień wzorca w tekście
 * 
 * @note Zależnie od tego, czy pattern i text są znormalizowane (np. do małych liter),
 *       wyszukiwanie może być wrażliwe lub niewrażliwe na wielkość liter.
 */
int searchWithDFA(const string& text, const string& pattern, int** dfa) {
    int m = pattern.size();
    int n = text.size();

    int occurrences = 0;  // licznik wystąpień
    int state = 0;        // stan automatu

    for (int i = 0; i < n; i++) {
        // Przechodzimy do kolejnego stanu na podstawie znaku text[i]
        state = dfa[state][(unsigned char)text[i]];

        // Jeśli osiągnęliśmy stan m, to znaczy, że wzorzec został dopasowany
        if (state == m) {
            occurrences++;
            cout << "Znaleziono wystapienie wzorca na pozycji: " 
                 << (i - m + 1) << endl;
        }
    }
    return occurrences;
}

/**
 * @brief Funkcja searchCaseInsensitive
 * 
 * Wykonuje wyszukiwanie wzorca w tekście niewrażliwe na wielkość liter.
 * 1. Konwertuje tekst i wzorzec na małe litery,
 * 2. Buduje dla tak przetworzonego wzorca automat (DFA),
 * 3. Uruchamia wyszukiwanie w przetworzonym tekście,
 * 4. Zwraca liczbę znalezionych wystąpień.
 * 
 * @param text Oryginalny tekst, w którym chcemy wyszukiwać (może zawierać wielkie/male litery)
 * @param pattern Oryginalny wzorzec do wyszukiwania
 * @return Liczba dopasowań wzorca w tekście (ignorując wielkość liter)
 * 
 * @note Wewnątrz funkcji następuje zwolnienie pamięci zaalokowanej dla tablicy dfa.
 */
int searchCaseInsensitive(const string& text, const string& pattern) {
    // 1. Konwersja na małe litery
    string lowerText = toLowerCase(text);
    string lowerPattern = toLowerCase(pattern);

    // 2. Budujemy automat dla "obniżonego" wzorca
    int** dfa = buildDFA(lowerPattern);

    // 3. Wyszukujemy w "obniżonym" tekście
    int found = searchWithDFA(lowerText, lowerPattern, dfa);

    // Zwolnienie pamięci dfa
    int m = lowerPattern.size();
    for (int i = 0; i <= m; i++) {
        delete[] dfa[i];
    }
    delete[] dfa;

    return found;
}

/**
 * @brief Funkcja main
 * 
 * Główna funkcja programu, w której:
 * 1. Następuje wczytanie wzorca oraz tekstu od użytkownika,
 * 2. Użytkownik określa, czy chce wyszukiwać niewrażliwie na wielkość liter,
 * 3. Uruchamiany jest odpowiedni tryb wyszukiwania (case-sensitive lub case-insensitive),
 * 4. Wyświetlana jest liczba znalezionych dopasowań.
 * 
 * @return Kod zakończenia programu (0 oznacza sukces).
 */
int main() {
    // Wczytujemy wzorzec i tekst
    string pattern, text;
    cout << "Podaj wzorzec: ";
    getline(cin, pattern);
    cout << "Podaj tekst: ";
    getline(cin, text);

    // Czy użytkownik chce wyszukiwać niewrażliwie na wielkość liter?
    char c;
    cout << "Czy wyszukiwać niewrażliwie na wielkość liter? (t/n): ";
    cin >> c;

    if (c == 't' || c == 'T') {
        // Tryb case-insensitive
        int found = searchCaseInsensitive(text, pattern);
        cout << "Liczba znalezionych wystapien (niewrazliwie na wielkosc liter): " 
             << found << endl;
    } else {
        // Tryb case-sensitive (oryginalny)
        // Budujemy dfa na oryginalnym wzorcu
        int** dfa = buildDFA(pattern);
        // Szukamy w oryginalnym tekście
        int found = searchWithDFA(text, pattern, dfa);
        cout << "Liczba znalezionych wystapien (wrazliwe na wielkosc liter): " 
             << found << endl;

        // Zwalniamy pamięć
        int m = pattern.size();
        for (int i = 0; i <= m; i++) {
            delete[] dfa[i];
        }
        delete[] dfa;
    }

    return 0;
}
