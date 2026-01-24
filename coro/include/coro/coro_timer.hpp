#pragma once
#include <coroutine>
#include <chrono_clocks/steady_clock.hpp>

namespace driver::async {

struct coro_timer {
    std::chrono::nanoseconds dur;
    driver::steady_clock::time_point deadline{};
    std::coroutine_handle<> handle{};

    bool await_ready() const noexcept {
        return dur.count() <= 0;
    }

    bool await_suspend(std::coroutine_handle<> h) noexcept {
        handle = h;
        deadline = driver::steady_clock::now() + dur;
        return true;
    }

    void process() noexcept {
        if (handle && driver::steady_clock::now() >= deadline) {
            auto h = handle;
            handle = {};
            h.resume();
        }
    }

    void await_resume() const noexcept {}
};

}
