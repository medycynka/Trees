#include <iostream>
#include <chrono>
#include <random>
#include <functional>
#include <string>
#include <limits>
#include "trees/rbt/rbt.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    constexpr int t1 = 1000;
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<> distribution(-25, 25);

    {
        auto start = std::chrono::steady_clock::now();
        auto tree = tree::rbt::RBTree<int>();
        for (auto i = 0; i < t1; ++i) {
            tree.insert(i);
        }
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
        std::cout << "Inserting " << t1 << " elements took " << duration << "us" << '\n';
    }
    {
        std::size_t mask = std::numeric_limits<int>::max();
        std::cout << "Mask: " << mask << "\n";
        std::cout << 213 << " " << std::hash<int>{}(213) % mask << "\n";
        std::cout << 1 << " " <<  std::hash<int>{}(1) % mask << "\n";
        std::cout << 23 << " " << std::hash<int>{}(23) % mask << "\n";
        std::cout << 23.0f << " " << std::hash<float>{}(23.0f) % mask << "\n";
        std::cout << 23.0 << " " << std::hash<double>{}(23.0) % mask << "\n";
        std::cout << 0.0321 << " " << std::hash<double>{}(0.0321) % mask << "\n";
        std::cout << 2.0321 << " " << std::hash<double>{}(2.0321) % mask << "\n";
        std::cout << "2.0321" << " " << std::hash<std::string>{}("2.0321") % mask << "\n";
        std::cout << "absdf" << " " << std::hash<std::string>{}("absdf") % mask << "\n";
        std::cout << "sdfab" << " " << std::hash<std::string>{}("sdfab") % mask << "\n";
        std::cout << (int)(std::numeric_limits<char>::max()) << "\n";
        std::cout << (int)(std::numeric_limits<unsigned char>::max()) << "\n";
    }
    {
        unsigned char test_t[] = {1, 2, 43};

        for (unsigned char i : test_t) {
            std::cout << i << ", ";
        }
        std::cout << "\n";

        test_t[1] = 12;
        test_t[2]++;

        for (unsigned char i : test_t) {
            std::cout << i << ", ";
        }
        std::cout << "\n";
    }

    return 0;
}
