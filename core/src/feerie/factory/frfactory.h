/**
 * @file
 * @brief Заголовочный файл с классом фабрики движка.
 * @author Igor Filatov
 */
#ifndef FRFACTORY_H
#define FRFACTORY_H

#include "../type/frtype.h"
#include "../collection/frcollection.h"

namespace feerie
{
    /**
     * @brief Шаблон класса фабрики.
     * @details Шаблон класса фабрики движка.
     * Ключ из хэша типа, для более быстрого поиска, компактности, и псевдоним из названия типа для читаемости.
     * Значение содержит пару из типа и функтора создания.
     */
    template<class String>
    class FrFactory
    {
    public:

        using Type = FrType<String>;

    private:

        static const std::size_t NAME = 0;

        FrCollection<size_t, Type*, String> types;

        template<typename Result, typename... Args>
        struct TypeWithCreator : Type
        {
            using Creator = Result(Args...);

            Creator* creator;

            TypeWithCreator(const String& name, Creator* creator) :
                Type(name),
                creator(creator)
            {
            }
        };

    public:

        /**
         * @brief Конструктор фабрики FrFactory.
         */
        FrFactory() : types(nullptr)
        {
        }

        /**
         * @brief Возращает указатель на тип с заданным названием.
         * @param[in] name Название типа.
         * @return Указатель на тип.
         * @details Если тип не найден, то возвращается nullptr.
         */
        inline Type* getType(const String& name) const
        {
            return types.template get<NAME>(name);
        }

        /**
         * @brief Возращает указатель на тип с заданным хешем.
         * @param[in] name Хеш типа.
         * @return Указатель на тип.
         * @details Если тип не найден, то возвращается nullptr.
         */
        inline Type* getType(size_t hash) const
        {
            return types[hash];
        }

        /**
         * @brief Добавление типа с функтором создания в фабрику.
         * @param[in] name Название типа.
         * @param[in] creator Функтор создания.
         * @return true в случае успешной регистрации, иначе false.
         */
        template<typename Result, typename... Args>
        bool add(const String& name, Result (*creator)(Args...))
        {
            auto* typeWithCreator = new TypeWithCreator<Result, Args...>(name, creator);
            return types.add(typeWithCreator->getHash(), typeWithCreator, typeWithCreator->getName());
        }

        /**
         * @brief Создание экземпляра класса по ссылке тип.
         * @param[in] type Ссылка на тип.
         * @param[in] args Аргументы функтора создания.
         * @return Возвращает экземпляр класса с типом из параметра шаблона Result.
         * @details Создание экземпляра класса по ссылке на привязанный к нему тип из фабрики.
         * В случае успеха вызывается функтор с переданными аргументами. В случае неудачи возвращается Result().
         */
        template<typename Result, typename... Args>
        inline Result create(const Type& type, const Args&... args) const
        {
            return create<Result, Args...>(type.getHash(), args...);
        }

        /**
         * @brief Создание экземпляра класса по указателю тип.
         * @param[in] type Указатель на тип.
         * @param[in] args Аргументы функтора создания.
         * @return Возвращает экземпляр класса с типом из параметра шаблона Result.
         * @details Создание экземпляра класса по указателю на привязанный к нему тип из фабрики.
         * В случае успеха вызывается функтор с переданными аргументами. В случае неудачи возвращается Result().
         */
        template<typename Result, typename... Args>
        inline Result create(Type* type, const Args&... args) const
        {
            if(nullptr == type) return Result();
            return create<Result, Args...>(type->getHash(), args...);
        }

        /**
         * @brief Создание экземпляра класса по хэшу типа.
         * @param[in] hash Хеш типа.
         * @param[in] args Аргументы функтора создания.
         * @return Возвращает экземпляр класса с типом из параметра шаблона Result.
         * @details Создание экземпляра класса по хэшу типа из фабрики.
         * В случае успеха вызывается функтор с переданными аргументами. В случае неудачи возвращается Result().
         */
        template<typename Result, typename... Args>
        Result create(size_t hash, const Args&... args) const
        {
            auto typeWithCreator = static_cast<TypeWithCreator<Result, Args...>*>(types[hash]);

            if(nullptr == typeWithCreator)
                return Result();
            else
                if(nullptr == typeWithCreator->creator) return Result();

            return typeWithCreator->creator(args...);
        }

        /**
         * @brief Создание экземпляра класса по названию типа.
         * @param[in] name Название типа.
         * @param[in] args Аргументы функтора создания.
         * @return Возвращает экземпляр класса с типом из параметра шаблона Result.
         * @details Создание экземпляра класса по хэшу типа из фабрики.
         * В случае успеха вызывается функтор с переданными аргументами. В случае неудачи возвращается Result().
         */
        template<typename Result, typename... Args>
        Result create(const String& name, const Args&... args) const
        {
            auto typeWithCreator = static_cast<TypeWithCreator<Result, Args...>*>(types.template get<NAME>(name));

            if(nullptr == typeWithCreator)
                return Result();
            else
                if(nullptr == typeWithCreator->creator) return Result();

            return typeWithCreator->creator(args...);
        }

        /**
         * @brief Удаление типа из фабрики по ссылке на него.
         * @param[in] type Ссылка на тип.
         * @return В случае успеха true, иначе false.
         */
        inline bool remove(const Type& type)
        {
            return remove(type.getHash());
        }

        /**
         * @brief Удаление типа из фабрики по указателю на него.
         * @param[in] type Указатель на тип.
         * @return В случае успеха true, иначе false.
         */
        inline bool remove(Type* type)
        {
            if(nullptr == type) return false;
            return remove(type->getHash());
        }

        /**
         * @brief Удаление типа из фабрики по его хешу.
         * @param[in] hash Хеш типа.
         * @return В случае успеха true, иначе false.
         */
        bool remove(size_t hash)
        {
            Type* type = types[hash];
            if(type != nullptr) delete type;
            return types.remove(hash);
        }

        /**
         * @brief Удаление типа из фабрики по его названию.
         * @param[in] name Название типа.
         * @return В случае успеха true, иначе false.
         */
        bool remove(const String& name)
        {
            Type* type = types.template get<NAME>(name);
            if(type != nullptr) delete type;
            return types.template remove<NAME>(name);
        }

    };
}
#endif // FRFACTORY_H
