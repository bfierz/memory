// Copyright (C) 2015 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef FOONATHAN_MEMORY_NEW_ALLOCATOR_HPP_INCLUDED
#define FOONATHAN_MEMORY_NEW_ALLOCATOR_HPP_INCLUDED

/// \file
/// \brief A new allocator.

#include <type_traits>

#include "config.hpp"

namespace foonathan { namespace memory
{
#if FOONATHAN_MEMORY_DEBUG_LEAK_CHECK
    namespace detail
    {
        static struct new_allocator_leak_checker_initializer_t
        {
            new_allocator_leak_checker_initializer_t() FOONATHAN_NOEXCEPT;
            ~new_allocator_leak_checker_initializer_t() FOONATHAN_NOEXCEPT;
        } new_allocator_leak_checker_initializer;
    } // namespace detail
#endif

    /// \brief A \ref concept::RawAllocator that allocates memory using \c new.
    ///
    /// It is no singleton but stateless; each instance is the same.
    /// \ingroup memory
    class new_allocator
    {
    public:
        using is_stateful = std::false_type;

        new_allocator() FOONATHAN_NOEXCEPT = default;
        new_allocator(new_allocator&&) FOONATHAN_NOEXCEPT {}
        ~new_allocator() FOONATHAN_NOEXCEPT = default;

        new_allocator& operator=(new_allocator &&) FOONATHAN_NOEXCEPT
        {
            return *this;
        }

        /// \brief Allocates memory using \c ::operator \c new.
        /// \details It uses the nothrow version.
        /// In case of \c nullptr, it loops calling \c std::new_handler
        /// as usual but if the handler is \c null,
        /// it calls \ref out_of_memory_handler prior to throwing \ref out_of_memory.
        void* allocate_node(std::size_t size, std::size_t alignment);

        /// \brief Deallocates memory using \c ::operator \c delete.
        void deallocate_node(void *node, std::size_t size, std::size_t alignment) FOONATHAN_NOEXCEPT;

        /// \brief Maximum node size.
        /// \details It forwards to \c std::allocator<char>::max_size(),
        /// or on a freestanding implementation returns the maximum value.
        std::size_t max_node_size() const FOONATHAN_NOEXCEPT;
    };
}} // namespace foonathan::memory

#endif // FOONATHAN_MEMORY_NEW_ALLOCATOR_HPP_INCLUDED
