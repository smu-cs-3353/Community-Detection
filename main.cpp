#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <fstream>

void loadHierarchy();
//struct to store the node properties of the football graph
struct footballData{
    long node_id;
    long value;
    std::string label;
};
//shortcut adjacency_list for the football graph
using footballGraph =  boost::adjacency_list<boost::vecS,
        boost::vecS, boost::undirectedS, footballData,
        boost::no_property, boost::no_property, boost::listS>;
int main()
{
loadHierarchy();
}


/*
 * Function returns an adjacency list read from a graphml file
 * Currently hard coded for changes
 */
void loadHierarchy(){
    std::ifstream inFile;
    inFile.open("/Users/yeet/Documents/GitHub/21f-pa03-how-bad-can-it-be/football/football.graphml", std::ifstream::in);
    footballGraph g;
    boost::dynamic_properties dp;
    dp.property("node_id", boost::get(&footballData::node_id, g));
    dp.property("value", boost::get(&footballData::value, g));
    dp.property("label", boost::get(&footballData::label, g));
    boost::read_graphml(inFile, g, dp);
    write_graphml(std::cout, g, dp, true);
    inFile.close();


}