#include <not_implemented.h>

#include "allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger) : _logger(logger)
{
    if (_logger)
        _logger->debug("allocator_global_heap constructor called");
}

allocator_global_heap::~allocator_global_heap()
{
    if(_logger)
        _logger->debug("allocator_global_heap destructor called");
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept : _logger(other._logger)
{
    if (this != &other)
    {
        other._logger = nullptr;
    }
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    if(this != &other)
    {
        _logger = other._logger;
        other._logger = nullptr;
    }
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    if(_logger)
        _logger->debug("allocate called");

    if (value_size == 0 || values_count == 0)
    {
        if (_logger)
            _logger->debug("Attempt to allocate memory with zero size or count");
        return nullptr;
    }

    void *ptr = nullptr;
    try {
          ptr = ::operator new(value_size * values_count);
    } catch (std::bad_alloc&) {
        if(_logger)
            _logger->debug("Failed to allocate memory");
        throw;
    }

    if(_logger)
        _logger->debug("Memory allocation requested");
    return ptr;
}

void allocator_global_heap::deallocate(
    void *at)
{
    if (_logger)
        _logger->debug("deallocate called");

    if (!at) return;

    ::operator delete(at);
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return "allocator_global_heap";
}