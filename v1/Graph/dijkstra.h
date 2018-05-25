// See Dijkstra's algorithm on wiki

EdgeValue shortest_path(
    const VrtxId src,
    const VrtxId dest,
    DistanceList& dist,     // distance from 'src'
    NodeList& path
    )
{
    static const VrtxId UNDEFINED_NODE = -1;

    static const EdgeValue INFINITY = std::numeric_limits<EdgeValue>::max();

    EdgeValue shortest_path_dist{INFINITY};

    VrtxId nr_nodes = m_verteces.size();

    std::vector<VrtxId> previous;
    std::vector<bool> visited;

    for (VrtxId vid = 0; vid < nr_nodes; ++vid)
    {
        dist.push_back(INFINITY);
        previous.push_back(UNDEFINED_NODE);
        visited.push_back(false);
    }

    dist[src] = 0; // distance is 0 from a node to itself

    VrtxId u{src};

    for (VrtxId uc = 0; uc < nr_nodes; ++uc)
    {
        // find closest to source node
        u = UNDEFINED_NODE;
        for (VrtxId i = 0; i < nr_nodes; ++i) {
            if (not visited[i]) {
              if (u == UNDEFINED_NODE) {u = i;}
              if (dist[i] < dist[u]) {u = i;}
            }
        }

        // exit because all remaining vertices are inaccessible from 'src'
        if (u == UNDEFINED_NODE or dist[u] == INFINITY) {
            break;
        }

        // exit if reached target
        if (u == dest) {
            shortest_path_dist = dist[u];
            break;
        }

        visited[u] = true;

        // for each neighbor v of u
        for (auto edge_id : m_verteces[u].m_links)
        {
            const Edge& edge = m_edges[edge_id];
            VrtxId v = (u == edge.m_src)? edge.m_dst : edge.m_src;

            if (visited[v] == true) continue;

            EdgeValue alt = dist[u] + edge.m_value;

            if (alt < dist[v])
            {
                dist[v] = alt;
                previous[v] = u;
            }
        }
    }

    if (u == dest and dest != UNDEFINED_NODE) {
        while (previous[u] != UNDEFINED_NODE) {
            path.push_back(u);
            u = previous[u];
        }
    }

    return shortest_path_dist;
}
