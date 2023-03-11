#include "unique_ptr.hpp"
#include <ios>
#include <iostream>

class A
{
  public:
    A(int a, int b) : a(a), b(b)
    {
    }
    int a;
    int b;
};

int main()
{
    xs::unique_ptr<A> a_ptr = xs::make_unique<A>(1, 2);
    std::cout << a_ptr->a << " " << a_ptr.get()->b << "\n";
    xs::unique_ptr<A> b_ptr(a_ptr.release());
    std::cout << std::boolalpha << (a_ptr.get() == nullptr) << "\n";
    std::cout << b_ptr->a << " " << b_ptr.get()->b << "\n";

    return 0;
}