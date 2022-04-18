#ifndef STATES_H
#define STATES_H

#include "drawer.h"
#include "polygones.h"
#include "request.h"
#include <QTableWidget>
#include <stack>

struct state_t {
    polygones_t polygones;
    std::vector<std::string> pointsTable;
};

std::vector<std::string> table_create(QTableWidget* pointsTable);
state_t state_create(const polygones_t& polygones, QTableWidget* pointsTable, std::vector<std::string> table = {});
void states_push(std::stack<state_t>& states, const state_t& state);
state_t states_pop(std::stack<state_t>& states);
void state_set(polygones_t& polygones, const state_t& state, drawer_t& drawer, QTableWidget* pointsTable);

#endif // STATES_H
