/**
 * @file
 * @brief Заголовочный файл с шаблонами классов коллекции.
 * @author Igor Filatov
 */

#ifndef FRCOLLECTION_H
#define FRCOLLECTION_H

#include <map>

namespace feerie
{
    /**
     * @brief Шаблон коллекции "ключ/значение" и псевдонимы/ключ.
     * @details Шаблон класса коллекции "ключ/значение" (aka map), с возможностью хранить псевдонимы для ключа.
     */
    template<class Key, typename Value, class... KeyAliases>
    class FrCollection
    {
    protected:

        static constexpr std::size_t MaxOffset = sizeof...(KeyAliases);

        /* ==================================== FrCollectionAliases ==================================== */

        // Basic template for the collection of aliases
        template<std::size_t Offset, class Basic, class... Aliases> struct FrCollectionAliases;

        // Specialization terminating the recursion of the alias collection template
        template<std::size_t Offset, class Basic> struct FrCollectionAliases<Offset, Basic> {};

        // Alias collection template, contains a map container, with a pseudonym to a key
        template<std::size_t Offset, class Basic, class Alias, class... Aliases>
        struct FrCollectionAliases<Offset, Basic, Alias, Aliases...> : public FrCollectionAliases<Offset + 1, Basic, Aliases...>
        {
            using TypeAlias = Alias;
            using TypeAliases = std::map<Alias, Basic>;
            FrCollectionAliases() : values() {}
            std::map<Alias, Basic> values;
        };

        /* ==================================== Get from FrCollectionAliases ==================================== */

        template<std::size_t Index, std::size_t Offset, class Basic, class Alias, class... Aliases>
        struct getter
        {
            typedef typename getter<Index - 1, Offset + 1, Basic, Aliases...>::return_type return_type;

            static  typename std::enable_if<(Index > 0), const return_type&>::type
            get(const FrCollectionAliases<Offset, Basic, Alias, Aliases...>& collectionAliases)
            {
                return getter<Index - 1, Offset + 1, Basic, Aliases...>::get(collectionAliases);
            }
        };

        template<std::size_t Offset, class Basic, class Alias, class... Aliases>
        struct getter<0, Offset, Basic, Alias, Aliases...>
        {
            typedef typename FrCollectionAliases<Offset, Basic, Alias, Aliases...>::TypeAliases return_type;
            static const return_type& get(const FrCollectionAliases<Offset, Basic, Alias, Aliases...>& collectionAliases)
            {
                return collectionAliases.values;
            }
        };

        template<std::size_t Index, class Basic, class Alias, class... Aliases>
        const typename getter<Index, 0, Basic, Alias, Aliases...>::return_type&
        getKey(const FrCollectionAliases<0, Basic, Alias, Aliases...>& aliases) const
        {
            return getter<Index, 0, Basic, Alias, Aliases...>::get(aliases);
        }

        /* ==================================== Clear operation with FrCollectionAliases ==================================== */

        // Function of stub operations with the collection of aliases (serves as a stub if there are no aliases in the collection)
        template <std::size_t Offset, class Basic, class... Aliases>
        void clearOperation(FrCollectionAliases<Offset, Basic>&)
        {
        }

        // Overloaded function of operations with the collection of aliases if they reached the bottom of the recursion
        template <std::size_t Offset, class Basic, class Alias>
        void clearOperation(FrCollectionAliases<Offset, Basic, Alias>& collectionAliases)
        {
            return collectionAliases.values.clear();
        }

        // Overloaded recursive function of operations with a collection of aliases
        template <std::size_t Offset, class Basic, class Alias, class... Aliases>
        typename std::enable_if<(Offset < MaxOffset), void>::type
        clearOperation(FrCollectionAliases<Offset, Basic, Alias, Aliases...>& collectionAliases)
        {
            collectionAliases.values.clear();
            FrCollectionAliases<Offset + 1, Basic, Aliases...>& base = collectionAliases;
            return clearOperation<Offset + 1>(base);
        }

        /* ==================================== Remove operation with FrCollectionAliases ==================================== */

        // Function of stub operations with the collection of aliases (serves as a stub if there are no aliases in the collection)
        template <std::size_t Offset, class Basic, class... Aliases>
        bool removeOperation(FrCollectionAliases<Offset, Basic>&, const Basic&)
        {
            return true;
        }

