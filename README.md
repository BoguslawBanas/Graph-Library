# Licencjat
Biblioteka do obsługi algorytmów grafowych

## Wyjaśnienie skrótów
N - typ generyczny zmiennej reprezentującej wartość liczbową.
G - typ generyczny zmiennej reprezentującej graf.
F - typ generyczny reprezentujący funktor.

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
| decreaseValue(HeapNode*, const N&) | void        | Zmniejsza wartość dla danej komórki znajdującej się w kopcu.      | ✅                           |

HeapNode to specjalna klasa, której instancja jest zwracana użytkownikowi przy wyłowaniu metody 'insert', która wystawia jeden akcesor:
- getPtr(): metoda zwraca wskaźnik na void, który wskazuje na komórkę w pamięci, gdzie znajduje się komórka umiejscowiona w kopcu.

Oprócz wyżej wymienionych metod, każda z trzech klas reprezentujących kopce dodatkowo wystawia metodę unionize(), która jako argument przyjmuje wskażnik do kopca (tego samego typu) i służy do złączania dwóch kopców. Po złączeniu następuje usunięcie kopca, który został podany w argumencie.

## Kolejka priorytetowa
Kolejka priorytetowa stanowi fasadę dla kopców. Dodatkowo klasa ta posiada pole std::vector, który przyjmuje instancje klasy HeapNode is służy do wyszukiwania w czasie stałym komórek pamięci, które są umieszczone w danych kopcach. Nazwy metod są analogiczne jak w przypadku klasy std::priority_queue (jedyna ważna różnica jest taka, że przy wywołaniu metody push(), jest sprawdzane czy w kopcu znajduje się już węzeł z takim kluczem, jeżeli tak to zamiast wstawienia, wykonywana jest operacja zmniejszenia klucza).

## Zbiory rozłączne
Ostatnim ważnym elementem biblioteki jest struktura zbiorów rozłącznych. Są one używane przy algorytmie Kruskala, który buduje z podanego grafu minimalne drzewo rozpinające. Klasa wystawia trzy metody:

| Nazwa metody   | Typ zwracany | Opis                                               |
|---------------|-------------|----------------------------------------------------|
| find()     | uint32_t        | Operacja zwraca indeks zbioru do jakiego należy podany w argumencie nr wierzchołka.           |
| unionize()    | void         | Operacja wywołuje metodę find() dla obu argumentów i łączy dwa wierzchołki do jednego zbioru.                   |
| unionize_f()      | void   | Operacja łączy dwa wierzchołki do jednego zbioru (bez wywołania metody find (są one od razu podane w argumencie metody)).              |

## Algorytmy
Na sam koniec znajduje się lista algorytmów jakie zostały zaimplementowane w bibliotece. Listę tą można podzielić na:

