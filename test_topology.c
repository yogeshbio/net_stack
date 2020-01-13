/*
 * test_graph.c
 *
 *  Created on: Jan 8, 2020
 *  Author: yk (yogidk@gmail.com)
 */

#include "graph.h"

extern graph_t* build_first_topology();

int main(int argc, char **argv)
{
 graph_t* topo = build_first_topology();

 dump_graph(topo);
  return 0;
}


