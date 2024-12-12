# algorytmy_i_struktury_danych_21_11_2024
c++ 
#Funkcje hashujace, implementacje !


# KMP

c++

#Kontekst:
Algorytm KMP służy do wyszukiwania wzorca (pattern) w tekście (text) w czasie liniowym, bez powtarzającego się wracania do początku wzorca przy każdym niedopasowaniu. Kluczowym elementem algorytmu jest wstępne obliczenie tablicy LPS (Longest Prefix Suffix), zwanej też tabelą π (pi), która pozwala uniknąć powtórnej analizy znaków, które już zostały dopasowane.

Założenia w kodzie:

    text to ciąg znaków, w którym szukamy wzorca.
    pattern to wzorzec, który chcemy odnaleźć w tekście.
    Tablica lps (długości pattern.size()) będzie przechowywać informację o najdłuższym właściwym prefiksie wzorca, który jest też jego sufiksem, dla każdej pozycji we wzorcu.

Funkcja: computeLPSArray()

Cel: Obliczenie tablicy LPS dla zadanego wzorca.
Dzięki tablicy LPS, dla każdej pozycji i we wzorcu znamy długość najdłuższego prefiksu, który jest równocześnie sufiksem podłańcucha pattern[0..i].

Parametry:

    pattern: wzorzec, dla którego obliczamy LPS.
    lps: wskaźnik na tablicę typu int, w której przechowamy obliczone wartości LPS.

Logika:

    Inicjujemy lps[0] = 0, ponieważ najdłuższy właściwy prefiks-sufiks dla pierwszego znaku jest zerowy (nie ma krótszego podłańcucha przed pierwszym znakiem).
    Używamy dwóch wskaźników:
        i – iteruje po kolejnych znakach wzorca,
        length – przechowuje aktualną długość najdłuższego dopasowania prefiks-sufiks.
    Startujemy z i = 1 i length = 0.
    Jeśli pattern[i] == pattern[length], to znaczy, że możemy rozszerzyć aktualne dopasowanie prefiks-sufiks, więc:
        Zwiększamy length o 1
        lps[i] = length
        Przechodzimy do następnego znaku, i++
    Jeśli pattern[i] != pattern[length], mamy niedopasowanie:
        Jeśli length != 0, to znaczy, że możemy spróbować krótszego prefiksu, czyli length = lps[length-1] i ponownie sprawdzamy dopasowanie.
        Jeśli length == 0, oznacza to, że nie mamy żadnego pasującego prefiksu, więc lps[i] = 0 i i++.

W trakcie działania, kod wypisuje stan indeksów, wartości length oraz aktualną zawartość lps, aby przypominać to, co jest pokazane na slajdach.

W efekcie powstaje tablica LPS, która dla każdej pozycji i we wzorcu mówi, jaki jest najdłuższy prefiks będący również sufiksem do tego miejsca.
Funkcja: KMPSearchSteps()

Cel:
Przedstawienie krok po kroku dopasowania wzorca do tekstu z wykorzystaniem tablicy LPS.
Zamiast po prostu zwrócić indeks znalezienia wzorca, funkcja ta wypisuje szczegółowo, jaki jest stan dopasowania (ile znaków pasuje, jaki jest aktualny shift s = i - j oraz długość dopasowania q = j).

Parametry:

    text – tekst, w którym szukamy.
    pattern – wzorzec, którego szukamy.
    lps – tablica LPS obliczona dla pattern.

Zmienne używane w logice:

    i – indeks w tekście (porusza się do przodu)
    j – indeks we wzorcu (mówi, ile znaków aktualnie się dopasowało)
    s = i - j – efektywny shift, czyli o ile przesunięty jest wzorzec względem tekstu.
    q = j – w prezentacjach zwykle używa się q jako długości aktualnie dopasowanego prefiksu wzorca. Tutaj j spełnia tę samą rolę.

Logika działania:

    Na początku i = 0, j = 0. To znaczy, nie dopasowaliśmy jeszcze żadnego znaku, a wzorzec zaczyna porównywać się z tekstem od pierwszego znaku.
    Za każdym razem funkcja printMatchState wypisuje aktualne położenie wzorca względem tekstu (s i q), co odpowiada diagramom na slajdach.
    Główna pętla:
        Sprawdzamy, czy text[i] == pattern[j].
            Jeśli tak: zwiększamy i i j, co oznacza, że dopasowaliśmy kolejny znak wzorca. Wypisujemy stan.
            Jeśli j osiągnie m (długość wzorca), to znaczy, że znaleźliśmy pełne dopasowanie.
            Wypisujemy informację o znalezieniu dopasowania i korzystając z lps[j-1] skracamy j, aby od razu móc szukać następnego dopasowania w tekście bez powracania do początku wzorca.
        Jeśli znaki nie pasują (text[i] != pattern[j]):
            Jeśli j > 0, to znaczy, że mamy już pewien dopasowany prefiks. Nie chcemy tracić tej informacji, więc ustawiamy j = lps[j-1]. To ruch kluczowy w KMP – nie cofamy się w tekście, a jedynie modyfikujemy j, korzystając z tablicy LPS, aby wzorzec przesunąć tak, by wykorzystać już dopasowane znaki.
            Jeśli j == 0, brak częściowego dopasowania, więc po prostu przesuwamy się o jeden w tekście, i++, próbując dopasować wzorzec od nowa.

Każdy krok jest wypisywany, dzięki czemu możemy prześledzić dokładnie, jak q (czyli j) i s (czyli i-j) zmieniają się w czasie, tak jak na slajdach.
Pozostałe elementy:

Funkcja printLPSState():
Wyświetla stan tablicy LPS w trakcie jej budowania – obecny indeks i, aktualną wartość length oraz wartości w lps.

Funkcja printMatchState():
Wyświetla aktualny stan dopasowania tekstu i wzorca:

    Na której pozycji w tekście i wzorcu jesteśmy,
    Jaki jest shift s oraz q (długość dopasowania).

Ta wizualizacja pozwala zobaczyć dokładnie to, co widać na prezentacji, czyli tekst i przesunięty względem niego wzorzec, wraz z informacją o tym, ile znaków wzorca już pasuje.
Podsumowanie logiki algorytmu KMP:

    Preprocessing (LPS):
    Najpierw obliczamy tablicę LPS na podstawie wzorca. Dla każdej pozycji we wzorcu wiemy, jaki jest najdłuższy prefiks będący jednocześnie sufiksem. Pozwala to przy nieudanym dopasowaniu uniknąć powrotu do początku wzorca, a zamiast tego – przeskoczyć do miejsca, o którym wiemy, że może dopasować się dalej.

    Wyszukiwanie:
    Porównujemy znaki tekstu i wzorca. Gdy dopasowanie się powiększa, przesuwamy oba indeksy do przodu. Gdy napotkamy niedopasowanie, wykorzystujemy tablicę LPS do znalezienia kolejnej sensownej pozycji we wzorcu bez cofania się w tekście.

    Złożoność:
    Dzięki tablicy LPS i inteligentnemu zarządzaniu niedopasowaniami, złożoność jest liniowa: O(n + m), gdzie n to długość tekstu, a m to długość wzorca.

Dzięki temu kodowi wraz z komentarzami i dodatkowymi wydrukami stanów, możemy "zobaczyć" algorytm KMP w działaniu, krok po kroku, w sposób zbliżony do tego, co jest pokazane na slajdach w prezentacji.

