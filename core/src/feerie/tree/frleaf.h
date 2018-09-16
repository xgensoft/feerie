/**
 * @file
 * @brief Заголовочный файл с классы листа дерева.
 * @author Igor Filatov
 */
#ifndef FRLEAF_H
#define FRLEAF_H

namespace feerie
{
    class FrNode;

    /**
     * @brief Класса листа дерева.
     * @details Класса листа корневого дерева. Содержит связь с предком и флаги.
     * Базовый класс для листов и узлов.
     */
    class FrLeaf
    {

        friend class FrNode;

    public:

        FrLeaf() : parent(nullptr)
        {
        }

        inline FrNode* getParent() const
        {
            return parent;
        }

        inline size_t getFlags() const
        {
            return flags;
        }

    protected:

        FrNode* parent;
        size_t flags;

    };

    /**
     * @brief Шаблон класса листа дерева содержащий данные.
     * @details Шаблон класса листа корневого дерева. Содержит данные и связь с предком.
     */
    template<class Data>
    class FrDataLeaf : FrLeaf
    {
    public:

        FrDataLeaf() :
            FrLeaf(),
            data()
        {
        }

        Data data;

    };
}

#endif // FRLEAF_H
