#include "states.h"

void states_push(std::stack<state_t>& states, const state_t& state)
{
    states.push(state);
}

state_t states_pop(std::stack<state_t>& states)
{
    state_t state {};
    if (states.empty())
        return state;
    state = states.top();
    states.pop();
    return state;
}

void state_set(polygones_t& polygones, const state_t& state, drawer_t& drawer, QTableWidget* pointsTable)
{
    drawer.clear();
    pointsTable->clear();
    QTableWidgetItem* x = new QTableWidgetItem;
    QTableWidgetItem* y = new QTableWidgetItem;
    x->setText("X");
    y->setText("Y");
    pointsTable->setHorizontalHeaderItem(0, x);
    pointsTable->setHorizontalHeaderItem(1, y);
    polygones_t t = state.polygones;
    polygones = t;
    for (size_t i = 0; i < state.pointsTable.size(); i++) {
        if ((int)i == pointsTable->rowCount())
            pointsTable->insertRow(pointsTable->rowCount());
        QTableWidgetItem* x = new QTableWidgetItem;
        QTableWidgetItem* y = new QTableWidgetItem;
        std::string t = state.pointsTable[i];
        if (t == " , ")
            continue;
        size_t index = t.find(',');
        x->setText(QString(t.substr(0, index).c_str()));
        y->setText(QString(t.substr(index + 1).c_str()));
        pointsTable->setItem(i, 0, x);
        pointsTable->setItem(i, 1, y);
    }
    drawer.redraw(polygones);
}

std::vector<std::string> table_create(QTableWidget* pointsTable)
{
    std::vector<std::string> table;
    for (int i = 0; i < pointsTable->rowCount(); i++) {
        std::string t;
        if (!pointsTable->item(i, 0) || !pointsTable->item(i, 1))
            t = " , ";
        else {
            QString x = pointsTable->item(i, 0)->text();
            QString y = pointsTable->item(i, 1)->text();
            t = QString("%1,%2").arg(x, y).toStdString();
        }
        table.push_back(t);
    }
    return table;
}

state_t state_create(const polygones_t& polygones, QTableWidget* pointsTable, std::vector<std::string> table)
{
    state_t state;
    state.polygones = polygones;
    if (table.empty())
        table = table_create(pointsTable);
    state.pointsTable = table;
    return state;
}
