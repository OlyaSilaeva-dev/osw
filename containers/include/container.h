#ifndef OSW_CONTAINER_H
#define OSW_CONTAINER_H

#include "allocator_global_heap.h"
#include "allocator_sorted_list.h"
#include "allocator_boundary_tags.h"
#include "allocator_buddies_system.h"
#include "b_tree.h"

namespace cntr
{

    enum class allocator_type
    {
        Boundary_Tags,
        Buddies,
        Global_Heap,
        Sorted_List
    };

    template<typename tkey, typename tvalue>
    class container : public b_tree<tkey, tvalue>
    {

    protected:

        allocator_type _allocator_type;

    public:

        explicit container(
                size_t t,
                std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
                allocator *allocator = nullptr,
                allocator_type type = allocator_type::Boundary_Tags,
                logger *logger = nullptr);

    public:

        virtual std::string get_typename()
        {
            return "container";
        }

        ~container() = default;

        container(container const & other) = default;

        container &operator=(container const & other) = default;

        container(container && other) noexcept = default;

        container &operator=(container && other) noexcept = default;

    };

    template <typename tkey, typename tvalue>
    inline container<tkey, tvalue>::container(size_t t, std::function<int(tkey const &, tkey const &)> keys_comparer, allocator *allocator, cntr::allocator_type type, logger *logger):
            b_tree<tkey, tvalue>(t, keys_comparer, allocator, logger), _allocator_type(type)
    {}

};

#endif