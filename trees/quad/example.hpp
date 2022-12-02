//
// Created by PC on 16.11.2022.
//

#ifndef TREES_EXAMPLE_HPP
#define TREES_EXAMPLE_HPP

#include <chrono>
#include <iostream>
#include <random>
#include "quad.hpp"

template<typename T = float>
struct BoxNode
{
    tree::quad::Box<T> box;
    std::size_t id{};
};

template<typename T = float>
std::vector<BoxNode<T>> generateRandomNodes(std::size_t n)
{
    auto generator = std::default_random_engine();
    auto originDistribution = std::uniform_real_distribution(0.0f, 1.0f);
    auto sizeDistribution = std::uniform_real_distribution(0.0f, 0.01f);
    auto nodes = std::vector<BoxNode<T>>(n);
    
    for (auto i = std::size_t(0); i < n; ++i)
    {
        nodes[i].box.left = originDistribution(generator);
        nodes[i].box.top = originDistribution(generator);
        nodes[i].box.width = std::min(1.0f - nodes[i].box.left, sizeDistribution(generator));
        nodes[i].box.height = std::min(1.0f - nodes[i].box.top, sizeDistribution(generator));
        nodes[i].id = i;
    }
    
    return nodes;
}

template<typename T = float>
void checkIntersections(std::vector<BoxNode<T>*> nodes1, std::vector<BoxNode<T>*> nodes2)
{
    if (nodes1.size() == nodes2.size()) {
        std::sort(std::begin(nodes1), std::end(nodes1));
        std::sort(std::begin(nodes2), std::end(nodes2));
        for (auto i = std::size_t(0); i < nodes1.size(); ++i)
            if (nodes1[i] != nodes2[i]){
                std::cout << "Different nodes at index " << i << ": " << nodes1[i]->box << ", " << nodes2[i]->box << "\n";
            }
    } else {
        std::cout << "Different sizes!" << "\n";
    }
}

void example(){
    auto n = std::size_t(1000);
    auto getBox = [](BoxNode<float>* node)
    {
        return node->box;
    };
    auto box = tree::quad::Box(0.0f, 0.0f, 1.0f, 1.0f);
    auto nodes = generateRandomNodes<float>(n);
    // Add nodes to quadtree
    auto quadtree = tree::quad::Quadtree<BoxNode<float>*, decltype(getBox)>(box, getBox);
    auto start1 = std::chrono::steady_clock::now();
    for (auto& node : nodes)
        quadtree.add(&node);
    // Randomly remove some nodes
    auto generator = std::default_random_engine();
    auto deathDistribution = std::uniform_int_distribution(0, 1);
    auto removed = std::vector<bool>(nodes.size(), false);
    std::generate(std::begin(removed), std::end(removed),
                  [&generator, &deathDistribution](){ return deathDistribution(generator); });
    for (auto& node : nodes)
    {
        if (removed[node.id])
            quadtree.remove(&node);
    }
    // Quadtree
    auto intersections1 = std::vector<std::vector<BoxNode<float>*>>(nodes.size());
    auto start2 = std::chrono::steady_clock::now();
    for (const auto& node : nodes)
    {
        if (!removed[node.id])
            intersections1[node.id] = quadtree.query(node.box);
    }
    auto duration2 = std::chrono::steady_clock::now() - start2;
    auto duration1 = std::chrono::steady_clock::now() - start1;
    std::cout << "quadtree: " << std::chrono::duration_cast<std::chrono::microseconds>(duration2).count() << "us" << '\n';
    std::cout << "quadtree with creation: " << std::chrono::duration_cast<std::chrono::microseconds>(duration1).count() << "us" << '\n';
}

#endif //TREES_EXAMPLE_HPP
