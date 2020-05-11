/*
 *  Created on: Jan 17, 2020
 *  Author: yogesh kulkarni

 * Represents network graph
 */

#pragma once


#include "intrusive_list/intrusive_list.h"
#include "string.h"

typedef struct node_ node_t;
typedef struct link_ link_t;
typedef struct graph_ graph_t;
typedef struct interface_ interface_t;

#include "net.h"

#define NODE_NAME_SIZE   16
#define IF_NAME_SIZE     16
#define MAX_INTF_PER_NODE   10


/*
 * Each node has interfaces and these interfaces are linked with each other.
 * There can be multiple interfaces per node.
 */

struct interface_
{
    char if_name[IF_NAME_SIZE];
    struct node_ *owner; //owner of the interface
    /*
       the interfaces  must link with each other, the easiest way is to use a pointer.
       Two interfaces can be linked if pointers of both interfaces point to each other
     */
    struct link_ *link;
    intf_nw_props_t intf_nw_props;
};


struct link_
{
    interface_t intf1;
    interface_t intf2;
    unsigned int cost;
};


/*
 * Each node contains interfaces (max=10) and between interfaces there
 * is a link.
 * Node embeds the graph list structure. Graph acts as HEAD node. Node will be attached
 * to the this HEAD graph.
 */
struct node_
{
    char node_name[NODE_NAME_SIZE];
    interface_t *intf[MAX_INTF_PER_NODE]; /* pointer to each interface. NULL means the interface is empty.*/
    struct list_head graph;   /* intrusive list of graph */
    node_nw_props_t node_nw_props;
    int udp_port_no;
    int udp_socket_fd;
};

/* The linked list is intrusive: meaning the list pointers are part of the data itself,
 * Advantages of intrusive list: number of allocations are less compared to the
 * traditional linked list because you need to allocate memory to the embedded struct
 * only once where as in traditional lists you need to allocate it
 * for the data once and to the struct list once.
 * Less dereferencing: In traditional lists you need to dereference once for getting the
 * list address and once again to get data. In intrusive lists it is only once.
 * Graph will be the HEAD node and other nodes will be attached to it using CDL_APPEND calls
 */


struct graph_
{
    char topology_name[32];
    struct list_head node_list; /* intrusive list of nodes */
};



/*  helper functions */

/*
 * simply gives an available empty slot to an interface of the node
 */
static inline int
get_slot_for_interface_in_node(node_t *node)
{
    int i ;

    for( i = 0 ; i < MAX_INTF_PER_NODE; i++)
    {
        if(node->intf[i])
            continue;
        return i;
    }
    return -1;
}


/*
 *  get the neighbor node for the given interface
 */
static inline node_t*
get_neighbor_node(interface_t* interface)
{

    assert(interface->owner);
    assert(interface->link);

    link_t *link = interface->link;

    if(&(link->intf1) == interface)  // given interface matches with intf1, hence neighbor must be intf2
    {
        return link->intf2.owner;
    }
    return link->intf1.owner;
}

/*
 * returns pointer to the local interface of a node, searched by if_name
 */
static inline interface_t*
get_intf_by_name_from_node(node_t *node,
                           char* if_name)
{
    assert(node);
    for(int i=0; i<MAX_INTF_PER_NODE;i++)
    {
        if(strncmp(if_name,node->intf[i]->if_name,IF_NAME_SIZE) != 0)
            continue;

        return (node->intf[i]); // return pointer to the local interface
    }
    return NULL;
}

/*
 *  returns pointer node present in a graph list, searched by node name
 */
static inline node_t*
get_node_from_node_name(graph_t* topo,
                        char* node_name)
{
    assert(topo);
    node_t* node;
    struct list_head *position;

    CDL_FOREACH(position,&topo->node_list) // here position gets filled with the address of list pointer of the node_t* struct.
    {
        node = list_entry(position, node_t, graph ); // This will give me the address of the actual node i.e. node_t* due to container_of macro
        if(strcmp(node_name, node->node_name) != 0)
            continue;
        return (node); // return pointer to node_t*
    }
}

/* Public API's */
graph_t*
create_new_graph(char* topology_name);

node_t*
create_graph_node(graph_t* graph,
        char* node_name);

void
insert_link_between_two_nodes(node_t* node1,
        node_t* node2,
        char* from_if_name,
        char* to_if_name,
        unsigned int cost);

void
dump_graph(graph_t* graph);

void
dump_node(node_t* node);

void
dump_interface(interface_t* intf);
