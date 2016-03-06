#include <cassert>
#include <iostream>

#include "cgem/Graph.h"

using namespace std;
using namespace gem;

typedef gem::Graph<int, int> G;

int main()
{
    G g;

    G::VrtxId v1 = g.add_vertex(1);
    G::VrtxId v2 = g.add_vertex(2);
    g.add_edge(v1, v2, 7);
    G::VrtxId v3 = g.add_vertex(3);
    g.add_edge(v1, v3, 9);
    g.add_edge(v2, v3, 10);
    G::VrtxId v4 = g.add_vertex(4);
    g.add_edge(v2, v4, 15);
    g.add_edge(v3, v4, 11);
    G::VrtxId v5 = g.add_vertex(5);
    g.add_edge(v4, v5, 6);
    G::VrtxId v6 = g.add_vertex(6);
    g.add_edge(v5, v6, 9);
    g.add_edge(v1, v6, 14);
    g.add_edge(v3, v6, 2);

    G::DistanceList dist_list;
    G::NodeList path;

    int dist = g.shortest_path(
        0,
        4,
        dist_list,
        path
    );

    std::cout << "min. distance =" << dist << "\n";

    assert(dist == 20);

    for (auto d : dist_list) {
        std::cout << d << ",";
    }
    std::cout << "\n";

    for (auto node_id : path) {
        std::cout << node_id << ",";
    }
    std::cout << "\n";

    return 0;
}
