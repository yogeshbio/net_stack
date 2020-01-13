/* Represents graph network */

#pragma once

#include "intrusive_list/intrusive_list.h"

#define NODE_NAME_SIZE   16
#define IF_NAME_SIZE     16
#define MAX_INTF_PER_NODE   10


typedef struct node_ node_t;
typedef struct link_ link_t;

typedef struct {
    char if_name[IF_NAME_SIZE];
    struct node_ *owner; //owner of the interface
    /* the interfaces  must link with each other, the easiest way is to use a pointer. Two interfaces can be linked if
       pointers of both interfaces point to each other
    */
    struct link_ *link;
}interface_t;

struct link_ {
    interface_t intf1;
    interface_t intf2;
    unsigned int cost;
};

typedef struct {
    char topology_name[32];
    struct list_head node_list; /* intrusive list of nodes */
}graph_t;


struct node_ {
    char node_name[NODE_NAME_SIZE];
    interface_t *intf[MAX_INTF_PER_NODE]; /* pointer to each interface. NULL means the interface is empty.*/
    struct list_head graph;   /* intrusive list of graph */
};



/*  helper functions */

/* simply gives an available empty slot to an interface of the node */
static inline int
get_slot_for_interface_in_node(node_t *node){

    int i ;

    for( i = 0 ; i < MAX_INTF_PER_NODE; i++){
        if(node->intf[i])
            continue;
        return i;
    }
    return -1;
}


/* get the neighbor node for the given interface */
static inline node_t*
get_neighbor_node(interface_t* interface){

    assert(interface->owner);
    assert(interface->link);

    link_t *link = interface->link;

    if(&(link->intf1) == interface)  // given interface matches with intf1, hence neighbor must be intf2
    {
        return link->intf2.owner;
    }
    return link->intf1.owner;
}



/* Public API's */
graph_t* create_new_graph(char* topology_name);
node_t* create_graph_node(graph_t* graph, char* node_name);
void insert_link_between_two_nodes(node_t* node1,
                                   node_t* node2,
                                   char* from_if_name,
                                   char* to_if_name,
                                   unsigned int cost
                                   );

void dump_graph(graph_t *graph);
void dump_node(node_t* node);
void dump_interface(interface_t* intf);
