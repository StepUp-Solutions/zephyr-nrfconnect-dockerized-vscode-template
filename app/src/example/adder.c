/*
 * adder.c
 *
 *  Created on: Jul 26, 2024
 *      Author: overriden-sfdd
 */

#include "adder.h"
#include <zephyr/sys/printk.h>

int
add(const int lhs, const int rhs)
{
    const int res = lhs + rhs;
    printk("Addition result: %d", res);
    return res;
}
