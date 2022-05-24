#ifndef STATES_H
#define STATES_H

#include "figures.h"
#include <stack>

struct state_t {
    explicit state_t(Rectangle rect, std::vector<Line> lines, int handler_state)
        : rect(rect)
        , lines(lines)
        , handler_state(handler_state)
    {
    }

    Rectangle rect;
    std::vector<Line> lines;
    int handler_state;
};

void states_push(std::stack<state_t>& states, const state_t& state);
state_t states_pop(std::stack<state_t>& states);

#endif // STATES_H
