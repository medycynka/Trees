//
// Created by PC on 16.11.2022.
//

#ifndef TREES_ITERATOR_HPP
#define TREES_ITERATOR_HPP

#pragma once

#include "base_iterators.hpp"

namespace tree::iterators {

    template <typename NODE>
    class Iterator : public BaseIterator<NODE, false> {
        using super = BaseIterator<NODE, false>;

    public:
        using super::super;
        typedef Iterator self_type;

        Iterator& operator= (const Iterator& source)       { if (this != &source) { this->Iter = source.Iter; } return (*this); };
        Iterator& operator= (Iterator&& source)   noexcept { this->Iter = source.Iter; return (*this); };
        bool      operator==(const Iterator& source) const { return (this->Iter == source.Iter); };
        bool      operator!=(const Iterator& source) const { return (this->Iter != source.Iter); };
    };

}

#endif //TREES_ITERATOR_HPP
