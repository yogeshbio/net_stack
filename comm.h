/*
 * comm.h
 *
 *  Created on: Apr 7, 2020
 *  Author: yogesh kulkarni (yogidk@gmail.com)
 */

#pragma once
#include "graph.h"

#define MAX_PKT_BUFFER_SIZE   2048


void udp_init_socket(node_t *node);

void nw_start_pkt_rx_thread(graph_t* topo);
