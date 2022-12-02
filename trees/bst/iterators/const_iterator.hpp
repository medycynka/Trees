//
// Created by PC on 16.11.2022.
//

#ifndef TREES_CONST_ITERATOR_HPP
#define TREES_CONST_ITERATOR_HPP

#pragma once

#include "base_iterators.hpp"

namespace tree::iterators {

    template <typename NODE>
    class ConstIterator : public BaseIterator<NODE, true> {
        using super = BaseIterator<NODE, true>;

    public:
        using super::super;
        typedef ConstIterator self_type;

        ConstIterator& operator= (const ConstIterator& source)        { if (this != &source) { this->Iter = source.Iter; } return (*this); };
        ConstIterator& operator= (const Iterator<NODE>& source)       { if (this != &source) { this->Iter = source.getIter(); } return (*this); };
        ConstIterator& operator= (ConstIterator&& source)    noexcept { this->Iter = source.Iter; return (*this); };
        ConstIterator& operator= (Iterator<NODE>&& source)   noexcept { this->Iter = source.getIter(); return (*this); };
        bool           operator==(const ConstIterator& source)  const { return (this->Iter == source.Iter); };
        bool           operator==(const Iterator<NODE>& source) const { return (this->Iter == source.getIter()); };
        bool           operator!=(const ConstIterator& source)  const { return (this->Iter != source.Iter); };
        bool           operator!=(const Iterator<NODE>& source) const { return (this->Iter != source.getIter()); };
    };

}

#endif //TREES_CONST_ITERATOR_HPP
