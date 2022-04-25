#include "polygones.h"

polygones_t polygones_init()
{
    static polygones_t polygones {};
    return polygones;
}

bool points_is_equal(const point_t& p1, const point_t& p2)
{
    return (p1.x == p2.x && p1.y == p2.y);
}

void polygone_add_point(polygone_t& polygone, const point_t& point, bool is_new_point)
{
    edge_t edge;
    if (polygone.edges.empty() || is_new_point) {
        edge = { point, point, point.color };
        polygone.edges.push_back(edge);
        return;
    }
    if (polygone.edges.back().p1.x == polygone.edges.back().p2.x && polygone.edges.back().p1.y == polygone.edges.back().p2.y) {
        polygone.edges.back().p2 = point;
        return;
    }
    edge = { polygone.edges.back().p2, point, point.color };
    polygone.edges.push_back(edge);
}

void polygone_sizes(point_t& min, point_t& max, const polygone_t& polygone)
{
    std::vector<edge_t> edges = polygone.edges;
    min = edges[0].p1;
    max = edges[0].p1;
    for (size_t i = 0; i < edges.size(); i++) {
        if (edges[i].p1.x < min.x)
            min.x = edges[i].p1.x;
        if (edges[i].p1.x > max.x)
            max.x = edges[i].p1.x;

        if (edges[i].p1.y < min.y)
            min.y = edges[i].p1.y;
        if (edges[i].p1.y > max.y)
            max.y = edges[i].p1.y;

        if (edges[i].p2.x < min.x)
            min.x = edges[i].p2.x;
        if (edges[i].p2.x > max.x)
            max.x = edges[i].p2.x;

        if (edges[i].p2.y < min.y)
            min.y = edges[i].p2.y;
        if (edges[i].p2.y > max.y)
            max.y = edges[i].p2.y;
    }
}

static size_t find_end(std::vector<edge_t>& edges, std::vector<edge_t>::iterator begin)
{
    while (begin != edges.end() - 1) {
        if (!points_is_equal(begin.base()->p2, (begin + 1).base()->p1))
            return begin - edges.begin();
        ++begin;
    }
    return begin - edges.begin();
}

void polygone_remove_point(polygone_t& polygone, const size_t index)
{
    if (polygone.edges.size() <= index)
        return;
    size_t end = find_end(polygone.edges, polygone.edges.begin() + index);
    if (index == end && points_is_equal(polygone.edges[index].p1, polygone.edges[index].p2)) {
        polygone.edges.erase(polygone.edges.begin() + index);
        return;
    }
    if (index == 0 || !points_is_equal(polygone.edges[index - 1].p2, polygone.edges[index].p1)) {
        if (points_is_equal(polygone.edges[end].p2, polygone.edges[index].p1))
            polygone.edges[end].p2 = polygone.edges[index + 1].p1;
        polygone.edges[index] = polygone.edges[index + 1];
        polygone.edges.erase(polygone.edges.begin() + index + 1);
        return;
    }
    if (index == polygone.edges.size() - 1 && !polygone.closed) {
        polygone.edges.erase(polygone.edges.begin() + index);
    }

    polygone.edges[index - 1] = { polygone.edges[index - 1].p1, polygone.edges[index].p2, polygone.edges[index - 1].color };
    polygone.edges.erase(polygone.edges.begin() + index);
}

void polygone_change_point(polygone_t& polygone, const size_t index, point_t point)
{
    if (polygone.edges.size() <= index)
        return;
    if (polygone.edges.size() == 1 && points_is_equal(polygone.edges[0].p1, polygone.edges[0].p2)) {
        polygone.edges[0] = { point, point, point.color };
        return;
    }
    if (index == 0) {
        if (points_is_equal(polygone.edges[polygone.edges.size() - 1].p2, polygone.edges[0].p1))
            polygone.edges[polygone.edges.size() - 1].p2 = point;
        polygone.edges[0] = { point, polygone.edges[0].p2, point.color };
        return;
    }
    polygone.edges[index - 1] = { polygone.edges[index - 1].p1, point, polygone.edges[index - 1].color };
    polygone.edges[index].p1 = point;
}

bool polygones_is_all_closed(const polygones_t& polygones)
{
    for (size_t i = 0; i < polygones.size(); i++)
        if (!polygones[i].closed && polygones[i].edges.size() != 0)
            return false;
    return true;
}

bool polygone_are_equal(const polygone_t& p1, const polygone_t& p2)
{
    if (p1.edges.size() != p2.edges.size())
        return false;
    if (p1.closed != p2.closed)
        return false;
    if (p1.filled != p2.filled)
        return false;
    if (p1.index_close != p2.index_close)
        return false;
    for (size_t i = 0; i < p1.edges.size(); i++)
        if (!points_is_equal(p1.edges[i].p1, p2.edges[i].p1) || !points_is_equal(p1.edges[i].p2, p2.edges[i].p2))
            return false;
    return true;
}

bool polygones_are_equal(const polygones_t& p1, const polygones_t& p2)
{
    if (p1.size() != p2.size())
        return false;
    for (size_t i = 0; i < p1.size(); i++)
        if (!polygone_are_equal(p1[i], p2[i]))
            return false;
    return true;
}

void polygones_unfill(polygones_t& polygones)
{
    for (size_t i = 0; i < polygones.size(); i++)
        polygones[i].filled = false;
}
