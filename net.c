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
    //char mac_val[6];
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
    memcpy(GET_NODE_LB(node), ipaddr, IPV4_ADDR_LEN_IN_BYTES);
    GET_NODE_LB(node)[IPV4_ADDR_LEN_IN_BYTES-1] = '\0'; // add null for termination
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
    memcpy(GET_INTF_IP(intf), ipaddr, IPV4_ADDR_LEN_IN_BYTES);
    GET_INTF_IP(intf)[IPV4_ADDR_LEN_IN_BYTES-1] = '\0'; // add null for termination
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
    memset(GET_INTF_IP(intf), 0, IPV4_ADDR_LEN_IN_BYTES);
    return true;
}

/*
 *  Function to return the local interface of the node having the subnet
 *  in which 'ip_addr' lies i.e. the subnet of the ip addr and the subnet of the
 *  interface must be the same.
 */
interface_t *
node_get_matching_subnet_interface(node_t *node, char *ip_addr)
{
      interface_t *intf;
      char mask;
      char *intf_ip_addr = NULL;
      char intf_subnet[IPV4_ADDR_LEN_IN_BYTES]; // subnet of the interface
      char subnet2[IPV4_ADDR_LEN_IN_BYTES];    // subnet of the input ip

     // Go through all the interfaces and for each interface apply the mask
    for(int i=0; i<MAX_INTF_PER_NODE; i++)
    {
        intf = node->intf[i];
        if(!intf || !(IS_INTF_IP_ADDR_CONFIGURED(intf))) // check if the ip addr is configured
            continue;

        // Get ip and mask for the interface, and then use apply_mask() to get subnet prefix from the mask
        intf_ip_addr = GET_INTF_IP(intf);

        mask = GET_INTF_MASK(intf);
        memset(intf_ip_addr, 0, IPV4_ADDR_LEN_IN_BYTES);
        memset(subnet2, 0, IPV4_ADDR_LEN_IN_BYTES);
        apply_mask(intf_ip_addr, mask, intf_subnet); // fills the subnet prefix
        apply_mask(ip_addr, mask, subnet2);

        if(strncmp(intf_subnet, subnet2, IPV4_ADDR_LEN_IN_BYTES) == 0)
            return intf;

    }

    return NULL;
}


////////////////////////////////////////////////////////////////////////////

//                         DUMP FUNCTIONS

///////////////////////////////////////////////////////////////////////////

/*
 *     Dump Functions of the Network.
 *     node properties
 *     interface properties
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
