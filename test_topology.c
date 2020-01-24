/*
 * test_graph.c
 *
 *  Created on: Jan 8, 2020
 *  Author: yogesh kulkarni (yogidk@gmail.com)
 */

#include "graph.h"


extern graph_t* build_first_topology();

int main(int argc, char **argv)
{
    graph_t* topo = build_first_topology();

    dump_network_graph(topo);  // dump the network graph in terms of links, interfaces etc.
    return 0;
}


