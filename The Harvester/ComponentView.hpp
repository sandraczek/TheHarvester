#pragma once
#include <tuple>        // Dla std::tuple
#include <vector>       // Dla przechowywania listy encji
#include <algorithm>    // Dla std::all_of

// Forward declaration ComponentStorage, bo ComponentView i Iterator b�d� z niej korzysta�
class ComponentStorage;

// --- ComponentViewIterator ---
// Iterator dla ComponentView, umo�liwiaj�cy iteracj� po encjach z wybranymi komponentami
template<typename... Components>
class ComponentViewIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::tuple<Entity, Components&...>; // To jest to, co zwracamy
    using difference_type = std::ptrdiff_t;
    using pointer = void; // Nie u�ywamy bezpo�redniego wska�nika
    using reference = value_type; // Zwracamy tuple z referencjami

    // Konstruktor do tworzenia iterat�w
    ComponentViewIterator(
        typename std::vector<Entity>::const_iterator entityIt,
        typename std::vector<Entity>::const_iterator entityEnd,
        ComponentStorage* storagePtr
    ) :
        currentEntityIt(entityIt),
        entitiesEnd(entityEnd),
        storage(storagePtr)
    {
        // Przy starcie iteratora, upewniamy si�, �e wskazuje na poprawn� encj�,
        // kt�ra posiada wszystkie wymagane komponenty.
        // Je�li aktualna encja nie ma wszystkich komponent�w, przechodzimy do nast�pnej.
        skipToNextValidEntity();
    }

    // Dereferencja operatora * (zwraca tuple z referencjami)
    reference operator*() const {
        if (currentEntityIt == entitiesEnd) {
            // Zwr�� co� sensownego dla ko�ca, cho� powinno by� to sprawdzane przed dereferencj�
            // Lepsze by�oby rzucenie wyj�tku lub upewnienie si�, �e nigdy si� to nie zdarza.
            // Dla prostoty zwracamy domy�lnie zainicjowan� tupl�.
            return {};
        }

        // U�ycie pack expansion do pobrania referencji do ka�dego komponentu
        // 'std::tie' tworzy tuple referencji, co pozwala na wi�zanie strukturalne
        return std::tie(*currentEntityIt, storage->get<Components>(*currentEntityIt)...);
    }

    // Operator inkrementacji pre-increment
    ComponentViewIterator& operator++() {
        if (currentEntityIt != entitiesEnd) {
            ++currentEntityIt;
            skipToNextValidEntity(); // Przeskocz do nast�pnej wa�nej encji
        }
        return *this;
    }

    // Operator inkrementacji post-increment (nieefektywny, ale wymagany dla zgodno�ci)
    ComponentViewIterator operator++(int) {
        ComponentViewIterator temp = *this;
        ++(*this);
        return temp;
    }

    // Operatory por�wnania
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

    // Pomocnicza funkcja do przeskakiwania encji, kt�re nie maj� wszystkich komponent�w
    void skipToNextValidEntity() {
        while (currentEntityIt != entitiesEnd) {
            Entity currentEntity = *currentEntityIt;
            // Sprawd�, czy aktualna encja posiada wszystkie wymagane komponenty
            bool allComponentsPresent = (storage->has<Components>(currentEntity) && ...); // Fold expression C++17

            if (allComponentsPresent) {
                break; // Znaleziono poprawn� encj�, zatrzymaj si�
            }
            ++currentEntityIt; // Przejd� do nast�pnej encji
        }
    }
};

// --- ComponentView ---
// Klasa zwracana przez ComponentStorage::view(), kt�ra dostarcza iteratory
template<typename... Components>
class ComponentView {
public:
    // Alias dla typu iteratora
    using Iterator = ComponentViewIterator<Components...>;

    ComponentView(ComponentStorage& storageRef) :
        storage(storageRef)
    {
        // Wype�nij list� wszystkich encji, kt�re posiadaj� JAKIKOLWIEK komponent.
        // Optymalniej by�oby w tym miejscu od razu filtrowa� tylko te, kt�re posiadaj� WSZYSTKIE Components.
        // Jednak pro�ciej jest to zrobi� w iteratorze.
        // Tworzymy tymczasowy wektor wszystkich unikalnych encji.
        // Mo�esz to zoptymalizowa�, aby getAll<Position>() zwraca�o tylko encje z PositionComponent,
        // a potem zebra� unikalne ID.
        // Tutaj dla prostoty bierzemy wszystkie encje, kt�re maj� przynajmniej jeden zdefiniowany komponent.
        // Lepsze podej�cie by�oby w ComponentStorage, aby przechowywa� `std::set<Entity>` wszystkich istniej�cych encji.
        // Na razie za��my, �e Position jest zawsze obecne w interesuj�cych nas encjach.
        for (auto const& pair : storage.getAll<Position>()) { // Przyk�adowo, pobieramy encje z Position
            allEntities.push_back(pair.first);
        }
        // ... musia�by� zebra� unikalne ID ze wszystkich storage'�w, kt�re s� aktualnie w u�yciu
        // lub mie� centraln� list� wszystkich aktywnych encji w ComponentStorage
    }

    // Metody begin() i end() wymagane dla p�tli for-each
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