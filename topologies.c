#include "graph.h"
#include "comm.h"


/*
     Topology looks like this:
                       +----------+
                   0/4 |          |0/0
      +----------------+   R0_re  +---------------------------+
      |  40.1.1.1/24   |122.1.1.0 | 20.1.1.1/24               |
      |                +----------+                           |
      |                                                       |
      |                                                       |
      |                                                       |
      | 40.1.1.2/24                                           | 20.1.1.2/24
 0/5  |                                                  0/1  |
  +---+------+                                              +----+-----+
  |          |0/3                                        0/2|          |
  | R2_re    +----------------------------------------------+  R1_re   +
  |122.1.1.2 | 30.1.1.2/24                     30.1.1.1/24  |122.1.1.1 |
  +----------+                                              +----------+

  PS:  R0_re, R1_re and R2_re represent nodes
       0/0, 0/1, 0/3, 0/5 etc represent interfaces (must be read as eth0/0, eth0/1 etc)
       Each node can have multiple interfaces
       Each node has an IP called loopback IP addr
       Each interface of a node will have one MAC addr, and may have one IP addr / mask


 */
graph_t* build_first_topology()
{
    graph_t *topo = create_new_graph("Graph-0");

    node_t *R0_re = create_graph_node(topo,"R0_re");

    node_t *R1_re = create_graph_node(topo,"R1_re");

    node_t *R2_re = create_graph_node(topo,"R2_re");

#ifdef DEBUG_INFO
    printf("Nodes = %p,%p,%p\n",R0_re,R1_re,R2_re);
#endif



    insert_link_between_two_nodes(R0_re,R1_re,"eth0/0","eth0/1",1);

    insert_link_between_two_nodes(R0_re,R2_re,"eth0/4","eth0/5",1);

    insert_link_between_two_nodes(R1_re,R2_re,"eth0/2","eth0/3",1);

    set_node_loopback_addr(R0_re, "122.1.1.0");
    set_intf_of_node_ip_addr(R0_re, "eth0/0","20.1.1.1",24);
    set_intf_of_node_ip_addr(R0_re, "eth0/4","40.1.1.1",24);

    set_node_loopback_addr(R1_re, "122.1.1.1");
    set_intf_of_node_ip_addr(R1_re, "eth0/1","20.1.1.2",24);
    set_intf_of_node_ip_addr(R1_re, "eth0/2","30.1.1.1",24);

    set_node_loopback_addr(R2_re, "122.1.1.2");
    set_intf_of_node_ip_addr(R2_re, "eth0/3","30.1.1.2",24);
    set_intf_of_node_ip_addr(R2_re, "eth0/5","40.1.1.2",24);


    nw_start_pkt_rx_thread(topo);  //start the pkt recv thread
    return topo;

}