        // Overloaded function of operations with the collection of aliases if they reached the bottom of the recursion
        template <std::size_t Offset, class Basic, class Alias>
        bool removeOperation(FrCollectionAliases<Offset, Basic, Alias>& collectionAliases, const Basic& basic)
        {
            std::size_t count = 0;
            for(auto iterator = collectionAliases.values.begin(); iterator != collectionAliases.values.end();)
            {
                if(basic == iterator->second)
                {
                   iterator = collectionAliases.values.erase(iterator);
                   count++;
                } else ++iterator;
            }
            return count > 0;
        }

        // Overloaded recursive function of operations with a collection of aliases
        template <std::size_t Offset, class Basic, class Alias, class... Aliases>
        typename std::enable_if<(Offset < MaxOffset), bool>::type
        removeOperation(FrCollectionAliases<Offset, Basic, Alias, Aliases...>& collectionAliases, const Basic& basic)
        {
            std::size_t count = 0;
            for(auto iterator = collectionAliases.values.begin(); iterator != collectionAliases.values.end();)
            {
                if(basic == iterator->second)
                {
                   iterator = collectionAliases.values.erase(iterator);
                   count++;
                } else ++iterator;
            }

            if(count > 0)
            {
                FrCollectionAliases<Offset + 1, Basic, Aliases...>& base = collectionAliases;
                return removeOperation<Offset + 1>(base, basic);
            }
            return false;
        }

        /* ==================================== Add Operation with FrCollectionAliases ==================================== */

        // Function of stub operations with the collection of aliases (serves as a stub if there are no aliases in the collection)
        template <std::size_t Offset, class Basic, class... Aliases>
        bool addOperation(FrCollectionAliases<Offset, Basic>&, const Basic&)
        {
            return true;
        }

        // Overloaded recursive function of operations with a collection of aliases
        template <std::size_t Offset, class Basic, class Alias>
        bool addOperation(FrCollectionAliases<Offset, Basic, Alias>& collectionAliases, const Basic& basic, const Alias& alias)
        {
            return collectionAliases.values.insert({alias, basic}).second;
        }

        // Overloaded recursive function of operations with a collection of aliases
        template <std::size_t Offset, class Basic, class Alias, class... Aliases>
        typename std::enable_if<(Offset < MaxOffset), bool>::type
        addOperation(FrCollectionAliases<Offset, Basic, Alias, Aliases...>& collectionAliases,
                     const Basic& basic, const Alias& alias, const Aliases&... aliases)
        {
            if(collectionAliases.values.insert({alias, basic}).second)
            {
                FrCollectionAliases<Offset + 1, Basic, Aliases...>& base = collectionAliases;
                return addOperation<Offset + 1>(base, basic, aliases...);
            }
            return false;
        }

    public:

        /* ==================================== Tools ==================================== */

        /**
         * @brief Конструктор FrCollection.
         * @param[in] defaultValue Cсылка на значение по умолчанию.
         */
        FrCollection(const Value& defaultValue = Value()) : values(), aliases(), defaultValue(defaultValue) {}

        /**
         * @brief Возвращает количество записей в коллекции.
         * @return Количество записей.
         */
        inline std::size_t size() const
        {
            return values.size();
        }

        /**
         * @brief Очищает коллекцию.
         */
        inline void clear()
        {
            clearOperation<0>(aliases);
            values.clear();
        }

        /**
         * @brief Возращает ссылку на значение по умолчанию.
         * @return Ссылка на значение по умолчанию.
         */
        inline const Value& getDefaultValue() const
        {
            return defaultValue;
        }

        /**
         * @brief Добавляет запись с заданным ключом, значением и псевдонимами.
         * @param[in] key Ключ.
         * @param[in] value Значение.
         * @param[in] aliases Псевдонимы.
         * @return В случае успеха true, иначе false.
         */
        inline bool add(const Key& key, const Value& value, const KeyAliases&... aliases)
        {
            if(values.insert({key, value}).second)
            {
                if(addOperation<0>(this->aliases, key, aliases...))
                    return true;
                remove(key);
            }
            return false;
        }

        /* ==================================== Operation with key ==================================== */

        /**
         * @brief Удаляет запись по заданному ключу.
         * @param[in] key Ключ.
         * @return В случае успеха true, иначе false.
         */
        inline bool remove(const Key& key)
        {
            if(values.erase(key) == 1) return removeOperation<0>(aliases, key);
            return false;
        }

