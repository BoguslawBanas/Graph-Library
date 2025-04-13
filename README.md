# Licencjat
Biblioteka do obsługi algorytmów grafowych

## Ogólne założenia i pojęcia

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
Biblioteka zawiera trzy rodzaje kopców:
- Kopiec binarny (BinaryHeap)
- Kopiec dwumianowy (BinomialHeap)
- Kopiec Fibonacciego (FibonacciHeap)

Każda z tych klas udostępnia zestaw metod:

| Nazwa metody   | Typ zwracany | Opis                                               | Czy metoda modyfikuje kopiec? |
|---------------|-------------|----------------------------------------------------|------------------------------|
| isEmpty()     | bool        | Sprawdza, czy kopiec jest pusty.                  | ❌                           |
| getSize()     | uint32_t         | Zwraca liczbę elementów w kopcu.                   | ❌                           |
| getMin()      | std::pair<uint32_t, N>   | Zwraca najmniejszy element w kopcu.                | ❌                           |
| extractMin()  | uint32_t   | Usuwa i zwraca najmniejszy element z kopca.        | ✅                           |
| insert(const uint32_t, const N&)      | HeapNode        | Wstawia nowy element do kopca.                     | ✅                           |
| decreaseKey(HeapNode*, const N&) | void        | Zmniejsza wartość klucza dla danego elementu.      | ✅                           |

HeapNode to specjalna klasa, której instancja jest zwracana użytkownikowi przy wyłowaniu metody 'insert', która wystawia dwie publiczne metody:
- getPtr(): metoda zwraca wskaźnik na void, który wskazuje na komórkę w pamięci, gdzie znajduje się komórka umiejscowiona w kopcu.
- getKey(): metoda zwraca klucz.

## Kolejka priorytetowa
Kolejka priorytetowa stanowi fasadę dla kopców. Dodatkowo klasa ta posiada pole std::vector, który przyjmuje instancje klasy HeapNode is służy do wyszukiwania w czasie stałym komórek pamięci, które są umieszczone w danych kopcach. Nazwy metod są analogiczne jak w przypadku klasy std::priority_queue (jedyna ważna różnica jest taka, że przy wywołaniu metody insert(), jest sprawdzane czy w kopcu znajduje się już węzeł z takim kluczem, jeżeli tak to zamiast wstawienia, wykonywana jest operacja zmniejszenia klucza).

## Zbiory rozłączne
Ostatnim ważnym elementem biblioteki jest struktura zbiorów rozłącznych. Są one używane przy algorytmie Kruskala, który buduje z podanego grafu minimalne drzewo rozpinające. Klasa wystawia trzy metody:

| Nazwa metody   | Typ zwracany | Opis                                               |
|---------------|-------------|----------------------------------------------------|
| find()     | uint32_t        | Operacja zwraca indeks zbioru do jakiego należy podany w argumencie nr wierzchołka.           |
| unionize()    | void         | Operacja wywołuje metodę find() dla obu argumentów i łączy dwa wierzchołki do jednego zbioru.                   |
| unionize_f()      | void   | Operacja łączy dwa wierzchołki do jednego zbioru (bez wywołania metody find (są one od razu podane w argumencie metody)).              |

## Algorytmy


