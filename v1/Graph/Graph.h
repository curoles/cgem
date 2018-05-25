#pragma once
#ifndef CGEM_GRAPH_H_INCLUDED
#define CGEM_GRAPH_H_INCLUDED

#include <vector>
#include <limits>

namespace gem {

template<class Node, class EdgeValue>
class Graph
{
public:

    typedef int VrtxId;
    typedef int EdgeId;
 
    //static const VrtxId UNDEFINED_NODE = -1;

    //static const EdgeValue INFINITY = std::numeric_limits<EdgeValue>::max();

    typedef std::vector<VrtxId> NodeList;

    typedef std::vector<EdgeValue> DistanceList;

    struct Edge
    {
        VrtxId m_src;
        VrtxId m_dst;
        EdgeValue m_value;
    };

    struct Vertex
    {
        Node m_node;
        std::vector<EdgeId> m_links;
        Vertex(const Node& node):m_node(node){}
    };

private: 
    std::vector<Vertex> m_verteces;
    std::vector<Edge> m_edges;

public:
    VrtxId push_back(const Node& node)
    {
        return add_vertex(node);
    }

    VrtxId add_vertex(const Node& node)
    {
        VrtxId vrtx_id = m_verteces.size();
        m_verteces.push_back(Vertex(node));

        return vrtx_id;
    }

    EdgeId add_edge(VrtxId src, VrtxId dst, const EdgeValue& edge_val)
    {
        // Assert vertices src and dst exist

        EdgeId edge_id = m_edges.size();
        m_edges.push_back(Edge{src, dst, edge_val});

        m_verteces[src].m_links.push_back(edge_id);
        m_verteces[dst].m_links.push_back(edge_id);

        return edge_id; 
    }

#include "dijkstra.h"
};

} //namespace gem

#endif
