#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;

int main( int argc , char** argv )
{
    typedef adjacency_list <    vecS,
                                vecS,
                                undirectedS,
                                no_property,
                                property < edge_weight_t, int > >
                                graph_t;

    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;


    if( argc != 3 )
    {
        std::cerr << "Usage: ./routing_table <input-file> <source-node>\n";
        return EXIT_FAILURE;
    }

    std::ifstream infile;
    infile.open( argv[1] );

    if( !infile )
    {
        std::cerr << "Error: could not open file.\n";
        return EXIT_FAILURE;
    }

    std::string line = "";
    int vertex1;
    int vertex2;
    int weight;
    int vertex_count = 0;

    std::vector< std::pair<int , int> > edge_array;
    std::vector< int > weights;

    std::getline( infile , line );
    std::stringstream count_stream( line );
    count_stream >> vertex_count;

    while( std::getline( infile , line ) )
    {
        std::stringstream line_stream( line );
        line_stream >> vertex1;
        line_stream >> vertex2;
        line_stream >> weight;

        edge_array.push_back( std::pair<int , int >( vertex1 , vertex2 ) );
        weights.push_back( weight );
    }


    graph_t graph( edge_array.begin() , edge_array.end() , weights.begin() , vertex_count );


    property_map<graph_t, edge_weight_t>::type weightmap = get( edge_weight, graph );


    std::vector<vertex_descriptor> parents(num_vertices( graph ));

    std::vector<int> distances(num_vertices( graph ));


    std::stringstream arg_stream( argv[2] );
    int source_node;
    arg_stream >> source_node;
    vertex_descriptor source = vertex( source_node , graph );


    dijkstra_shortest_paths(    graph ,
                                source ,
                                predecessor_map( boost::make_iterator_property_map( parents.begin() , get( boost::vertex_index , graph ))).
                                distance_map( boost::make_iterator_property_map( distances.begin() , get( boost::vertex_index, graph ))));

    std::cout << "Source Node: " << source_node << "\n";

    std::cout << "---------------------Shortest Distances-------------------------------\n\n";
    printf( "%10s%20s\n" , "Node" , "Distance" );

    graph_traits < graph_t >::vertex_iterator vi, vend;

    for( boost::tie( vi , vend ) = vertices( graph ); vi != vend; ++vi)
    {
        printf( "%10i%20i\n" , *vi , distances[*vi] );
    }

    std::cout << "\n\n---------------------Forwarding Table-------------------------------\n\n";
    printf( "%15s%20s\n" , "Destination" , "Link" );

    for( boost::tie( vi , vend ) = vertices( graph ); vi != vend; ++vi)
    {
        printf( "%15i%20i\n" , *vi , distances[*vi] );
    }

    std::cout << "\n";

    return EXIT_SUCCESS;
}
