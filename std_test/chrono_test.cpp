#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>

volatile int sink;

inline constexpr uint32_t test = 0;
int main()
{
    for (auto size = 1ull; size < 1000000000ull; size *= 100) {
        // 记录开始时间
        auto start = std::chrono::system_clock::now();
        // 做一些工作
        std::vector<int> v(size, 42);
        sink = std::accumulate(v.begin(), v.end(), 0u); // 确保其副效应
        // 记录结束时间
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << "Time to fill and iterate a vector of " 
                  << size << " ints : " << diff.count() << " s\n";
    }
}
