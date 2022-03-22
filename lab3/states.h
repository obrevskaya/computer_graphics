#ifndef STATES_H
#define STATES_H

#include "drawing.h"
#include "tasks.h"
#include "structures.h"
#include <stack>

struct data_t {
    bool is_line;
    union {
        line_t line;
        spectrum_t spectrum;
    };
    int method;
    color_t color;
};

void states_push(std::stack<std::vector<data_t>>& states, const std::vector<data_t> state);
std::vector<data_t> states_pop(std::stack<std::vector<data_t>>& states);
void dataset_set(const std::vector<data_t>& state, drawer_t& drawer);
void data_set(const data_t& data, drawer_t& drawer);

#endif // STATES_H
