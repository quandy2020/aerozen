/*
 * Copyright (C) 2026 duyongquan <quandy2020@126.com>
 */

#ifndef AEROZEN_UTILS__NEVER_DESTROYED_HPP_
#define AEROZEN_UTILS__NEVER_DESTROYED_HPP_

#include <new>
#include <type_traits>
#include <utility>

namespace aerozen {
namespace utils {

template <typename T>
class NeverDestroyed
{
public:
    NeverDestroyed() {
        new (&this->storage_) T();
    }

    template <typename... Args>
    explicit NeverDestroyed(Args&&... _args) {
        new (&this->storage_) T(std::forward<Args>(_args)...);
    }

    NeverDestroyed(const NeverDestroyed&) = delete;
    NeverDestroyed& operator=(const NeverDestroyed&) = delete;
    NeverDestroyed(NeverDestroyed&&) = delete;
    NeverDestroyed& operator=(NeverDestroyed&&) = delete;

    ~NeverDestroyed() = default;

    T& Access() {
        return *reinterpret_cast<T*>(&this->storage_);
    }

    const T& Access() const {
        return *reinterpret_cast<const T*>(&this->storage_);
    }

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type storage_;
};

}  // namespace utils
}  // namespace aerozen

#endif  // AEROZEN_UTILS__NEVER_DESTROYED_HPP_
