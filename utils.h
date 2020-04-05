/*
 * utils.h
 *
 * common utility functions and macros
 *  Author: yogesh kulkarni(yogidk@gmail.com)
 */

#pragma once

#define IPV4_ADDR_LEN_IN_BYTES          16 // 16 bytes will be ddd.ddd.ddd.ddd + null

#define UNSET_BIT(n, pos)       (n = n & ((1 << pos) ^ 0xFFFFFFFF))
#define SET_BIT(n, pos)         (n = n | 1 << pos)




//  macro to check if addr is a broadcast
#define IS_MAC_BROADCAST_ADDR(mac) \
        (mac[0] == 0xFF  &&  mac[1] == 0xFF && mac[2] == 0xFF && \
        mac[3] == 0xFF  &&  mac[4] == 0xFF && mac[5] == 0xFF)


void
apply_mask(char *prefix, char mask, char *str_prefix);

void
layer2_fill_with_broadcast_mac(char *mac_array);
