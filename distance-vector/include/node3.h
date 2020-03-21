#ifndef NODE3_H_
#define NODE3_H_

#include "prog3.h"

void rtinit3();
void rtupdate3(struct rtpkt* rcvdpkt);
void linkhandler3(int linkid, int newcost);
void printdt3(struct distance_table* dtptr);

#endif