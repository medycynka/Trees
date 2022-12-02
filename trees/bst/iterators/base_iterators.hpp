//
// Created by PC on 22.11.2022.
//

#ifndef TREES_BASE_ITERATORS_HPP
#define TREES_BASE_ITERATORS_HPP

#pragma once

#include "../bst_node.hpp"

namespace tree::iterators {

    template <typename NODE, bool Const>
    class BaseIterator {
    protected:
        NODE *Iter;

    public:
        typedef typename NODE::value_type       value_type;
        typedef typename NODE::reference        reference;
        typedef typename NODE::pointer          pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef int                             difference_type;
        typedef typename std::conditional<Const, typename NODE::const_reference, reference>::type ref_type;
        typedef typename std::conditional<Const, typename NODE::const_pointer, pointer>::type ptr_type;

        BaseIterator()                           : Iter{ nullptr } {}
        explicit BaseIterator(NODE* ptr)         : Iter{ ptr } {};
        BaseIterator(const BaseIterator& s)      : Iter{ s.Iter } {};
        BaseIterator(BaseIterator&& s) noexcept  : Iter{ std::move(s.Iter) } {};

        NODE* getIter()       { return Iter; };
        NODE* getIter() const { return Iter; };

        BaseIterator&       operator++           ();
        const BaseIterator& operator++           (int);
        BaseIterator&       operator--           ();
        const BaseIterator <NODE, Const> & operator--(int);
        BaseIterator&       operator=            (const BaseIterator& source)              { if (this != &source) { this->Iter = source.Iter; } return (*this); };
        BaseIterator&       operator=            (BaseIterator&& source)          noexcept { this->Iter = source.Iter; return (*this); };
        bool                operator==           (const BaseIterator& source)        const { return (Iter == source.Iter); };
        bool                operator!=           (const BaseIterator& source)        const { return (Iter != source.Iter); };
        explicit            operator NODE&       ()                                        { return (*Iter); };
        explicit            operator const NODE& ()                                  const { return (*Iter); };
        ref_type            operator*            ()                                  const { return (Iter->key); };
        ptr_type            operator->           ()                                  const { return Iter; };
        explicit            operator bool        ()                                  const { return (Iter != nullptr); };
    };

    template <typename T, bool Const>
    BaseIterator<T, Const>& BaseIterator<T, Const>::operator++() {
        if (this->Iter != nullptr) {
            this->Iter = this->Iter->node_Successor();
        }

        return *this;
    }

    template <typename T, bool Const>
    const BaseIterator<T, Const>& BaseIterator<T, Const>::operator++(int) {
        BaseIterator pom = *this;
        ++(*this);

        return pom;
    }

    template <typename T, bool Const>
    BaseIterator<T, Const>& BaseIterator<T, Const>::operator--() {
        if (this->Iter != nullptr) {
            this->Iter = this->Iter->node_Predecessor();
        }

        return *this;
    }

    template <typename T, bool Const>
    const BaseIterator <T, Const> & BaseIterator<T, Const>::operator--(int) {
        BaseIterator pom = *this;
        --(*this);

        return pom;
    }

    template <typename NODE, bool Const>
    class BaseReverseIterator {
    protected:
        NODE *Iter;

    public:
        typedef typename NODE::value_type       value_type;
        typedef typename NODE::reference        reference;
        typedef typename NODE::pointer          pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef int                             difference_type;
        typedef typename std::conditional<Const, typename NODE::const_reference, reference>::type ref_type;
        typedef typename std::conditional<Const, typename NODE::const_pointer, pointer>::type ptr_type;

        BaseReverseIterator()                                  : Iter{ nullptr } {}
        explicit BaseReverseIterator(NODE* ptr)                : Iter{ ptr } {};
        BaseReverseIterator(const BaseReverseIterator& s)      : Iter{ s.Iter } {};
        BaseReverseIterator(BaseReverseIterator&& s) noexcept  : Iter{ std::move(s.Iter) } {};

        NODE* getIter()       { return Iter; };
        NODE* getIter() const { return Iter; };

        BaseReverseIterator&       operator++    ();
        const BaseReverseIterator& operator++    (int);
        BaseReverseIterator&       operator--    ();
        const BaseReverseIterator <NODE, Const> & operator--(int);
        BaseReverseIterator&       operator=     (const BaseReverseIterator& source)       { if (this != &source) { this->Iter = source.Iter; } return (*this); };
        BaseReverseIterator&       operator=     (BaseReverseIterator&& source)   noexcept { this->Iter = source.Iter; return (*this); };
        bool                operator==           (const BaseReverseIterator& source) const { return (Iter == source.Iter); };
        bool                operator!=           (const BaseReverseIterator& source) const { return (Iter != source.Iter); };
        explicit            operator NODE&       ()                                        { return (*Iter); };
        explicit            operator const NODE& ()                                  const { return (*Iter); };
        ref_type            operator*            ()                                  const { return (Iter->key); };
        ptr_type            operator->           ()                                  const { return Iter; };
        explicit            operator bool        ()                                  const { return (Iter != nullptr); };
    };

    template <typename T, bool Const>
    BaseReverseIterator<T, Const>& BaseReverseIterator<T, Const>::operator++() {
        if (this->Iter != nullptr) {
            this->Iter = this->Iter->node_Successor();
        }

        return *this;
    }

    template <typename T, bool Const>
    const BaseReverseIterator<T, Const>& BaseReverseIterator<T, Const>::operator++(int) {
        BaseReverseIterator pom = *this;
        ++(*this);

        return pom;
    }

    template <typename T, bool Const>
    BaseReverseIterator<T, Const>& BaseReverseIterator<T, Const>::operator--() {
        if (this->Iter != nullptr) {
            this->Iter = this->Iter->node_Predecessor();
        }

        return *this;
    }

    template <typename T, bool Const>
    const BaseReverseIterator <T, Const> & BaseReverseIterator<T, Const>::operator--(int) {
        BaseReverseIterator pom = *this;
        --(*this);

        return pom;
    }

}

#endif //TREES_BASE_ITERATORS_HPP
