#ifndef GRAPHCLEAR_TYPEDEFS_H
#define GRAPHCLEAR_TYPEDEFS_H

#include <boost/graph/adjacency_list.hpp>

namespace graphclear
{
    class cut_sequence_t;
    
class surveillance_graph_vertex
{
public:
    surveillance_graph_vertex() : outgoing_completed(0) {};
    int w;
    int outgoing_completed;
};

class surveillance_graph_edge
{
public:
    surveillance_graph_edge() : spanning_tree(false),w(0) {
    };
    cut_sequence_t* cut_sequence_source_to_target;
    cut_sequence_t* cut_sequence_target_to_source;
    int w;
    bool spanning_tree;
};

typedef boost::adjacency_list
    <boost::vecS, boost::vecS, boost::undirectedS, 
     surveillance_graph_vertex, surveillance_graph_edge >
    sg_base;
     
} /* sg_typedefs */
     
#endif