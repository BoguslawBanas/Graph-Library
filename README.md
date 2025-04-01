# Licencjat
Biblioteka do obsługi algorytmów grafowych

## Ogólne założenia

## Grafy
Bilbioteka udostępnia użytkownikowi 8 rodzajów grafów, cztery z nich są reprezentowane za pomocą struktury list sąsiedztwa:
- ListGraph: nieskierowany graf bez wag.
- ListGraphDirected: skierowany graf bez wag.
- ListGraphWeighted: nieskierowany graf ważony.
- ListGraphDirectedAndWeighted: skierowany graf ważony.

Pozostałe cztery grafy są reprezentowane za pomocą struktury macierzy:
- MatrixGraph: nieskierowany graf bez wag.
- MatrixGraphDirected: skierowany graf bez wag.
- MatrixGraphWeighted: nieskierowany graf ważony.
- MatrixGraphDirectedAndWeighted: skierowany graf ważony.

Każda ze struktur wystawia użytkownikowi zestaw metod, które służą do tworzenia, modyfikacji lub pozyskiwania informacji z obecnego stanu w jakim znajduje się wcześniej stworzony graf.

Dla grafów (zarówno bez, jak i z wagami) zestaw metod wygląda następująco:

| Nazwa metody          | Typ zwracany | Opis                                                        | Czy metoda modyfikuje graf? |
|-----------------------|-------------|-------------------------------------------------------------|-----------------------------|
| isDirected()         | bool        | Sprawdza, czy graf jest skierowany.                         | ❌                          |
| isWeighted()         | bool        | Sprawdza, czy graf jest ważony.                             | ❌                          |
| getSize()            | uint32_t        | Zwraca liczbę wierzchołków w grafie.                        | ❌                          |
| areVertexesConnected() | bool      | Sprawdza, czy dwa wierzchołki są połączone krawędzią.       | ❌                          |
| transpose()          | void        | Tworzy transpozycję grafu (zamienia kierunki krawędzi).     | ✅                          |
| addVertex()         | uint32_t        | Dodaje nowy wierzchołek do grafu.                           | ✅                          |
| deleteEdge()        | void        | Usuwa krawędź z grafu.                                      | ✅                          |
| getNeighbours()      | const std::vector<uint32_t>    | Zwraca listę sąsiadów danego wierzchołka.                   | ❌                          |

Zestaw metod zarezerwowanych tylko dla grafów bez wag:

| Nazwa metody       | Typ zwracany | Opis                                      | Czy metoda modyfikuje graf? |
|--------------------|-------------|-------------------------------------------|-----------------------------|
| addEdge()         | void        | Dodaje krawędź między dwoma wierzchołkami. | ✅                          |
| getListOfEdges()  | const std::vector<uint32_t>        | Zwraca listę wszystkich krawędzi w grafie. | ❌                          |

Zestaw metod zarezerwowanych tylko dla grafów ważonych (N jest traktowany jako typ generyczny):

| Nazwa metody              | Typ zwracany | Opis                                                   | Czy metoda modyfikuje graf? |
|---------------------------|-------------|--------------------------------------------------------|-----------------------------|
| getMax()                 | N         | Zwraca wartość podaną w konstruktorze, która reprezentuje nieskończoność w danym grafie. | ❌                          |
| getNeighboursWithWeights() | const std::vector<std::pair<uint32_t,N>>        | Zwraca listę sąsiadów wraz z wagami krawędzi.          | ❌                          |
| addEdge()                | void        | Dodaje krawędź, wraz z wagą między dwoma wierzchołkami.             | ✅                          |
| getWeight()              | N   | Zwraca wagę krawędzi między dwoma wierzchołkami.       | ❌                          |
| getListOfEdges()         | const std::vector<std::pair<std::pair<uint32_t, uint32_t>,N>>        | Zwraca listę wszystkich krawędzi w grafie wraz z ich wagami.             | ❌                          |

## Kopce

## Kolejka priorytetowa

## Zbiory rozłączne

## Algorytmy

## Conclusion
Markdown is widely used for documentation, README files, and content formatting in various platforms. It provides an easy-to-read and easy-to-write syntax.
