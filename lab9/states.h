#ifndef STATES_H
#define STATES_H

#include "figures.h"
#include <stack>

struct state_t {
    explicit state_t(const Polygon& cutter, const Polygon& enter, const int& handler_state)
        : cutter(cutter)
        , enter(enter)
        , handler_state(handler_state)
    {
    }

    Polygon cutter;
    Polygon enter;
    int handler_state;
};

void states_push(std::stack<state_t>& states, const state_t& state);
state_t states_pop(std::stack<state_t>& states);

#endif // STATES_H
