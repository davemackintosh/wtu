#pragma once

#include <expected> // C++23
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <type_traits>

// Concept to ensure T is copyable or moveable, as required for shared ownership
template <typename T>
concept BorrowableType =
    std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>;

template <BorrowableType T> class Ref;

template <BorrowableType T> class MutRef;

template <BorrowableType T> class Borrowable {
  public:
    Borrowable(T &&resource)
        : resource_(std::make_shared<T>(std::move(resource))) {}

    auto borrow() const -> std::expected<Ref<T>, std::string> {
        std::shared_lock lock(mutex_);
        if (mut_ref_active_) {
            return std::unexpected("Cannot create Ref while MutRef is active");
        }
        return Ref<T>(resource_, mutex_);
    }

    auto borrow_mut() -> std::expected<MutRef<T>, std::string> {
        std::unique_lock lock(mutex_);
        if (mut_ref_active_ || ref_count_ > 0) {
            return std::unexpected(
                "Cannot create MutRef while other borrows are active");
        }
        mut_ref_active_ = true;
        return MutRef<T>(resource_, mutex_, mut_ref_active_);
    }

  private:
    mutable std::shared_mutex mutex_;
    mutable bool mut_ref_active_ = false;
    mutable int ref_count_ = 0;
    std::shared_ptr<T> resource_;

    friend class Ref<T>;
    friend class MutRef<T>;
};

template <BorrowableType T> class Ref {
  public:
    Ref(const std::shared_ptr<T> &resource, std::shared_mutex &mutex)
        : resource_(resource), lock_(mutex) {}

    auto operator->() const -> const T * { return resource_.get(); }
    auto operator*() const -> const T & { return *resource_; }

  private:
    std::shared_ptr<T> resource_;
    std::shared_lock<std::shared_mutex> lock_;
};

template <BorrowableType T> class MutRef {
  public:
    MutRef(const MutRef &) = delete;
    MutRef(MutRef &&) = delete;
    auto operator=(const MutRef &) -> MutRef & = delete;
    auto operator=(MutRef &&) -> MutRef & = delete;
    MutRef(const std::shared_ptr<T> &resource, std::shared_mutex &mutex,
           bool &mut_ref_flag)
        : resource_(resource), lock_(mutex), mut_ref_flag_(mut_ref_flag) {}

    ~MutRef() { mut_ref_flag_ = false; }

    auto operator->() -> T * { return resource_.get(); }
    auto operator*() -> T & { return *resource_; }

  private:
    std::shared_ptr<T> resource_;
    std::unique_lock<std::shared_mutex> lock_;
    bool &mut_ref_flag_;
};
