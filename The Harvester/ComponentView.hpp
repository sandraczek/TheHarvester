#pragma once
#include <tuple>        // Dla std::tuple
#include <vector>       // Dla przechowywania listy encji
#include <algorithm>    // Dla std::all_of

// Forward declaration ComponentStorage, bo ComponentView i Iterator bêd¹ z niej korzystaæ
class ComponentStorage;

// --- ComponentViewIterator ---
// Iterator dla ComponentView, umo¿liwiaj¹cy iteracjê po encjach z wybranymi komponentami
template<typename... Components>
class ComponentViewIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::tuple<Entity, Components&...>; // To jest to, co zwracamy
    using difference_type = std::ptrdiff_t;
    using pointer = void; // Nie u¿ywamy bezpoœredniego wskaŸnika
    using reference = value_type; // Zwracamy tuple z referencjami

    // Konstruktor do tworzenia iteratów
    ComponentViewIterator(
        typename std::vector<Entity>::const_iterator entityIt,
        typename std::vector<Entity>::const_iterator entityEnd,
        ComponentStorage* storagePtr
    ) :
        currentEntityIt(entityIt),
        entitiesEnd(entityEnd),
        storage(storagePtr)
    {
        // Przy starcie iteratora, upewniamy siê, ¿e wskazuje na poprawn¹ encjê,
        // która posiada wszystkie wymagane komponenty.
        // Jeœli aktualna encja nie ma wszystkich komponentów, przechodzimy do nastêpnej.
        skipToNextValidEntity();
    }

    // Dereferencja operatora * (zwraca tuple z referencjami)
    reference operator*() const {
        if (currentEntityIt == entitiesEnd) {
            // Zwróæ coœ sensownego dla koñca, choæ powinno byæ to sprawdzane przed dereferencj¹
            // Lepsze by³oby rzucenie wyj¹tku lub upewnienie siê, ¿e nigdy siê to nie zdarza.
            // Dla prostoty zwracamy domyœlnie zainicjowan¹ tuplê.
            return {};
        }

        // U¿ycie pack expansion do pobrania referencji do ka¿dego komponentu
        // 'std::tie' tworzy tuple referencji, co pozwala na wi¹zanie strukturalne
        return std::tie(*currentEntityIt, storage->get<Components>(*currentEntityIt)...);
    }

    // Operator inkrementacji pre-increment
    ComponentViewIterator& operator++() {
        if (currentEntityIt != entitiesEnd) {
            ++currentEntityIt;
            skipToNextValidEntity(); // Przeskocz do nastêpnej wa¿nej encji
        }
        return *this;
    }

    // Operator inkrementacji post-increment (nieefektywny, ale wymagany dla zgodnoœci)
    ComponentViewIterator operator++(int) {
        ComponentViewIterator temp = *this;
        ++(*this);
        return temp;
    }

    // Operatory porównania
    bool operator==(const ComponentViewIterator& other) const {
        return currentEntityIt == other.currentEntityIt;
    }

    bool operator!=(const ComponentViewIterator& other) const {
        return !(*this == other);
    }

private:
    typename std::vector<Entity>::const_iterator currentEntityIt;
    typename std::vector<Entity>::const_iterator entitiesEnd;
    ComponentStorage* storage;

    // Pomocnicza funkcja do przeskakiwania encji, które nie maj¹ wszystkich komponentów
    void skipToNextValidEntity() {
        while (currentEntityIt != entitiesEnd) {
            Entity currentEntity = *currentEntityIt;
            // SprawdŸ, czy aktualna encja posiada wszystkie wymagane komponenty
            bool allComponentsPresent = (storage->has<Components>(currentEntity) && ...); // Fold expression C++17

            if (allComponentsPresent) {
                break; // Znaleziono poprawn¹ encjê, zatrzymaj siê
            }
            ++currentEntityIt; // PrzejdŸ do nastêpnej encji
        }
    }
};

// --- ComponentView ---
// Klasa zwracana przez ComponentStorage::view(), która dostarcza iteratory
template<typename... Components>
class ComponentView {
public:
    // Alias dla typu iteratora
    using Iterator = ComponentViewIterator<Components...>;

    ComponentView(ComponentStorage& storageRef) :
        storage(storageRef)
    {
        // Wype³nij listê wszystkich encji, które posiadaj¹ JAKIKOLWIEK komponent.
        // Optymalniej by³oby w tym miejscu od razu filtrowaæ tylko te, które posiadaj¹ WSZYSTKIE Components.
        // Jednak proœciej jest to zrobiæ w iteratorze.
        // Tworzymy tymczasowy wektor wszystkich unikalnych encji.
        // Mo¿esz to zoptymalizowaæ, aby getAll<Position>() zwraca³o tylko encje z PositionComponent,
        // a potem zebraæ unikalne ID.
        // Tutaj dla prostoty bierzemy wszystkie encje, które maj¹ przynajmniej jeden zdefiniowany komponent.
        // Lepsze podejœcie by³oby w ComponentStorage, aby przechowywaæ `std::set<Entity>` wszystkich istniej¹cych encji.
        // Na razie za³ó¿my, ¿e Position jest zawsze obecne w interesuj¹cych nas encjach.
        for (auto const& pair : storage.getAll<Position>()) { // Przyk³adowo, pobieramy encje z Position
            allEntities.push_back(pair.first);
        }
        // ... musia³byœ zebraæ unikalne ID ze wszystkich storage'ów, które s¹ aktualnie w u¿yciu
        // lub mieæ centraln¹ listê wszystkich aktywnych encji w ComponentStorage
    }

    // Metody begin() i end() wymagane dla pêtli for-each
    Iterator begin() {
        return Iterator(allEntities.begin(), allEntities.end(), &storage);
    }

    Iterator end() {
        return Iterator(allEntities.end(), allEntities.end(), &storage);
    }

private:
    ComponentStorage& storage;
    std::vector<Entity> allEntities; // Tymczasowa lista encji do iteracji
};