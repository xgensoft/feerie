/**
 * @file
 * @brief Заголовочный файл с классом типа для фабрики движка.
 * @author Igor Filatov
 */
#ifndef FRTYPE_H
#define FRTYPE_H

#include <functional>

namespace feerie
{
    /**
     * @brief Шаблон класса типа.
     * @details Шаблон класса типа для фабрики движка. Содержит название и хэш от него (для более быстрого поиска).
     */
    template<class String>
    class FrType
    {
    public:

        /**
         * @brief Конструктор FrType.
         * @param[in] name Название типа.
         */
        FrType(const String& name) :
            name(name),
            hash(std::hash<String>()(name))
        {
        }

        /**
         * @brief Виртуальный деструктор FrType.
         */
        virtual ~FrType()
        {

        }

        /**
         * @brief Возвращает ссылку на название типа.
         * @param[in] key Ключ.
         * @return Ссылка на название.
         */
        inline const String& getName() const
        {
            return name;
        }

        /**
         * @brief Возвращает хэш из названия типа.
         * @return хэш.
         */
        inline size_t getHash() const
        {
            return hash;
        }

        /**
         * @brief Операция сравнения.
         * @param[in] type Тип с которым сравнивать.
         * @return Если хеш меньше хеша из аргумента type, то возвращает true, иначе false.
         * @details Операции сравнения типов нужны для использования в коллекциях.
         */
        inline bool operator< (const FrType& type) const
        {
            return hash < type.hash;
        }

        /**
         * @brief Операция сравнения.
         * @param[in] type Тип с которым сравнивать.
         * @return Если хеш равен хешу из аргумента type, то возвращает true, иначе false.
         * @details Операции сравнения типов нужны для использования в коллекциях.
         */
        inline bool operator== (const FrType& type) const
        {
            return hash == type.hash;
        }

        /**
         * @brief Операция сравнения.
         * @param[in] type Тип с которым сравнивать.
         * @return Если хеш не равен хешу из аргумента type, то возвращает true, иначе false.
         * @details Операции сравнения типов нужны для использования в коллекциях.
         */
        inline bool operator!= (const FrType& type) const
        {
            return hash != type.hash;
        }

    private:

        const String name;
        const size_t hash;

    };
}
#endif // FRTYPE_H
