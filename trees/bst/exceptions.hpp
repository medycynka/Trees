//
// Created by PC on 16.11.2022.
//

#ifndef TREES_EXCEPTIONS_HPP
#define TREES_EXCEPTIONS_HPP

#pragma once

#include <exception>

namespace tree::exception {
    struct NodeIndexOutOfBoundException : public std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Out of bound! Tried to get element at index bigger than 2 or smaller than 0!";
        }
    };

    struct NodeFailedAllocException : public std::bad_alloc {
        [[nodiscard]] const char* what() const noexcept override {
            return "Error during creating new node.";
        }
    };

    struct TreeIndexOutOfBoundException : public std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Out of bound! Tried to get element at index bigger than size of tree or smaller than 0!";
        }
    };

    struct TreeInsertionFailedException : public std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Failed during insertion.";
        }
    };

    struct TreeReplaceException : public std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Tried to replace for an element that would destroy the balance of a tree.";
        }
    };
}


#endif //TREES_EXCEPTIONS_HPP
