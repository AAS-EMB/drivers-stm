#include <gtest/gtest.h>
#include <gpio_base.hpp>

class fake_gpio final : public driver::IGpio {
public:
    ~fake_gpio() noexcept override = default;

    void set() noexcept override {
        state = true;
    }

    void reset() noexcept override {
        state = false;
    }

    void toggle() noexcept override {
        is_set() ? reset() : set();
    }

    bool is_set() const noexcept override {
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

TEST(Gpio, Toggle) {
    fake_gpio fg;

    EXPECT_FALSE(fg.is_set());

    fg.toggle();

    EXPECT_TRUE(fg.is_set());

    fg.toggle();

    EXPECT_FALSE(fg.is_set());
}