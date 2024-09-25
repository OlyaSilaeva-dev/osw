#ifndef OSW_COLLECTION_H
#define OSW_COLLECTION_H

#include "container.h"
using namespace cntr;

template<typename tkey, typename tvalue>
class collection final :container<tkey, tvalue>
{

public:

    explicit collection(
        size_t t,
        std::function<int(tkey const &, tkey const &)> keys_comparer = std::less<tkey>(),
        allocator *allocator = nullptr,
        allocator_type type = allocator_type::Boundary_Tags,
        logger *logger = nullptr);

    ~collection() = default;

    collection(collection const & other) = default;

    collection &operator=(collection const & other) = default;

    collection(collection && other) noexcept = default;

    collection &operator=(collection && other) noexcept = default;

    std::string get_typename()
    {
        return "collection";
    }

};

template <typename tkey, typename tvalue>
inline collection<tkey, tvalue>::collection(
    size_t t, 
    std::function<int(tkey const &, tkey const &)> keys_comparer, 
    allocator *allocator,
    allocator_type type,
    logger *logger):
        b_tree<tkey, tvalue>(t, keys_comparer, allocator, type, logger) {}

#endif //OSW_COLLECTION_H