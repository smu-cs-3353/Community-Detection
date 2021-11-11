#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <fstream>

void loadHierarchy();
struct graphData{
    std::string name;
};
//struct to store the node properties of the football graph
struct footballData{
    long node_id;
    long value;
    std::string label;
};
struct edgeProperty{
    std::string name;
    int distance = 1;
    int betweenessValue;
};
//shortcut adjacency_list for the football graph
using footballGraph =  boost::adjacency_list<boost::vecS,
        boost::vecS, boost::undirectedS, footballData,
        edgeProperty, graphData, boost::listS>;
using Vertex = boost::graph_traits<footballGraph>::vertex_descriptor;
int main()
{
loadHierarchy();
}


/*
 * Function constructs an adjacency list read from a graphml file
 * Currently hard coded for changes
 */
void loadHierarchy(){
    std::ifstream inFile;
    inFile.open("/Users/yeet/Documents/GitHub/21f-pa03-how-bad-can-it-be/football/football.graphml", std::ifstream::in);
    footballGraph g;
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id", boost::get(&footballData::node_id, g));
    dp.property("value", boost::get(&footballData::value, g));
    dp.property("label", boost::get(&footballData::label, g));
    boost::read_graphml(inFile, g, dp);
    std::vector<Vertex> p(boost::num_vertices(g));
    Vertex s = *(boost::vertices(g).first);
    //std::vector<boost::graph_traits<footballGraph>::vertices_size_type> d;
    p[s] = s;


    boost::breadth_first_search(g, s,boost::visitor(boost::make_bfs_visitor
    (boost::record_predecessors(p, boost::on_tree_edge()))));



    //write_graphml(std::cout, g, dp, true);
    inFile.close();
}

