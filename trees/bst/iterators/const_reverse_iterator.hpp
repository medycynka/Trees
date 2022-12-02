//
// Created by PC on 16.11.2022.
//

#ifndef TREES_CONST_REVERSE_ITERATOR_HPP
#define TREES_CONST_REVERSE_ITERATOR_HPP

#pragma once

#include "base_iterators.hpp"

namespace tree::iterators {

    template <typename NODE>
    class ConstReverseIterator : public BaseReverseIterator<NODE, true> {
        using super = BaseReverseIterator<NODE, true>;

    public:
        using super::super;
        typedef ConstReverseIterator self_type;

        ConstReverseIterator& operator= (const ConstReverseIterator& source)        { if (this != &source) { this->Iter = source.Iter; } return (*this); };
        ConstReverseIterator& operator= (const ReverseIterator<NODE>& source)       { if (this != &source) { this->Iter = source.getIter(); } return (*this); };
        ConstReverseIterator& operator= (ConstReverseIterator&& source)    noexcept { this->Iter = source.Iter; return (*this); };
        ConstReverseIterator& operator= (ReverseIterator<NODE>&& source)   noexcept { this->Iter = source.getIter(); return (*this); };
        bool                  operator==(const ConstReverseIterator& source)  const { return (this->Iter == source.Iter); };
        bool                  operator==(const ReverseIterator<NODE>& source) const { return (this->Iter == source.getIter()); };
        bool                  operator!=(const ConstReverseIterator& source)  const { return (this->Iter != source.Iter); };
        bool                  operator!=(const ReverseIterator<NODE>& source) const { return (this->Iter != source.getIter()); };
    };

}

#endif //TREES_CONST_REVERSE_ITERATOR_HPP
