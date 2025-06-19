#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <iterator>
#include <random>

const int THREAD_THRESHOLD = 1000;

void parallel_merge(const std::vector<int>& left, const std::vector<int>& right, std::vector<int>& result, size_t start = 0) {
    size_t n1 = left.size(), n2 = right.size();

    // Base case: use std::merge for small inputs
    if (n1 + n2 <= THREAD_THRESHOLD) {
        std::merge(left.begin(), left.end(), right.begin(), right.end(), result.begin() + start);
        return;
    }

    // Ensure left is larger
    if (n1 < n2) {
        parallel_merge(right, left, result, start);
        return;
    }

    // Divide left in half
    size_t mid1 = n1 / 2;
    int midVal = left[mid1];

    // Binary search in right to find position of midVal
    auto it = std::lower_bound(right.begin(), right.end(), midVal);
    size_t mid2 = std::distance(right.begin(), it);

    // Final position in result
    size_t mid_res = start + mid1 + mid2;
    result[mid_res] = midVal;

    std::vector<int> left1(left.begin(), left.begin() + mid1);
    std::vector<int> right1(right.begin(), right.begin() + mid2);

    std::vector<int> left2(left.begin() + mid1 + 1, left.end());
    std::vector<int> right2(right.begin() + mid2, right.end());

    std::thread t1([&]() {
        parallel_merge(left1, right1, result, start);
    });
    std::thread t2([&]() {
        parallel_merge(left2, right2, result, mid_res + 1);
    });

    t1.join();
    t2.join();
}

int main() {
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    std::uniform_int_distribution<int> dist {1, 52};
    
    auto gen = [&](){
                   return dist(mersenne_engine);
               };

    std::vector<int> A(10), B(10);
    std::generate(A.begin(), A.end(), gen);
    std::sort(A.begin(), A.end());
    std::cout << "Array A: ";
    for (int x : A)
        std::cout << x << " ";
    std::cout << "\n";

    std::generate(B.begin(), B.end(), gen);
    std::sort(B.begin(), B.end());
    std::cout << "Array B: ";
    for (int x : B)
        std::cout << x << " ";

    std::vector<int> result(A.size() + B.size());

    parallel_merge(A, B, result);

    std::cout << "\nMerged Array: ";
    for (int x : result)
        std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