        /**
         * @brief operator [] Возвращает ссылку на значение по ключу.
         * @param[in] key Ключ.
         * @return Ссылка на значение. Если ключ не найден, возвращает ссылку на значение по умолчанию.
         */
        inline const Value& operator[](const Key& key) const
        {
            const auto& it = values.find(key);
            return (it != values.end() ? it->second : defaultValue);
        }

        /**
         * @brief Возвращает ссылку на значение по ключу.
         * @param[in] key Ключ.
         * @param[out] value Ссылка на значение. Если ключ не найден, значение не меняется.
         * @return В случае успеха true, иначе false.
         */
        inline bool get(const Key& key, Value& value) const
        {
            const auto& it = values.find(key);
            if(it != values.end())
            {
                value = it->second;
                return true;
            }
            return false;
        }

        /**
         * @brief Обновляет значение по ключу.
         * @param[in] key Ключ.
         * @param[in] value Ссылка на новое значение.
         * @return В случае успеха true, иначе false.
         */
        inline bool update(const Key& key, const Value& value)
        {
            auto it = values.find(key);
            if(it != values.end())
            {
                it->second = value;
                return true;
            }
            return false;
        }

        /**
         * @brief Проверяет наличие записи в коллекции по ключу.
         * @param[in] key Ключ.
         * @return В случае присутствия значения - true, иначе false.
         */
        inline bool exists(const Key& key) const
        {
            return values.find(key) != values.end();
        }

        /* ==================================== Operation with Aliases ==================================== */

        /**
         * @brief Удаляет запись по заданному псевдониму.
         * @param[in] alias Псевдоним.
         * @return В случае успеха true, иначе false.
         */
        template<std::size_t Offset, class TypeAlias>
        bool remove(const TypeAlias& alias)
        {
            Key key;
            if(find<Offset>(alias, key)) return remove(key);
            return false;
        }

        /**
         * @brief Возвращает ссылку на значение по псевдониму.
         * @param[in] alias Псевдоним.
         * @return Ссылка на значение. Если запись не найдена, возвращает ссылку на значение по умолчанию.
         */
        template<std::size_t Offset, class TypeAlias>
        const Value& get(const TypeAlias& alias) const
        {
            Key key;
            if(find<Offset>(alias, key)) return operator [](key);
            return defaultValue;
        }

        /**
         * @brief Возвращает ссылку на значение по псевдониму.
         * @param[in] alias Псевдоним.
         * @param[out] value Ссылка на значение. Если запись не найдена, значение не меняется.
         * @return В случае успеха true, иначе false.
         */
        template<std::size_t Offset, class TypeAlias>
        bool get(const TypeAlias& alias, Value& value) const
        {
            Key key;
            if(find<Offset>(alias, key)) return get(key, value);
            return false;
        }

        /**
         * @brief Обновляет значение по псевдониму.
         * @param[in] alias Псевдоним.
         * @param[in] value Ссылка на новое значение.
         * @return В случае успеха true, иначе false.
         */
        template<std::size_t Offset, class TypeAlias>
        bool update(const TypeAlias& alias, const Value& value)
        {
            Key key;
            if(find<Offset>(alias, key)) return update(key, value);
            return false;
        }

        /**
         * @brief Проверяет наличие записи в коллекции по псевдониму.
         * @param[in] alias Псевдоним.
         * @return В случае присутствия записи - true, иначе false.
         */
        template<std::size_t Offset, class TypeAlias>
        inline bool exists(const TypeAlias& alias) const
        {
            return getKey<Offset>(aliases).find(alias) != getKey<Offset>(aliases).end();
        }

        /**
         * @brief Возвращает ключ по псевдониму.
         * @param[in] alias Псевдоним.
         * @param[out] value Найденый ключ. Если псевдоним не найден, ключ не меняется.
         * @return В случае успеха true, иначе false.
         */        
        template<std::size_t Offset, class TypeAlias>
        bool find(const TypeAlias& alias, Key& key) const
        {
            const auto& it = getKey<Offset>(aliases).find(alias);
            if(it != getKey<Offset>(aliases).end())
            {
                key = it->second;
                return true;
            }
            return false;
        }

    protected:

        const Value defaultValue;
        std::map<Key, Value> values;
        FrCollectionAliases<0, Key, KeyAliases...> aliases;

    };
}

#endif // FRCOLLECTION_H
