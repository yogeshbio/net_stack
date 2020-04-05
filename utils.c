/*
 * utils.c
 * General utility functions
 * Author: yogesh kulkarni(yogidk@gmail.com)
 */

#include "arpa/inet.h"
#include "utils.h"
/*
 * Apply mask value to a prefix and update it (to get the subnet prefix)
 * Params: prefix, input string representing IP addr
 *         mask: mask value between 0-32
 *         str_prefix: output string after applying mask to prefix (gives subnet prefix)
 * e.g. prefix = 142.1.1.1, mask 24, then str_prefix will store 142.1.1.0 (anding with 255:255:255:0)
 * The assumption here is that the packet from the host comes here.
 */

void
apply_mask(char *prefix, char mask, char *str_prefix)
{
    /* convert the char string into network address struct (convert ipv4 addr from
       text to binary form) using inet_pton. inet_pton() generates output in network byte
       order
    */
    unsigned int binary_prefix = 0;
    inet_pton(AF_INET, prefix, &binary_prefix); // Produces network byte order

    /* convert to host byte order (you can also use htonl as it is used here only
     * for convenience to get back host byte order to perform ANDing  in unset)
     */
    binary_prefix = ntohl(binary_prefix);


    for(int i=0; i < (32 - mask); i++) // unset/clear all the bits starting from LSB,
        UNSET_BIT(binary_prefix, i); // we do looping as masking is per bit

    /* Convert back to network byte order (its used for convenience again as
     * input to inet_ntop() must be this order. We can also use ntohl() as we want
     * just to change the order of bytes before calling inet_ntop()
     */

    binary_prefix = htonl(binary_prefix);

    /* Convert ipv4 addr from binary to text form , dst will be in string human
     * readable format ddd.ddd.ddd.ddd
     */
    inet_ntop(AF_INET, &binary_prefix, str_prefix, IPV4_ADDR_LEN_IN_BYTES);
}


void
layer2_fill_with_broadcast_mac(char *mac_array)
{
    mac_array[0] = 0xFF;
    mac_array[1] = 0xFF;
    mac_array[2] = 0xFF;
    mac_array[3] = 0xFF;
    mac_array[4] = 0xFF;
    mac_array[5] = 0xFF;


}
