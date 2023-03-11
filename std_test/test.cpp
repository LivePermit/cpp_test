/*
有n个整数，从中抽取4次（每次抽完后，放回该数），请写一个函数，判断抽取的4个数之和是否存在等于m的可能性，
可能返回true，不可能返回false；其中 0 < n < 1000,用你能找到的效率最高的方法。
如：n = 3， a[] = {1,3,5}, m = 10, 返回true;
n = 3， a[] = {1,3,5}, m = 9,  返回false

bool  Judge(int m, int n, int* a)
*/

#include <iostream>
#include <unordered_set>
#include <vector>

bool Judge(int m, std::vector<int> a)
{
    size_t n = a.size();
    std::unordered_set<int> twoNum;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i; j < n; j++)
        {
            twoNum.insert(a[i] + a[j]);
        }
    }
    for (auto i : twoNum)
    {
        if (twoNum.find(m - i) != twoNum.end())
        {
            return true;
        }
    }
    return false;
}

int main()
{
    std::cout << Judge(10, {1, 3, 5}) << std::endl;
    std::cout << Judge(9, {1, 3, 5}) << std::endl;
}