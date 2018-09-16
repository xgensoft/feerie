/**
 * @file
 * @brief Заголовочный файл с классом узла дерева.
 * @author Igor Filatov
 */
#ifndef FRNODE_H
#define FRNODE_H

#include <forward_list>

#include "frleaf.h"

namespace feerie
{
    /**
     * @brief Класса узла дерева.
     * @details Класса узла дерева, наследуется от листа и содержит односвязный список дочерних листов и узлов.
     */
    class FrNode : FrLeaf
    {
    public:

        FrNode() :
            FrLeaf(),
            leaves()
        {
        }

        inline const std::forward_list<FrLeaf*>& getLeaves() const
        {
            return leaves;
        }

        inline void add(FrLeaf* leaf)
        {
            if(nullptr != leaf)
            {
                leaves.push_front(leaf);
                leaf->parent = this;
            }
        }

        inline void remove(FrLeaf* leaf)
        {
            if(nullptr != leaf)
            {
                leaves.remove(leaf);
                leaf->parent = nullptr;
            }
        }

        void clear()
        {
            for(auto& leaf : leaves)
            {
                if(nullptr == leaf) continue;
                leaf->parent = nullptr;
            }

            leaves.clear();
        }

    private:

        std::forward_list<FrLeaf*> leaves;

    };
}

#endif // FRNODE_H
