/*
 * graph.c
 *
 *  Created on: Jan 7, 2020
 *  Author: yk (yogidk@gmail.com)
 */

#include "graph.h"
#include <string.h>
#include "comm.h"

struct list_head *head = NULL;

graph_t* create_new_graph(char* topology_name)
{
    graph_t* g = calloc(1, sizeof(graph_t));
    int size =  strlen(topology_name);

    strncpy(g->topology_name, topology_name, size);
    g->topology_name[size+1] = '\0';

    /* The linked list is intrusive: meaning the list pointers are part of the data itself,
       hence number of allocations are less compared to the traditional linked list
       This is the first element of the list prepared, as head was initialized to NULL.
       Hence Graph will be the HEAD node and other nodes will be attached to it
       using CDL_APPEND calls
     */
    CDL_APPEND(head,&g->node_list);
    return g;

}

/* create a new node and attach it to the graph */
node_t* create_graph_node(graph_t* graph, char* node_name)
{
    node_t* node = calloc(1,sizeof(node_t));
    int size     =  strlen(node_name);
    strncpy(node->node_name, node_name, size);
    node->node_name[size+1] = '\0';   // strlen does not include NULL character
    init_node_nw_property(&(node->node_nw_props));
    udp_init_socket(node);
    CDL_APPEND(head,&node->graph);  /* append all nodes to the graph HEAD node. Note: we have a circular list */

    return node;
}

void insert_link_between_two_nodes(node_t* node1,
        node_t* node2,
        char* from_if_name,
        char* to_if_name,
        unsigned int cost
)
{
    link_t* link = calloc(1, sizeof(link_t));

    /* set interface properties */
    int size;
    size =  strlen(from_if_name);
    strncpy(link->intf1.if_name, from_if_name, size);
    link->intf1.if_name[size+1] = '\0';
    size =  strlen(to_if_name);
    strncpy(link->intf2.if_name, to_if_name, size);
    link->intf2.if_name[size+1] = '\0';

    /* Make both pointers point to the same link to establish a link */
    link->intf1.link = link;
    link->intf2.link = link;

    /* Also add the node owner of the interface */
    link->intf1.owner = node1;
    link->intf2.owner = node2;
    link->cost = cost;

    int slot_for_interface; /* Fill the info of the nodes with appropriate interface slots for that node */

    slot_for_interface = get_slot_for_interface_in_node(node1);
    node1->intf[slot_for_interface] = &link->intf1;

    slot_for_interface = get_slot_for_interface_in_node(node2);
    node2->intf[slot_for_interface] = &link->intf2;

    init_intf_nw_property(&link->intf1.intf_nw_props);   // init interface properties
    init_intf_nw_property(&link->intf2.intf_nw_props);

    assign_intf_mac_addr(&link->intf1);              // init mac addresses
    assign_intf_mac_addr(&link->intf2);
}


/* Display routines to dump the graph info */

void dump_graph(graph_t *topo)
{
    node_t* node;
    printf("Graph topology name: %s\n",topo->topology_name);

    struct list_head *position;
    CDL_FOREACH(position, &topo->node_list)  // here position gets filled with the address of list pointer of the node_t* struct.
    {
        node = list_entry(position, node_t , graph ); // This will give me the address of the actual node i.e. node_t* due to container_of macro

#ifdef DEBUG_INFO
        printf("position addr and node addr  = %p, %p\n",position,node);
#endif

        dump_node(node);
    }

}

void dump_node(node_t* node)
{
    printf("Node name: %s\n",node->node_name);
    interface_t *intf;
    for( int i=0; i < MAX_INTF_PER_NODE; i++){

        intf = node->intf[i];
        if(!intf) break;
        dump_interface(intf);
    }

}

void dump_interface(interface_t* intf)
{
    node_t *neighbor = get_neighbor_node(intf);
    printf("Interface Name:%s, Owner Node:%s, Neighbor Node:%s, Cost:%d\n",
            intf->if_name,
            intf->owner->node_name,
            neighbor->node_name,
            intf->link->cost
    );
}
