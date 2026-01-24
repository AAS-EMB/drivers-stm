#pragma once
#include <coroutine>
#include <chrono_clocks/steady_clock.hpp>

namespace driver::async {

struct coro_timer {
    std::chrono::nanoseconds dur;
    driver::steady_clock::time_point deadline{};
    std::coroutine_handle<> handle{};
    bool armed{false};

    static coro_timer sleep_for(std::chrono::nanoseconds d) noexcept {
        return coro_timer{d};
    }

    static coro_timer sleep_until(driver::steady_clock::time_point tp) noexcept {
        coro_timer t{};
        t.deadline = tp;
        return t;
    }

    bool await_ready() const noexcept {
        return dur.count() <= 0;
    }

    bool await_suspend(std::coroutine_handle<> h) noexcept {
        handle = h;
        armed = true;

        if (deadline == driver::steady_clock::time_point{}) {
            deadline = driver::steady_clock::now() + dur;
        }
        return true;
    }

    void process() noexcept {
        if (armed && handle && driver::steady_clock::now() >= deadline) {
            auto h = handle;
            handle = {};
            armed = false;
            if (!h.done())
                h.resume();
        }
    }

    void await_resume() const noexcept {}
};

}
