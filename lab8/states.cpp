#include "states.h"

void states_push(std::stack<state_t>& states, const state_t& state)
{
    states.push(state);
}

state_t states_pop(std::stack<state_t>& states)
{
    state_t state { Cutter(QColor(0, 0, 0)), {}, 0 };
    if (states.empty())
        return state;
    state = states.top();
    states.pop();
    return state;
}
