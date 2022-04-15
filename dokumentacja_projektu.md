
# Antywirus dla systemu z jądrem Linux

## Eryk Muchorowski

## Spis treści

- [Dynamiczna i statyczna linkowalność](#dynamiczna-i-statyczna-linkowalność)
- [Algorytm funkcji skrótu](#algorytm-funkcji-skrótu)
- [Uruchomienia i wyłączanie aplikacji](#uruchomienia-i-wyłączanie-aplikacji)
- [Uprawnienia uruchomieniowe aplikacji](#uprawnienia-uruchomieniowe-aplikacji)
- [Statystyki wydajnościowe działania](#statystyki-wydajnościowe-działania)
- [Przypadki użycia](#przypadki-użycia)
- [Mechanizm kwarantanny](#mechanizm-kwarantanny)
- [Czystość kodu i praktyki programistyczne](#czystość-kodu-i-praktyki-programistyczne)
- [Statyczna analiza kodu](#statyczna-analiza-kodu)
- [Testowanie aplikacji](#testowanie-aplikacji)
- [Unikanie błędów bezpieczeństwa](#unikanie-błędów-bezpieczeństwa)


## Dynamiczna i statyczna linkowalność
Przy rozważaniu linkowalności bibliotek należy zwrócić uwagę na dwa aspekty:
1. Rozmiar pliku wykonywalnego po skompilowaniu
2. Wygoda użytkowania (user-friendly)

Celem znalezienia pewnego kompromisu zdecydowano, że wszystkie biblioteki **domyślnie** zainstalowane na systemie UNIX, będą linkowane dynamicznie - pozwoli to zdecydowanie zmniejszyć rozmiar pliku po skompilowaniu. W aplikacji użyto jednak jednej biblioteki zewnętrznej (OpenSSL), która domyślnie wymaga instalacji. Zalinkowana została zatem w sposób statyczny.
Rozmiar pliku po skompilowaniu: ok. 230kB.
![image](https://user-images.githubusercontent.com/100531644/163628829-f804a729-3020-4663-870e-2dfce56c0770.png)
Wygoda użytkownika: wystarczy pobrać program, załadować bazę danych i aplikacja jest gotowa do użytku.

## Algorytm funkcji skrótu
Przy wyborze funkcji skrótu rozważono trzy kluczowe aspekty:
1. Standardowe wybory twórców baz danych
2. Wydajność programu  
3. Aspekty bezpieczeństwa  

### Standardowe wybory twórców baz danych
Większość baz danych open-source znajdujących się w Internecie, korzysta głównie z trzech funkcji skrótu:
- MD5
- SHA-1
- SHA-256

Główną część stanowiła jednak teoretycznie przestarzała już funkcja **MD5**.

Przykładowe popularne bazy skrótów złośliwych programów:
- [Team Cymru](https://team-cymru.com/community-services/mhr/) - SHA-1, SHA-256, MD5  
- [Github](https://github.com/CYB3RMX/MalwareHashDB) - MD5  
- [Virusshare](https://virusshare.com/hashes) - MD5  

### Wydajność programu
W kwestii wydajnościowej zdecydowano się na sprawdzenie wydajności trzech wcześniej wspomnianych funkcji.  

**Założenia dotyczące testowania:**
- Testy przeprowadzone były przy pomocy narzędzia **OpenSSL** z poziomu Linuxa (jest to ta sama biblioteka, która używana jest w implementacji Antywirusa).
- Sprzęt, na którym przeprowadzono testy:
  - Procesor Intel Core i5-1135G7 2.40GHz 11. generacji
  - 8GB ramu wydzielona na wirtualną maszynę w środowisku VirtualBox
- Wykorzystano komendę 'openssl speed'. Testuje ona funkcje skrótu poprzez liczenie przez 3 sekundy kolejno bloki coraz większych rozmiarów  

Wyniki przeprowadzonych testów przedstawione są w kB/s:

| funkcja\blok[B] | 16        | 64         | 256        | 1024       | 8192       | 16384      |
|-----------------|-----------|------------|------------|------------|------------|------------|
| MD5             | 72029.19k | 178245.55k | 302214.06k | 372617.56k | 409523.54k | 416950.95k |
| SHA-1           | 81141.97k | 171779.63k | 311560.13k | 397080.58k | 399493.80k | 394575.87k |
| SHA-256         | 37970.48k | 82089.60k  | 128531.03k | 160539.65k | 178978.82k | 170716.90k |

Funkcje **SHA-1** oraz **MD5** cechują się podobną wydajnością, zaś **SHA-256** jest dwukrotnie wolniejsza pod względem przeliczonych bajtów na sekundę.

### Aspekty bezpieczeństwa
- **SHA-1** jak i **MD5** są już oficjalnie wycofane i nie powinny być używane w nowych aplikacjach
- **SHA-256** cały czas jest obowiązującym standardem
- W przypadku Antywirusa głównym celem funkcji skrótu jest identyfikacja pliku
- Kolizyjność funkcji w przypadku słabszego algorytmu, może prowadzić do tzw. False Positive, czyli sytuacji, w których teoretycznie bezpieczny plik zostanie uznany za niebezpieczny, gdyż jego skrót koliduje ze skrótem pewnego złośliwego oprogramowania.

### Decyzja projektowa - MD5
Biorąc pod uwagę wcześniej rozważone aspekty, w Antywirusie rolę funkcji skrótu będzie stanowić **MD5**. Wynika to z faktu, iż jest ona najbardziej powszechna w przypadku wykorzystania (dużo baz skrótów złośliwego oprogramowania open-source). Ponadto wypada ona bardzo
dobrze w testach wydajnościowych. Jej największą wadą jest kolizyjność, jednak poziom prawdopodobieństwa kolizji równy <img src="https://render.githubusercontent.com/render/math?math=1.47\cdot10^{-29}"> [(*Avira)](https://www.avira.com/en/blog/md5-the-broken-algorithm) jest zdecydowanie akceptowalny.

## Uruchomienia i wyłączanie aplikacji
Aplikacja uruchamiana jest za każdym razem na potrzebę wykonania jednego polecenia (jedno skanowanie - jedno uruchomienie). W przypadku chęci opuszczenia programu, możliwe jest wysłanie sygnału wyłączenia (Ctrl+C), które zostało obsłużone w odpowiedni sposób (aby zamknąć program w kontrolowany sposób i "wyczyścić" środowisko). Fragment kodu odpowiadający za zatrzymywanie aplikacji:
![image](https://user-images.githubusercontent.com/100531644/163629609-59c67fc3-e50b-45c3-b39d-d4f3523edcfb.png)

## Uprawnienia uruchomieniowe aplikacji
Aplikacja może być uruchomiona zarówno z perspektywy użytkownika, jak i roota. W zależności od wykonawcy programu, określana jest ścieżka dla niezbędnych plików/folderów programu. Są to:
1. /L_Antivirus - folder główny
2. /L_Antivirus/quarantine - folder będący kwarantanną
3. /L_Antivirus/list - lista poprzednich ścieżek plików będących na kwarantannie wraz z ich skrótami
4. /L_Antivirus/database - plik z bazą skrótów - musi zostać umieszczony w tej lokalizacji przez użytkownika, ważne jest zachowanie nazwy i formatu (dokument tekstowy, skróty w postacji hexadecymalnej - każdy w jednej linii).

Przy uruchomieniu programu odbywa się weryfikacja użytkownika, aby stworzyć foldery robocze w odpowiedniej lokalizacji:
![image](https://user-images.githubusercontent.com/100531644/163630036-ffbc8b0c-2e4e-444c-b1d1-f9038ebf6972.png)

Poziom uprawnień nie ma bezpośredniego wpływu na działania programu - jednak możliwe jest tylko i wyłącznie skanowanie plików i folderów, do których mamy uprawnienia (reszta jest pomijana - domyślnie uznawana za bezpieczne, bo i tak wykonawca programu nie może ich uruchomić).

Zaletą takiego podejścia jest bardzo duża uniwersalność (nie potrzebujemy uprawnień roota, aby przeskanować sobie pliki na komputerze). Ewentualnym problemem, który może się pojawić, są problemy w działaniu w pewnych szczególnych (choć rzadkich) przypadkach - np. gdyby użytkownik z jakiegoś powodu stracił uprawnienia do niezbędnych plików programu.
## Statystyki wydajnościowe działania
Program wyświetla statystyki w czesie rzeczywistym podczas skanowania rekursywnego katalogu. Wyświetlanie statystyk w przypadku innych operacji uznano za niepotrzebne - są one zazwyczaj bardzo szybkie (wyjątek - skanowanie bardzo dużego pliku, zostało to jednak uznane za zbyt rzadki przypadek).
Przykład wyżej opisanego mechanizmu (skanowanie katalogu /home):
![image](https://user-images.githubusercontent.com/100531644/163630464-ab5bc89e-7e8a-420a-bb3c-e8a3443f510f.png)

Ponadto po każdym skanowaniu program pokazuje liczbę niebezpiecznych plików oraz listuje je.

## Przypadki użycia
Antywirus ma działać w dwóch trybach:
- skanowanie wskazanego pliku za pomocą ścieżki absolutnej bądź relatywnej względem folderu, w którym uruchamiana jest aplikacja
- skanowanie wszystkich plików we wskazanym katalogu oraz rekursywnie we wszystkich podkatalogach

### Skanowanie pliku
W obecnej wersji program przyjmuje jako jedyny (obligatoryjny) argument ścieżkę do pliku. Następnie zaciągając rekordy z wcześniej
przygotowanego pliku, będącego *bazą danych skrótów*, sprawdza, czy skrót skanowanego pliku nie znajduje się w bazie. Możliwe są zatem trzy możliwe scenariusze wyniku działania programu:
- Informacja zwrotna, iż plik jest potencjalnie niebezpieczny
- Informacja zwrotna, iż plik nie został uznany za niebezpieczny
- Błąd, związany z nieznalezieniem podanego przez użytkownika pliku
## Mechanizm kwarantanny
## Czystość kodu i praktyki programistyczne
Podczas wytwarzania aplikacji korzystano z narzędzia IDE CLion, ułatwiającego dbanie o styl dzięki narzędziu *reformatowania kodu* oraz jego *refaktoryzacji*.
Ponadto przyjęte zostały następujące konwencje nazewnictwa (nie są one w 100% zgodne ze standardem C++, jednak kluczowym aspektem jest konsekwencja w oznaczeniach, która została zachowana):
- Projekt podzielony na mniejsze pliki, klasy
- Nazwy funkcji i zmiennych dokładnie opisują ich przeznaczenie
- nazwy zmiennych, plików dodatkowych - *underscore case*, nazwy funkcji - *mixed case*, nazwy klas, plików - *capital case*
- Optymalizacja importów, nie zanieczyszczanie przestrzeni nazw
- Komentarze umieszczanie przy nieoczywistych konstrukcjach

## Statyczna analiza kodu
Używane IDE CLion udostępnia kilka funkcji związanych ze statyczną analizą kodu. Jedną z nich
jest domyślny statyczny analizator kodu, drugą zaś narzędzie **Clang-Tidy**, przeprowadzające głębszą na poziomie
logicznym analizę. Diagnozuje on oraz sugeruje sposoby naprawy wielu typowych błędów programistycznych.

### Przykłady użycia wspomnianych narzędzi:

Fragment kodu uznany za niedokładny:
```cpp
bool HashesDatabase::isInDatabase(std::string digest) {
    return hashes.find(digest) != hashes.end();
}
```
Sugestia narzędzia **Clang-Tidy**

![](https://github.com/Ixico/BSO-projekt/blob/master/Documentation%20resources/clang_tidy.png)

Kod po zastosowaniu sugestii narzędzia **Clang-Tidy**:
```cpp
bool HashesDatabase::isInDatabase(const std::string& digest) {
    return hashes.find(digest) != hashes.end();
}
```
## Testowanie aplikacji
Aplikacja została w całości stworzona w języku **C++** z użyciem systemu budowania **CMake** oraz **make**.
Działa ona w przestrzeni użytkownika. Wszystkie testy przeprowadzone są na maszynie wirtualnej **Ubuntu 20.04 LTS**.
Do kompilowania użyto kompilatora **g++**. Po stworzeniu pliku uruchomieniowego program działa według wcześniej postawionych
założeń.

### Przykład działania programu
*hash_database.txt*
```
0cc175b9c0f1b6a831c399e269772661
92eb5ffee6ae2fec3ad71c777531578f
0949f7eb1f66dad39d488d5d22531166
8277e0910d750195b448797616e091ad
```
*safe_file.txt*
```
hello world
```
*dangerous_file.txt*
```
goodbye world
```

### Wszystkie możliwe scenariusze uruchomienia programu
![](https://github.com/Ixico/BSO-projekt/blob/master/Documentation%20resources/basic_test.png)

## Unikanie błędów bezpieczeństwa
Proces unikania błędów bezpieczeństwa składał się z bardzo wielu kroków:
1. Statyczna analiza kodu przy użyciu wbudowanych narzędzi w IDE CLion (w szczególności **Clang-Tidy**)
2. Przeprowadzenie testów użytkowych, celowego wprowadzania złego inputu, testów funkcjonalnych
3. Dynamiczna analiza kodu - sprawdzenie działania
4. Obsługa błędów poprzez mechanizm wyjątków
