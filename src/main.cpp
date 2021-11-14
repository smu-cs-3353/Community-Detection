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
struct do_nothing
{
    template <typename VertexOrEdge1, typename VertexOrEdge2>
    void operator()(const VertexOrEdge1& , VertexOrEdge2& ) const
    {
    }
};

//shortcut adjacency_list for the football graph
using footballGraph =  boost::adjacency_list<boost::vecS,
        boost::vecS, boost::undirectedS, footballData>;
//shortcut for vertex_descriptor types
using Vertex = boost::graph_traits<footballGraph>::vertex_descriptor;
using Edge = boost::graph_traits<footballGraph>::edge_descriptor;
footballGraph readMap(std::string path);
std::vector<std::vector<Vertex>> processParentMaps(footballGraph);
void printParentMaps(footballGraph graph, std::vector<std::vector<Vertex>>& shortestPathFromTo);
void newManAlgo(footballGraph graph, std::vector<std::vector<Vertex>>& paths);
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
    std::vector<std::vector<Vertex>> shortestPathFromTo;
    //runs DFS from each node to generate parent maps
    shortestPathFromTo = processParentMaps(graph);
    boost::print_graph(graph);
    newManAlgo(graph, shortestPathFromTo);

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
        //std::cout << "file opened " << std::endl;
        boost::write_graphviz_dp(outFile, g, dp, "label");
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
typedef std::vector<Vertex>::value_type *Piter;
//implemented here: https://www.boost.org/doc/libs/1_77_0/libs/graph/example/bfs.cpp
void printParentMaps(footballGraph graph, std::vector<std::vector<Vertex>>& shortestPathFromTo){
    for(int i = 0; i < shortestPathFromTo.size(); i++){
        std::cout << "=========================================" <<  std::endl << "Printing Predecessor Map For Node "<< i <<  std::endl;
        std::for_each(boost::vertices(graph).first, boost::vertices(graph).second,
                      print_parent<Piter>(&shortestPathFromTo[1][0]));
    }
}
using EdgeIterator = boost::graph_traits<footballGraph>::edge_iterator;

footballGraph calculateBetween(footballGraph graph, std::vector<std::vector<Vertex>>& shortestPathFromTo, footballGraph bestGraph, int total, int counter) {
    if (num_edges(graph) == 0) {
        bestGraph = graph;
        return bestGraph;
    } else {
        footballGraph::edge_iterator edge, edge_end;
        std::pair <EdgeIterator, EdgeIterator> ei = boost::edges(graph);
        std::vector<Edge> maxList;
        Edge max;
        //for each edge
        int betweenValMax = -1;
        for (EdgeIterator edge_iter = ei.first; edge_iter != ei.second; ++edge_iter) {
            int betweenVal = 0;
            Edge toCheck = *edge_iter;
            Vertex go = vertex(0, graph);
            //for each shortest path
            for (int i = 0; i < shortestPathFromTo.size(); i++) {
                if (out_degree(vertex(i, graph), graph) == 0)
                    continue;
                for (int j = 0; j < shortestPathFromTo[i].size(); j++) {
                    if (out_degree(vertex(j, graph), graph) == 0)
                        continue;
                    bool check = true;
                    if (i == j) {
                        continue;
                    } else if (shortestPathFromTo[i][j] == i) {
                        betweenVal++;
                        continue;
                    }

                    Vertex temp = i;
                    Vertex temp2 = shortestPathFromTo[i][j];
                    //checks if edge appears on path, if yes, increment betweenVal counter
                    while (shortestPathFromTo[temp][j] != vertex(j, graph) && temp2 != temp) {
                        Vertex sor1 = toCheck.m_source;
                        Vertex tar1 = toCheck.m_target;
                        Vertex sor2 = temp;
                        Vertex tar2 = temp2;

                        if (sor1 == sor2 && tar1 == tar2) {
                            betweenVal++;
                            break;
                        }
                        temp = temp2;
                        temp2 = shortestPathFromTo[temp2][j];
                    }
                }
            }
//if new max is reached, clear max list, start new list
            if (betweenVal > betweenValMax) {
                maxList.clear();
                betweenValMax = betweenVal;
                maxList.push_back(*edge_iter);
                max = *edge_iter;


            }
            else if (betweenVal == betweenValMax) {
                maxList.push_back(*edge_iter);

            }
        }

        for(int i = 0; i < maxList.size(); i++){
            if(i==1){
                std::cout << "it happened for " <<  maxList.size() << " edges " << std::endl;

            }
            remove_edge(maxList[i], graph);
            std::cout << "max betweenness edge is " << std::endl;
            std::cout << maxList[i] << std::endl;
        }
        footballGraph g_Copy = graph;
        if(num_edges(g_Copy) < (.8)*total && num_edges(g_Copy) != 0){
            std::cout << "reached if" << std::endl;
            counter++;
            std::string inx = std::to_string(counter);
            std::fstream outFile;
            std::string path = "../football/football_starting_example";
            path = path + inx + ".dot";
            outFile.open(path.c_str(), std::fstream::out);
            if(outFile.is_open()) {
                boost::dynamic_properties dp(boost::ignore_other_properties);
                dp.property("label", boost::get(&footballData::label, graph));
                boost::write_graphviz_dp(outFile, graph, dp, "label");
            }else{
                std::cout << "file not open" << std::endl;
            }
            total = (0.5)*total;

        }
        return calculateBetween(g_Copy, shortestPathFromTo, g_Copy, total, counter);

    }
}
void newManAlgo(footballGraph graph, std::vector<std::vector<Vertex>>& paths){
    int total = num_edges(graph);
    int counter = 0;
    graph = calculateBetween(graph, paths, graph, total, counter);
}












