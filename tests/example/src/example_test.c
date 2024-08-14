#include "example/adder.h"

#include <zephyr/ztest.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/sys_heap.h>

struct my_suite_fixture
{
    size_t  max_size;
    size_t  size;
    uint8_t buff[1];
};

static void*
my_suite_setup(void)
{
    printk("setup");
    struct my_suite_fixture* fixture = k_malloc(sizeof(struct my_suite_fixture) + 255);

    zassume_not_null(fixture, NULL);
    fixture->max_size = 256;

    return fixture;
}

static void
my_suite_before(void* f)
{
    struct my_suite_fixture* fixture = (struct my_suite_fixture*)f;
    memset(fixture->buff, 0, fixture->max_size);
    fixture->size = 0;
    printk("before");
}

static void
my_suite_teardown(void* f)
{
    k_free(f);
    printk("teardown");
}

ZTEST_SUITE(my_suite, NULL, my_suite_setup, my_suite_before, NULL, my_suite_teardown);

ZTEST_F(my_suite, test_feature_x)
{
    zassert_equal(0, fixture->size);
    zassert_equal(256, fixture->max_size);
}

ZTEST_F(my_suite, test_feature_example)
{
    zassert_equal(0, add(2, -2));
}
