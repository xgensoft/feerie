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
     * @brief Шаблон класса листа дерева.
     * @details Шаблон класса листа корневого дерева. Содержит связь с предком.
     */
    class FrLeaf
    {

        friend class FrNode;

    public:

        inline FrNode* getParent() const
        {
            return parent;
        }

    protected:

        FrNode* parent;

    };

    /**
     * @brief Шаблон класса листа дерева содержащий данные.
     * @details Шаблон класса листа корневого дерева. Содержит данные и связь с предком.
     */
    template<class Data>
    class FrDataLeaf : FrLeaf
    {

    public:

        Data data;

    };
}

#endif // FRLEAF_H
