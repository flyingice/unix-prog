#ifndef NODE2_H_
#define NODE2_H_

#include "prog3.h"

void rtinit2();
void rtupdate2(struct rtpkt* rcvdpkt);
void linkhandler2(int linkid, int newcost);
void printdt2(struct distance_table* dtptr);

#endif