#include <gtest/gtest.h>
#include "gpio_base.hpp"

class fake_gpio : driver::gpio_base<fake_gpio> {
public:
    void set() noexcept {
        state = true;
    }

    void reset() noexcept {
        state = false;
    }

    [[nodiscard]] bool is_set() const noexcept {
        return state;
    }

private:
    bool state = false;
};

class fake_gpio_copy : driver::gpio_base<fake_gpio_copy> {
public:
    void set() noexcept {
        state = true;
    }

    void reset() noexcept {
        state = false;
    }

    [[nodiscard]] bool is_set() const noexcept {
        return state;
    }

private:
    bool state = false;
};

TEST(Gpio, SetReset) {
    fake_gpio fg;

    EXPECT_FALSE(fg.is_set());

    fg.set();

    EXPECT_TRUE(fg.is_set());

    fg.reset();

    EXPECT_FALSE(fg.is_set());
}

TEST(Gpio, CallInLambdaMultiDriver) {
    fake_gpio fg1;
    fake_gpio_copy fg2;

    auto set = [](auto & driver) noexcept {
        driver.set();
    };

    auto reset = [](auto & driver) noexcept {
        driver.reset();
    };

    auto is_set = [](auto & driver) noexcept -> bool {
        return driver.is_set();
    };

    EXPECT_FALSE(is_set(fg1));
    EXPECT_FALSE(is_set(fg2));

    set(fg1);
    set(fg2);

    EXPECT_TRUE(is_set(fg1));
    EXPECT_TRUE(is_set(fg2));

    reset(fg1);
    reset(fg2);

    EXPECT_FALSE(is_set(fg1));
    EXPECT_FALSE(is_set(fg2));
}