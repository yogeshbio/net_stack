/*
 * net.c
 *
 *  Created on: Jan 18, 2020
 *  Author: yogesh kulkarni
 *  This file contains the network properties such as IP, MAC
 */
#include "graph.h"
#include "net.h"
#include "stdint.h"


/*
   Hash function to generate a random number for MAC. This hash function is said to have minimal collisions.
   No guarantee on this, but this is as per some stack overflow comment
 */
uint64_t MurmurOAAT64_hash ( const char * key)
{
    uint64_t h = (525201411107845655ull);
    for (;*key;++key) {
        h ^= *key;
        h *= 0x5bd1e9955bd1e995;
        h ^= h >> 47;
    }
    return h;
}

/*
 * Assign the mac addr to an interface. Use a hash function to generate a random value
 * and use only 6 bytes(48 bits) from it.
 */
void
assign_intf_mac_addr(interface_t *intf)
{
    char mac_val[6];
    node_t* node = intf->owner;
    if(!node)
    {
        printf("ERROR!! No node owner for the given interface!!!\n");
        return;
    }

    // Calculate a random mac addr using hash

    uint64_t hashval =  MurmurOAAT64_hash(intf->if_name); //use interface name to calculate hash, this is a 64 bit val, only 6 bytes = 48 bits of this will be used.
    memset(GET_INTF_MAC(intf), 0, NUMELEMS(GET_INTF_MAC(intf)));
    memcpy(GET_INTF_MAC(intf), (char*)&hashval, NUMELEMS(GET_INTF_MAC(intf))); // Not sure if typecasting hashval to char* is really necessary as memcpy does this anyways
}

/*
 * Set loopback address of the node
 */
bool
set_node_loopback_addr(node_t* node, char* ipaddr)
{
    assert(node);
    if(!ipaddr)     // null check
    {
        printf("Invalid IP addr !!");
        return false;
    }
    node->node_nw_props.is_loopback_configured = true;
    memcpy(GET_NODE_LB(node), ipaddr, IP_ADDR_SIZE);
    GET_NODE_LB(node)[IP_ADDR_SIZE-1] = '\0'; // add null for termination
    return true;
}

/*
 * Each node has an interface and each interface can have an IP addr. Given a node and
 * an interface name, assign an ip addr and mask to this interface
 */
bool
set_intf_of_node_ip_addr(node_t* node, char* if_name, char* ipaddr, char mask)
{
    assert(node);
    assert(ipaddr);

    interface_t* intf = get_intf_by_name_from_node(node, if_name);
    if(!intf)
    {
        printf(" Invalid Interface !!");
        return false;
    }
    intf->intf_nw_props.is_ip_addr_configured = true;
    memcpy(GET_INTF_IP(intf), ipaddr, IP_ADDR_SIZE);
    GET_INTF_IP(intf)[IP_ADDR_SIZE-1] = '\0'; // add null for termination
    intf->intf_nw_props.mask     = mask;
    return true;
}


bool
unset_intf_of_node_ip_addr(node_t* node, char* if_name)
{
    interface_t* intf = get_intf_by_name_from_node(node, if_name);
    if(!intf)
    {
        printf(" Invalid Interface !!");
        return false;
    }
    intf->intf_nw_props.is_ip_addr_configured = false;
    memset(GET_INTF_IP(intf), 0, IP_ADDR_SIZE);
    return true;
}



/*
 * Dump Functions of the Network.
 * node properties
 * interface properties
 */

void dump_intf_props(interface_t* intf)
{
    dump_interface(intf); // dump the interface details

    if(intf->intf_nw_props.is_ip_addr_configured)
    {
        printf("\t IP ADDR: %s/%u", GET_INTF_IP(intf), intf->intf_nw_props.mask);
    }
    else
    {
        printf("\t IP ADDR:%s/%u", "Nil",0);
    }
    printf("\t MAC ADDR: %02x:%02x:%02x:%02x:%02x:%02x\n",
            GET_INTF_MAC(intf)[0], GET_INTF_MAC(intf)[1],
            GET_INTF_MAC(intf)[2], GET_INTF_MAC(intf)[3],
            GET_INTF_MAC(intf)[4], GET_INTF_MAC(intf)[5]
    );
}

void dump_node_network_props(node_t* node)
{
    printf("==========================================================\n");
    printf("Node Name: %s\n",node->node_name);
    printf("Node Flags:%d\n",node->node_nw_props.flags);
    if(node->node_nw_props.is_loopback_configured)
    {
        printf("\t loopback addr:%s/32\n",GET_NODE_LB(node));
    }
    interface_t *intf;
    for( int i=0; i < MAX_INTF_PER_NODE; i++){

        intf = node->intf[i];
        if(!intf) break;
        dump_intf_props(intf);
    }
}

void dump_network_graph(graph_t* graph)
{
    node_t* node;
    printf("Graph topology name: %s\n",graph->topology_name);

    struct list_head *position;
    CDL_FOREACH(position, &graph->node_list)  // here position gets filled with the address of list pointer of the node_t* struct.
    {
        node = list_entry(position, node_t , graph ); // This will give me the address of the actual node i.e. node_t* due to container_of macro
        dump_node_network_props(node);

    }
}
