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

//struct to store the node properties of the football graph
struct footballData{
    long node_id;
    long value;
    std::string label;
};
//shortcut adjacency_list for the football graph
using footballGraph =  boost::adjacency_list<boost::vecS,
        boost::vecS, boost::undirectedS, footballData>;
//shortcut for vertex_descriptor types
using Vertex = boost::graph_traits<footballGraph>::vertex_descriptor;
using Edge = boost::graph_traits<footballGraph>::edge_descriptor;
template < typename Graph, typename IncomingMap, typename DistanceMap, typename PathCountMap >
struct visitor_type : public boost::bfs_visitor<> {


    visitor_type(IncomingMap incoming, DistanceMap distance,
                 PathCountMap path_count,
                 std::stack< Vertex >& ordered_vertices)
            : incoming(incoming), distance(distance), path_count(path_count), ordered_vertices(ordered_vertices)
            {}
            void examine_vertex(Vertex v, Graph&){
        ordered_vertices.push(v);
    }
    void tree_edge(Edge e, Graph& g){
        Vertex v = source(e, g);
        Vertex w = target(e, g);
        put(distance, w, get(distance, v) + 1);

        put(path_count, w, get(path_count, v));
        incoming[w].push_back(e);
    }
    void non_tree_edge(Edge e, Graph& g)
    {
        Vertex v = source(e, g);
        Vertex w = target(e, g);
        if (get(distance, w) == get(distance, v) + 1)
        {
            put(path_count, w,
                get(path_count, w) + get(path_count, v));
            incoming[w].push_back(e);
        }
    }
private:
    IncomingMap incoming;
    DistanceMap distance;
    PathCountMap path_count;
    std::stack< Vertex > ordered_vertices;
    };

footballGraph readMap(std::string path);
std::vector<std::vector<Vertex>> processParentMaps(footballGraph);
void printParentMaps(footballGraph graph, std::vector<std::vector<Vertex>> shortestPathToFrom);
//struct from boost examples used to print out the parent
//implemented here: https://www.boost.org/doc/libs/1_77_0/libs/graph/example/bfs.cpp
template < class ParentDecorator > struct print_parent
{
    print_parent(const ParentDecorator& p_) : p(p_) {}
    template < class Vertex > void operator()(const Vertex& v) const
    {
        std::cout << "parent[" << v << "] = " << p[v] << std::endl;
    }
    ParentDecorator p;
};

//Pass in path to graphml file in the program arguments
int main(int argc, char* argv[])
{   //creates a graph object of the graphml file provided
    footballGraph graph = readMap(argv[1]);
    //initializes a vector to store parent maps
    std::vector<std::vector<Vertex>> shortestPathToFrom;
    //runs DFS from each node to generate parent maps
    shortestPathToFrom = processParentMaps(graph);
    //printParentMaps(graph, shortestPathToFrom);

}
/**
 *   Function takes in a path to a graphml and returns a graph object
 */
footballGraph readMap(std::string path){
    std::ifstream inFile;
    inFile.open(path.c_str(), std::ifstream::in);
    footballGraph g;
    boost::dynamic_properties dp(boost::ignore_other_properties);
    dp.property("node_id", boost::get(&footballData::node_id, g));
    dp.property("value", boost::get(&footballData::value, g));
    dp.property("label", boost::get(&footballData::label, g));
    boost::read_graphml(inFile, g, dp);
    std::fstream outFile;
    std::string path2 = "../football/football_starting_example.dot";
    outFile.open(path2.c_str(), std::fstream::out);
    if(outFile.is_open()) {
        std::cout << "file opened " << std::endl;
        boost::write_graphviz_dp(outFile, g, dp, "node_id");
    }

    return g;
}


/**
 * Function takes in a graph object
 * Function returns a vector of parent maps for each node in the given graph
 */
std::vector<std::vector<Vertex>> processParentMaps(footballGraph g){
    //create vector to return
    std::vector<std::vector<Vertex>> parentLists;
    //create idMap
    auto vertex_idMap = get(boost::vertex_index, g);
    //create itrs
    boost::graph_traits <footballGraph>::vertex_iterator i, end;
    //for each vertex, call breadth first search with record_predecessors visitor
    //push back each predecessor/parent map to parentLists
    for(boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
        std::vector<Vertex> p(boost::num_vertices(g));
        Vertex s = vertex_idMap[*i];
        p[s] = s;
        //call to breadth first search
        boost::breadth_first_search(g, s,boost::visitor
        //creates a bfs visitor to record predecessor
        //TO ADD: PATHCOUNTER
        (boost::make_bfs_visitor
        (boost::record_predecessors
        (&p[0],boost::on_tree_edge())
          )));
        parentLists.push_back(p);
    }
    //printParentMaps(g, parentLists);
    //print out read in graph
    //boost::print_graph(g);
    return parentLists;
}
//takes in a graph and it's vector of predecessor maps and prints them out
//implemented here: https://www.boost.org/doc/libs/1_77_0/libs/graph/example/bfs.cpp
void printParentMaps(footballGraph graph, std::vector<std::vector<Vertex>> shortestPathToFrom){
    for(int i = 0; i < shortestPathToFrom.size(); i++){
        typedef std::vector<Vertex>::value_type *Piter;
        std::cout << "=========================================" <<  std::endl << "Printing Predecessor Map For Node " << i << std::endl;
        std::for_each(boost::vertices(graph).first, boost::vertices(graph).second,
                      print_parent<Piter>(&shortestPathToFrom[i][0]));
    }
}

//        void construct_visitor(){
//            std::vector<footballGraph::vertices_size_type> test;
//            boost::iterator_property_map<
//                    std::vector<std::vector<Edge>>*
//                    ,
//                    boost::LvaluePropertyMapConcept<
//                            footballData
//                            ,
//                            std::vector<Edge>
//                    >
//            > incomingMap;
//            boost::iterator_property_map<std::vector<footballGraph::vertices_size_type>*, Vertex> distanceMap;
//            boost::iterator_property_map<std::vector<footballGraph::degree_size_type>*, Vertex> pathCountMap;
//
//
////            boost::make_iterator_property_map(test.begin(), Vertex)
//            visitor_type
//                    <footballGraph, incomingMap, distanceMap, pathCountMap:
//                    visit;
//}












