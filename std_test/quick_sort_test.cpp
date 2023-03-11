#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

template <typename It, typename Compare> void QuickSort(It first, It last, Compare com)
{
    if (first == last)
    {
        return;
    }

    auto pivot = *std::next(first, std::distance(first, last) / 2);
    auto mid1 = std::partition(first, last, [&pivot, &com](auto &e) { return com(e, pivot); });
    auto mid2 = std::partition(mid1, last, [&pivot](auto &e) { return pivot == e; });
    QuickSort(first, mid1, com);
    QuickSort(mid2, last, com);
}

template <typename It> void QuickSort(It first, It last)
{
    using ElmT = typename std::iterator_traits<It>::value_type;
    QuickSort(first, last, std::greater<ElmT>{});
}

int main()
{
    std::vector<int> v1{1, 2, 3, 4, 5};
    std::copy_if(std::begin(v1), std::end(v1), std::ostream_iterator<int>(std::cout, ", "),
                 std::bind(std::greater<int>{}, std::placeholders::_1, 2));
    std::cout << std::endl;

    std::vector<int> v2 = {8, 2, 64, -87, 61, -42, 64, 0, 45, 3, 1, 7, 5, -4, 6};
    QuickSort(std::begin(v2), std::end(v2));
    std::copy(std::begin(v2), std::end(v2), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
}