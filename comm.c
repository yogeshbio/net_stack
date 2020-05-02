/*
 * comm.c
 *
 *  Created on: Apr 7, 2020
 *  Author: yogesh kulkarni (yogidk@gmail.com)
 *
 *  The steps would be :
 *   1. Assign unique UDP port number for each node
 *   2. Open UDP socket for all the nodes using the above unique port number
 *   3. Listen on the UDP socket with select()
 *   4. Nodes communicate by sending the data to destination node's port number with IP = 127.0.0.1
 */


#include "comm.h"
#include "graph.h"
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h> /*for struct hostent*/
#include "net.h"
#include <unistd.h> // for close

static int udp_port_no = 40000;
static char recv_buffer[MAX_PKT_BUFFER_SIZE];

static int get_unique_udp_port_no()
{
    return udp_port_no++;
}

/*
 * Called by every node during init. Each node gets a unique udp port to bind.
 */
void udp_init_socket(node_t *node)
{
    node->udp_port_no = get_unique_udp_port_no();

    int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP );

    if(socket_fd == -1){
        printf("Socket Creation Failed for node %s\n", node->node_name);
        return;
    }

    struct sockaddr_in node_addr;
    node_addr.sin_family      = AF_INET;
    node_addr.sin_port        = node->udp_port_no;
    /*
     * INADDR_ANY : The socket will be bound to all local interfaces i.e receives packets
       from all interfaces
       For example, suppose that a host has interfaces 0, 1 and 2 (eth0, eth1 and
       eth2). If a UDP socket on this host is bound using INADDR_ANY and udp port 8000,
       then the socket will receive all packets for port 8000 that arrive on interfaces
       0, 1, or 2. If a second socket attempts to Bind to port 8000 on interface 1,
       the Bind will fail since the first socket already ``owns'' that port/interface.
     */
    node_addr.sin_addr.s_addr = INADDR_ANY;
    /*
      bind: is to associate the parent socket with a unique port (e.g. 40000)
      as defined above
    */
    if (bind(socket_fd, (struct sockaddr *)&node_addr, sizeof(struct sockaddr)) == -1)
    {
        printf("Error : socket bind failed for Node %s\n", node->node_name);
        return;
    }

    node->udp_socket_fd = socket_fd;
}


static void
_pkt_rx(node_t*  receiving_node,
        char* pkt_rx_with_aux_data,
        unsigned int bytes_recvd)
{


}
/*   */
static void *
 _nw_pkt_rx_thread(void *arg){

    node_t *node;
    struct list_head *position;

    fd_set active_sock_fd_set,   // file descriptor set
           backup_sock_fd_set;

    int sock_max_fd = 0;
    int bytes_recvd = 0;

    graph_t *topo = (void *)arg;

    int addr_len = sizeof(struct sockaddr); //needed for standard UDP socket API calls
    struct sockaddr_in sender_addr;

    FD_ZERO(&active_sock_fd_set);   // set to zero
    FD_ZERO(&backup_sock_fd_set);



    /* From the graph_t*, its possible to get the node_t* using below technique
       due to intrusive list.
       We iterate through all the nodes to copy their udp fd's into FD set
     */

    //here position gets filled with the address of list pointer of the node_t* struct.
    CDL_FOREACH(position, &topo->node_list){

        // This will give the address of the actual node i.e. node_t* due to container_of macro
        node = list_entry(position, node_t , graph);

        if(!node->udp_socket_fd) // skip nodes that do not have udp socket FD
            continue;

        if(node->udp_socket_fd > sock_max_fd)
            sock_max_fd = node->udp_socket_fd; // get max value of the FD in the set

        FD_SET(node->udp_socket_fd, &backup_sock_fd_set);   //macro copies the file descriptor to the set

    }

    while(1){ // infinite loop to listen to

        /*
          When select system call unblocks, it internally updates the socket FD set.
          This is the behvior of the system call, hence when used within a loop we need
          to re-initalize the FD set with the original set. This is why the backup FD set is
          needed because active_fd_set will get modified
         */

        memcpy(&active_sock_fd_set, &backup_sock_fd_set, sizeof(fd_set));

        /* select is a blocking call and will exit only when some value is received on one
           of the udp descriptors of the set
        */
        select(sock_max_fd + 1, &active_sock_fd_set, NULL, NULL, NULL);

       /* At this stage we know that there is some data to read on one of the UDP
          descriptors, but we do not know the exact node for which the FD was set. Hence
          we iterate again through all the nodes and check if the UDP socket FD for that
          node was set ?
       */
        CDL_FOREACH(position, &topo->node_list){

            node = list_entry(position, node_t , graph);

            if(FD_ISSET(node->udp_socket_fd, &active_sock_fd_set)) // gives us the node for which data arrived
            {
                // Read the data for this node using UDP socket API
                memset(recv_buffer, 0, MAX_PKT_BUFFER_SIZE);
                bytes_recvd = recvfrom(node->udp_socket_fd, (char *)recv_buffer,
                        MAX_PKT_BUFFER_SIZE, 0, (struct sockaddr *)&sender_addr, &addr_len);
                /* process the recvd data in recv_buffer */
                _pkt_rx(node, recv_buffer, bytes_recvd);
            }
        }
    }
}

/*  create a thread to read or recv pkts for all the interfaces of the given
 *  node
 *  */
void nw_start_pkt_rx_thread(graph_t* topo)
{
    pthread_attr_t attr;
    pthread_t rx_pkt_thread;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&rx_pkt_thread, &attr,
                    _nw_pkt_rx_thread,
                    (void *)topo);
}
