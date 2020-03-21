#ifndef NODE0_H_
#define NODE0_H_

#include "prog3.h"

void rtinit0();
void rtupdate0(struct rtpkt* rcvdpkt);
void linkhandler0(int linkid, int newcost);
void printdt0(struct distance_table* dtptr);

#endif