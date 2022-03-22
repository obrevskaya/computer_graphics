#include "states.h"

void states_push(std::stack<std::vector<data_t>>& states, const std::vector<data_t> state)
{
    states.push(state);
}

std::vector<data_t> states_pop(std::stack<std::vector<data_t>>& states)
{
    std::vector<data_t> state {};
    if (states.empty())
        return state;
    state = states.top();
    states.pop();
    return state;
}

void dataset_set(const std::vector<data_t>& state, drawer_t& drawer)
{
    drawer.clear();
    for (size_t i = 0; i < state.size(); i++)
        data_set(state[i], drawer);
}

void data_set(const data_t& data, drawer_t& drawer)
{
    params_t params {};
    if (data.is_line)
        params = params_t { .command = DRAW_LINE, .method = data.method, .line = data.line };
    else
        params = params_t { .command = DRAW_SPECTRUM, .method = data.method, .spectrum = data.spectrum };
    request(drawer, params)
        .handle(data.color);
}
