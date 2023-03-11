#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

/* 使用默认构造器和删除器，即通过new申请内存，通过delete释放内存 */

namespace xs
{
template <typename T> class unique_ptr
{
    using Pointer = T *;
    using Deleter = std::function<void(Pointer)>;

  public:
    unique_ptr() : ptr_(nullptr)
    {
    }

    unique_ptr(std::nullptr_t) : ptr_(nullptr)
    {
    }

    unique_ptr(
        Pointer ptr, Deleter &&del = [](Pointer ptr) { delete ptr; })
        : ptr_(ptr), del_(del)
    {
    }

    unique_ptr(const unique_ptr &) = delete;

    unique_ptr &operator=(const unique_ptr &) = delete;

    unique_ptr &operator=(T &&right)
    {
        reset(right.get());
        del_ = right.get_deleter();
        return *this;
    }

    ~unique_ptr()
    {
        if (ptr_ != nullptr)
        {
            del_(ptr_);
        }
    }

    Pointer operator->()
    {
        return ptr_;
    }

    Pointer operator*()
    {
        return ptr_;
    }

    operator bool()
    {
        return ptr_ == nullptr;
    }

    Pointer release()
    {
        return std::exchange(ptr_, nullptr);
    }

    Pointer get()
    {
        return ptr_;
    }

    Deleter &get_deleter()
    {
        return del_;
    }

    void reset(Pointer ptr = nullptr)
    {
        Pointer old = std::exchange(ptr_, ptr);
        if (old != nullptr)
        {
            del_(ptr_);
        }
    }

    void swap(unique_ptr &right)
    {
        std::swap(get(), right.get());
    }

  private:
    Pointer ptr_;
    Deleter del_;
};

template <typename T, typename... Args> unique_ptr<T> make_unique(Args &&...args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
} // namespace xs