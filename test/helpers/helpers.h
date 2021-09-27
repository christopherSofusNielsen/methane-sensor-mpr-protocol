#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool test_array_range(uint8_t expect[], uint16_t expect_from, uint8_t actual[], uint16_t actual_from, uint16_t num_elements);
void load_array(uint8_t arr[], uint16_t begin, uint16_t length, uint8_t value);

#endif