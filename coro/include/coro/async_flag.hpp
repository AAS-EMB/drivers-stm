#pragma once
#include <coroutine>
#include <cstdbool>

namespace driver::async {

struct async_flag {
    bool await_ready() const noexcept {
        return flag;
    }

    void await_suspend(std::coroutine_handle<> h) noexcept {
        waiter = h;
    }

    bool await_resume() const noexcept {
        return flag;
    }

    void operator=(bool const& value) noexcept {
        flag = value;
        if (waiter) {
            auto h = waiter;
            waiter = {};
            h.resume();
        }
    }

private:
    bool flag{false};
    std::coroutine_handle<> waiter{};
};

}