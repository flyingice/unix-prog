#ifndef NODE1_H_
#define NODE1_H_

#include "prog3.h"

void rtinit1();
void rtupdate1(struct rtpkt* rcvdpkt);
void linkhandler1(int linkid, int newcost);
void printdt1(struct distance_table* dtptr);

#endif