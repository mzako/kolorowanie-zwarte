#ifndef EDGE_H
#define EDGE_H

struct Edge {
    int v1;
    int v2;
    int color;

    Edge(int v1, int v2, int color = 0) : v1(v1), v2(v2), color(color) {}
};
#endif //EDGE_H
