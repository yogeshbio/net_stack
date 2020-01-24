/*
 *  net.h
 *
 *  Created on: Jan 17, 2020
 *  This file contains the network properties such as IP, MAC
 *  Author: yogesh kulkarni
 */
#pragma once
#include "stdio.h"
#include <stdbool.h>


#define IP_ADDR_SIZE 16
#define MAC_SIZE     6

typedef struct node_nw_props node_nw_props_t;
typedef struct intf_nw_props intf_nw_props_t;

/* MACROS to get ip and mac addresses from interface and node */

// For INTF
#define GET_INTF_MAC(intf_ptr)    (intf_ptr)->intf_nw_props.mac.mac_addr         // get interface mac addr
#define GET_INTF_IP(intf_ptr)     (intf_ptr)->intf_nw_props.intf_ip.ip_addr           // get interface ip addr
#define IS_INTF_IN_L3_MODE(intf_ptr)  ((intf_ptr)->intf_nw_props->is_ip_addr_configured)? true:false

// For NODE
#define GET_NODE_LB(node_ptr)     (node_ptr)->node_nw_props.loopback_ip.ip_addr  // get node loopback ip addr


// Common macros
#define NUMELEMS(x)  (sizeof(x) / sizeof((x)[0]))


typedef struct
{
    unsigned char ip_addr[IP_ADDR_SIZE];  // ip addr is of the format e.g. (192:168:182:145) which is 15 bytes and 1 NULL byte
}ip_addr_t;

typedef struct
{
    unsigned char mac_addr[MAC_SIZE];  // mac is 48 bit or 6 bytes
}mac_addr_t;


/*
   Each node has a loop back IP address,
   this is the NW property of the node
 */
struct node_nw_props
{
    /*
      Used to find various device types capabilities of
      the node and other features
     */
    unsigned int flags;
    bool is_loopback_configured;      // if true then below variable contains
    ip_addr_t loopback_ip;            // loopback ip addr of the node

};

/*

   Each interface will have a MAC addr (L2) and may also contain an IP addr (L3)

 */
struct intf_nw_props
{
    mac_addr_t    mac;                    // mac is burnt in the NIC
    bool          is_ip_addr_configured;  // if set to true, ip addr is configured and interface is in L3 mode
    ip_addr_t     intf_ip;
    char          mask;                   // every ip addr has a mask
};


static void inline
init_node_nw_property(node_nw_props_t* node_nw_prop)
{
    node_nw_prop->is_loopback_configured    = false;
    memset(node_nw_prop->loopback_ip.ip_addr,
            0,
            sizeof(node_nw_prop->loopback_ip.ip_addr));
}


static void inline
init_intf_nw_property(intf_nw_props_t* intf_nw_prop)
{
    memset(intf_nw_prop->mac.mac_addr,
            0,
            sizeof(intf_nw_prop->mac.mac_addr));

    intf_nw_prop->is_ip_addr_configured = false;

    memset(intf_nw_prop->intf_ip.ip_addr,
            0,
            sizeof(intf_nw_prop->intf_ip.ip_addr));

    intf_nw_prop->mask = 0;
}


/* API's to set NW node properties */
bool
set_node_loopback_addr(node_t* node, char* ipaddr);

bool
set_intf_of_node_ip_addr(node_t* node,
        char* if_name,
        char* ipaddr,
        char mask);

bool
unset_intf_of_node_ip_addr(node_t* node, char* if_name);

void
assign_intf_mac_addr(interface_t *intf);

void
dump_network_graph(graph_t* graph);

void
dump_node_network_props(node_t* node);

void
dump_intf_props(interface_t* intf);