### Badanie spójności grafu
Zostały zaimplementowane dwa algorytmy badające [spójność grafu](https://pl.wikipedia.org/wiki/Graf_spójny):

| Nazwa funkcji | Typ zwracany | Opis                                                                 |
|---------------|---------------|----------------------------------------------------------------------|
| isWeaklyConnected()       | bool | Funkcja sprawdza czy graf jest słabo spójny. |
| isStronglyConnected()     | bool | Funkcja sprawdza czy graf jest silnie spójny. |

Dla grafów nieskierowanych obie funkcje zawsze zwrócą dokładnie taką samą wartość. W przypadku grafów skierowanych graf jest słabo spójny, jeżeli jego pochodny graf nieskierowany jest spójny. Graf skierowany silnie spójny to taki graf, który dla każdej pary wierzchołków (u,v) istnieje ścieżka z u do v.

### Badanie dwudzielności grafu
Funckja isBipartite() przyjmuje w argumencie graf, sprawdza czy graf jest [dwudzielny](https://pl.wikipedia.org/wiki/Graf_dwudzielny) i zwraca wartość prawda/fałsz.

### Przeszukiwanie wszerz

| Nazwa funkcji | Typ zwracany | Opis                                                                 |
|---------------|---------------|----------------------------------------------------------------------|
| bfs(const G&, const uint32_t, const uint32_t)  | int32_t | Algorytm przechodzenia wszerz, który zwraca informację o odległości między dwoma wierzchołkami. |
| bfs(const G&, const uint32_t, const std::vector<uint32_t>&) | std::unordered_map<uint32_t, int32_t>* | Algorytm przechodzenia wszerz, który zwraca informację o odległości między jednym wierzchołkiem, a zbiorem wierzchołków. |
| bfs(const G&, const uint32_t) | std::vector<int32_t>* | Algorytm przechodzenia wszerz, który zwraca informację o odległości między wierzchołkiem startowym, a wszystkimi wierzchołkami w grafie. |
| bfs_with_f(const G&, const uint32_t, const uint32_t, const F&) | int32_t | Algorytm przechodzenia wszerz, który zwraca informację o odległości między dwoma wierzchołkami, wraz ze "instrukcjami" dot. przechodzenia po grafie. |
| bfs_with_f(const G&, const uint32_t, const std::vector<uint32_t>&, const F&) | std::unordered_map<uint32_t, int32_t>* | Algorytm przechodzenia wszerz, który zwraca informację o odległości między jednym wierzchołkiem, a zbiorem wierzchołków, wraz z "instrukcjami" dot. przechodzenia po grafie. |
| bfs_with_f(const G&, const uint32_t, const F&) | std::vector<int32_t>* | Algorytm przechodzenia wszerz, który zwraca informację o odległości między wierzchołkiem startowym, a wszystkimi wierzchołkami w grafie, wraz z "instrukcjami" dot. przechodznia po grafie. |
| bfs_functor(const G&, const uint32_t, F&) | void | Algorytm przechodzenia po grafie, który po dotarciu do danego wierzchołka wywołuje dany przez użytkownika bool operator()(const uint32_t), który został zaimplementowany w klasie F. |
| bfs_path(const G&, const uint32_t, const uint32_t) | std::vector<int32_t>* | Algorytm przechodzenia wszerz, który zwraca ścieżkę między dwoma wierzchołkami, w przypadku jeżeli nie istnieje taka ścieżka, funkcja zwraca nullptr. |
| bfs_path_with_f(const G&, const uint32_t, const uint32_t, const F&) | std::vector<int32_t>* | Algorytm przechodzenia wszerz, który zwraca ścieżkę między dwoma wierzchołkami, wraz z "instrukcjami" dot. przechodzenia po grafie. W przypadku jeżeli nie istnieje taka ścieżka, funkcja zwraca nullptr. |

### Przeszukiwanie w głąb
Biblioteka daje dwie opcje przechodzenia po grafie w głąb: przechodzenie pre-order i przechodzenie post-order. Pierwszy rodzaj najpierw wykonuje operację na wierzchołku i przechodzi dalej (w głąb grafu), a drugi robi dokładnie odwrotnie.

| Nazwa funkcji | Typ zwracany | Opis                                                                 |
|---------------|---------------|----------------------------------------------------------------------|
| dfs_preorder()   | void | Algorytm przechodzenia w głąb grafu (pre-order). |
| dfs_postorder()  | void | Algorytm przechodzenia w głąb grafu (post-order). |

### Budowanie minimalnego drzewa rozpinającego
W bilbiotece zostały zaimplementowane dwa algorytmy, które służą do tworzenia minimalnego drzewa rozpinającego. Obie funkcje zwracają wskaźnik do instancji std::vector, który przyjmuje std::pair<uint32_t, uint32_t>, która reprezentuje krawędź między dwoma wierzchołkami. W przypadku jeżeli nie uda się wygenerować minimalnego drzewa rozpinającego, zostaje zwrócona wartość NULL.

| Nazwa funkcji | Typ zwracany | Opis                                                                 |
|---------------|---------------|----------------------------------------------------------------------|
| prim()        | std::vector<std::pair<uint32_t, uint32_t>> | Implementuje algorytm Prima do znajdowania minimalnego drzewa rozpinającego (MST) grafu. |
| kruskal()     | std::vector<std::pair<uint32_t, uint32_t>> | Implementuje algorytm Kruskala do znajdowania minimalnego drzewa rozpinającego (MST) grafu. |

### Algorytm Dijkstry
Algorytm Dijkstry wyszukuje najkrótszą scieżkę w grafie ważonym. Ważnym ograniczeniem tego algorytmu jest to, że działa on jedynie w grafach bez żadnych wag ujemnych.

| Nazwa funkcji | Typ zwracany | Opis                                                                 |
|---------------|---------------|----------------------------------------------------------------------|
| dijkstra(const G&, const uint32_t, const uint32_t) | N | Algorytm Dijkstry, który zwraca odległość najkrótszej ścieżki między dwoma wierzchołkami. |
| dijkstra(const G&, const uint32_t, const std::vector<uint32_t>&) | std::unordered_map<uint32_t, N>* | Algorytm Dijkstry, który zwraca odległości najkrótszych ścieżek, między wierzchołkiem startowym, a wybranymi przez użytkownika wierzchołkami "docelowymi". |
| dijkstra(const G&, const uint32_t) | std::vector<N>* | Algorytm Dijkstry, który zwraca odległości między wierzchołkiem startowym, a wszystkimi wierzchołkami w grafie. |
| dijkstra_with_f(const G&, const uint32_t, const uint32_t, const F&) | N | Algorytm Dijkstry, który zwraca odległość najkrótszej ścieżki między dwoma wierzchołami, wraz z "instrukcjami" dot. przechodzenia po grafie. |
| dijkstra_with_f(const G&, const uint32_t, const std::vector<uint32_t>&, const F&) | std::unordered_map<uint32_t, N>* | Algorytm Dijkstry, który zwraca odległość najkrótszej ścieżki między wierzchołkiem startowym, a listą wierzchołków "docelowych", wraz z "instrukcjami" dot. przechodzenia po grafie. |
| dijkstra_with_f(const G&, const uint32_t, const F&) | std::vector<N>* | Algorytm Dijkstry, który zwraca odległość najkrótszej ścieżki między wierzchołkiem startowym, a wszystkimi wierzchołkami w grafie, wraz z "instrukcjami" dot. przechodzenia po grafie. |
| dijkstra_path(const G&, const uint32_t, const uint32_t) | std::vector<uint32_t>* | Algorytm Dijkstry, który zwraca ścieżkę między dwoma wierzchołkami. W przypadku jeżeli ścieżka nie istnieje funkcja zwraca nullptr. |
| dijkstra_path_with_f(const G&, const uint32_t, const uint32_t, const F&) | std::vector<uint32_t>* | Algorytm Dijkstry, który zwraca ścieżkę między dwoma wierzchołkami, wraz z "instrukcjami" dot. przechodzenia po grafie. W przypadku jeżeli ścieżka nie istnieje funkcja zwraca nullptr. |

### Algorytmy Bellmana-Forda i Floyda-Warshalla
Biblioteka implementuje dwa algorytmy, które wyszukują najkrótszą scieżkę w grafie z wagami ujemnymi. Pierwszy z nich służy do wyznaczenia najkrótszej odległości od jednego wierzchołka, a drugi z nich służy do wyznaczenia odległości między wszystkimi wierzchołkami. Ważnym aspektem wyznaczania najkrótszej ścieżki w grafach z wagami ujemnymi jest pojęcie cyklu ujemnego. Cyklem ujemnym nazywamy taki cykl, w którym suma wag krawędzi jest ujemna. Wystąpienie takiej ścieżki w grafie powoduje, że niemożliwym staje się wyznaczenie najkrótszej ścieżki (zawsze można wykonać dodatkowe przejście po takiej ścieżce, aby zmniejszyć sumę wag). Oba algorytmy sprawdzają wystąpienie cyklu ujemnego i w przypadku jego istnienia zostaje zwrócona wartość NULL. 

| Nazwa funkcji | Typ zwracany | Opis                                                                 |
|---------------|---------------|----------------------------------------------------------------------|
| bellmanFord()        | std::vector<N>* | Funkcja implementuje algorytm Bellmana-Forda, w przypadku wystąpienia cyklu ujemnego zostaje zwrócona wartość NULL. |
| floydWarshall()    | std::vector<std::vector<N>>* | Implementuje algorytm Floyda-Warshalla, w przypadku wystąpienia cyklu ujemnego zostaje zwrócona wartość NULL. |

### Algorytm A*
Ostatnim algorytm jaki został zaimplementowany jest algorytm A*, który umożliwia wyszukiwanie najkrótszej ścieżki w grafie (bez wag ujemnych). Algorytm ten jest szybszy od algorytmu Dijkstry, który ma dokładnie takie samo zadanie, różnica polega na tym, że A* używa funkcji heurystycznej, która "ukierunkowuje" kierunek przechodzenia po grafie. Ważne jest to, że funkcja heurystyczna musi stanowić "optymistycznie oszacowanie", co oznacza, że odległość zwrócona przez funkcję heurystyczną między dwoma wierzchołkami nie może być większa niż "rzeczywista" odległość między tymi wierzchołkami.

| Nazwa funkcji | Typ zwracany | Opis                                                                 |
|---------------|---------------|----------------------------------------------------------------------|
| A_star()        | N            | Funkcja implmenetuje algorytm A* i zwraca odległość między wybranymi wierzchołkami. |
| A_star_path()     | std::vector<uint32_t>* | Funkcja implementuje algorytm A* i zwraca wskaźnik do std::vector, który trzyma ścieżkę od wierzchołka startowego do końcowego. W przypadku braku takiej ścieżki zostaje zwrócona wartość NULL. |


