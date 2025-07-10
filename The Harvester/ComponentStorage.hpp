#pragma once

#include <unordered_map>
#include <vector>
#include <any>
#include <typeindex>
#include <tuple>
#include <optional>
#include <functional>

using Entity = uint32_t;

class ComponentStorage
{
private:
    std::unordered_map<std::type_index, std::any> storages;

    template<typename T>
    std::unordered_map<Entity, T>& getOrCreateStorage() {
        auto type_id = std::type_index(typeid(T));
        auto it = storages.find(type_id);

        if (it == storages.end()) {
            storages[type_id] = std::unordered_map<Entity, T>();
        }

        return std::any_cast<std::unordered_map<Entity, T>&>(storages.at(type_id));
    }

    template<typename T>
    const std::unordered_map<Entity, T>* getStorage() const {
        auto type_id = std::type_index(typeid(T));
        auto it = storages.find(type_id);

        if (it == storages.end()) {
            return nullptr;
        }

        return std::any_cast<std::unordered_map<Entity, T>>(&it->second);
    }

public:
    template<typename T>
    void add(Entity e, const T& component) {
        getOrCreateStorage<T>()[e] = component;
    }

    template<typename T>
    T& get(Entity e) {
        return getOrCreateStorage<T>().at(e);
    }

    template<typename T>
    T* try_get(Entity e) {
        auto* storage = const_cast<std::unordered_map<Entity, T>*>(getStorage<T>());
        if (!storage) return nullptr;

        auto it = storage->find(e);
        if (it != storage->end()) {
            return &(it->second);
        }
        return nullptr;
    }

    template<typename T>
    bool has(Entity e) {
        const auto* storage = getStorage<T>();
        return storage ? storage->count(e) > 0 : false;
    }

    template<typename T>
    void remove(Entity e) {
        auto* storage = const_cast<std::unordered_map<Entity, T>*>(getStorage<T>());
        if (storage) {
            storage->erase(e);
        }
    }

    template<typename T>
    std::unordered_map<Entity, T>& getAll() {
        return getOrCreateStorage<T>();
    }

    void removeAll(Entity e) {
        for (auto& [type, any_storage] : storages) {
            std::any_cast<std::unordered_map<Entity, std::any>&>(any_storage).erase(e);
        }
    }

    template <typename... Components>
    class View {
    private:
        // Przechowujemy wskaŸniki do map komponentów, które nas interesuj¹.
        std::tuple<std::unordered_map<Entity, Components>*...> component_maps;
        // WskaŸnik do mapy, po której bêdziemy iterowaæ (tej najmniejszej).
        std::unordered_map<Entity, std::any>* smallest_map;

        // Metoda pomocnicza do sprawdzania, czy encja ma wszystkie pozosta³e komponenty
        bool has_all_components(Entity e) {
            // U¿ywamy "fold expression" (C++17) do sprawdzenia wszystkich warunków.
            return ((std::get<std::unordered_map<Entity, Components>*>(component_maps)->count(e) > 0) && ...);
        }

    public:
        // Konstruktor przyjmuje referencje do map od ComponentStorage
        View(std::unordered_map<Entity, Components>&... maps)
            : component_maps(std::make_tuple(&maps...)) {
            // Prosta optymalizacja: znajdujemy najmniejsz¹ mapê, aby po niej iterowaæ
            size_t min_size = -1;

            // Kolejny "fold expression" do znalezienia najmniejszej mapy.
            ((maps.size() < min_size ? (min_size = maps.size(), smallest_map = reinterpret_cast<std::unordered_map<Entity, std::any>*>(&maps)) : 0), ...);
        }

        class iterator {
        private:
            View& view;
            // Iterator mapy, po której faktycznie siê poruszamy
            typename std::unordered_map<Entity, std::any>::iterator current_iterator;
            typename std::unordered_map<Entity, std::any>::iterator end_iterator;

            // Przesuwa iterator do nastêpnej pasuj¹cej encji
            void advance() {
                while (current_iterator != end_iterator) {
                    if (view.has_all_components(current_iterator->first)) {
                        return; // Znaleziono pasuj¹c¹, przerywamy.
                    }
                    ++current_iterator; // Sprawdzamy nastêpn¹.
                }
            }

        public:
            iterator(View& v, bool is_end) : view(v) {
                if (view.smallest_map) {
                    current_iterator = is_end ? view.smallest_map->end() : view.smallest_map->begin();
                    end_iterator = view.smallest_map->end();
                    if (!is_end) {
                        advance(); // Ustawiamy iterator na pierwszej pasuj¹cej encji
                    }
                }
            }

            // Zwraca krotkê z ID encji i referencjami do jej komponentów
            auto operator*() const {
                Entity entity = current_iterator->first;
                return std::tie(entity, std::get<std::unordered_map<Entity, Components>*>(view.component_maps)->at(entity)...);
            }

            iterator& operator++() {
                ++current_iterator;
                advance();
                return *this;
            }

            bool operator!=(const iterator& other) const {
                return current_iterator != other.current_iterator;
            }
        };

        iterator begin() { return iterator(*this, false); }
        iterator end() { return iterator(*this, true); }
    };

    // Funkcja fabrykuj¹ca, która tworzy i zwraca obiekt View
    template <typename... Components>
    View<Components...> view() {
        // "Fold expression" do pobrania referencji do wszystkich potrzebnych map.
        return View(getOrCreateStorage<Components>()...);
    }
};