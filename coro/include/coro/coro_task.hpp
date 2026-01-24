#pragma once
#include <coroutine>
#include <memory_resource>

namespace driver::async {

struct coro_task {
    static inline std::pmr::memory_resource*& get_resource() noexcept {
        static std::pmr::memory_resource* res = std::pmr::new_delete_resource();
        return res;
    }

    static inline void set_resource(std::pmr::memory_resource& r) noexcept {
        get_resource() = &r;
    }

    struct promise_type {
        coro_task get_return_object() {
            return coro_task{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() noexcept {}
        void unhandled_exception() { std::terminate(); }

        static void* operator new(std::size_t size) {
            return coro_task::get_resource()->allocate(size, alignof(promise_type));
        }

        static void operator delete(void* ptr, std::size_t size) noexcept {
            #if defined(PLATFORM_HOST)
                std::pmr::get_default_resource()->deallocate(ptr, size);
            #else
                (void)ptr; (void)size;
                std::terminate();
            #endif
        }
    };

    std::coroutine_handle<promise_type> h;
};

}