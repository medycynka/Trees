//
// Created by PC on 16.11.2022.
//

#ifndef TREES_REVERSE_ITERATOR_HPP
#define TREES_REVERSE_ITERATOR_HPP

#pragma once

#include "base_iterators.hpp"

namespace tree::iterators {

    template <typename NODE>
    class ReverseIterator : public BaseReverseIterator<NODE, false> {
        using super = BaseReverseIterator<NODE, false>;

    public:
        using super::super;
        typedef ReverseIterator self_type;

        ReverseIterator& operator= (const ReverseIterator& source)       { if (this != &source) { this->Iter = source.Iter; } return (*this); };
        ReverseIterator& operator= (ReverseIterator&& source)   noexcept { this->Iter = source.Iter; return (*this); };
        bool             operator==(const ReverseIterator& source) const { return (this->Iter == source.Iter); };
        bool             operator!=(const ReverseIterator& source) const { return (this->Iter != source.Iter); };
    };

}

#endif //TREES_REVERSE_ITERATOR_HPP
