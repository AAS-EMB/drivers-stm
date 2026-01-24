#pragma once
#include <coroutine>
#include <cstdbool>

namespace driver::async {

struct async_flag {
    async_flag() = default;
    async_flag(const async_flag&) = delete;
    async_flag& operator=(const async_flag&) = delete;

    struct awaiter {
        async_flag & f;

        bool await_ready() const noexcept {
            return f.flag;
        }

        void await_suspend(std::coroutine_handle<> h) noexcept {
            assert(!f.handle && "Only one waiter supported");
            f.handle = h;
            f.armed  = true;
        }

        void await_resume() const noexcept {}
    };

    auto operator co_await() noexcept {
        return awaiter{*this};
    }

    async_flag& operator=(bool const& value) noexcept {
        value ? set() : reset();
        return *this;
    }

    bool is_set() const noexcept {
        return flag;
    }

private:
    bool flag{false};
    bool armed{false};
    std::coroutine_handle<> handle{};

    void set() noexcept {
        if (flag) return;

        flag = true;

        if (armed && handle) {
            auto h = handle;
            handle = {};
            if (!h.done()) {
                h.resume();
            }
        }
    }

    void reset() noexcept {
        flag = false;
        handle = {};
    }
};

}