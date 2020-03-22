#include "node2.h"

#include <stdio.h>

extern int TRACE;
extern int YES;
extern int NO;

static const int id = 2;
static int connectcosts2[4] = { 3, 1, 0, 2 };
static struct distance_table dt2;

/* students to write the following two routines, and maybe some others */

static void notifyNeighbors() {
  int mincosts[4] = {};
  for (int i = 0; i < 4; i++) {
    mincosts[i] = dt2.costs[i][id];
  }

  struct rtpkt pkt = {};
  for (int dest = 0; dest < 4; dest++) {
    if (dest != id && connectcosts2[dest] != INFINITY) {
      creatertpkt(&pkt, id, dest, mincosts);
      printf("%d notifies %d of the update\n", id, dest);
      tolayer2(pkt);
    }
  }
}

void rtinit2() {
  printf("init node %d\n", id);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      dt2.costs[i][j] = (j == id ? connectcosts2[i] : INFINITY);
    }
  }

  notifyNeighbors();
}

void rtupdate2(rcvdpkt) struct rtpkt* rcvdpkt;
{
  if (rcvdpkt == NULL || rcvdpkt->destid != id) {
    return;
  }

  printf("update distance table on node %d\n", id);
  int change = 0;
  int src = rcvdpkt->sourceid;
  // distance table [id][i] stores the minimum routig cost from node id to i
  for (int i = 0; i < 4; i++) {
    dt2.costs[i][src] = rcvdpkt->mincost[i];
    if (dt2.costs[i][src] + connectcosts2[src] < dt2.costs[i][id]) {
      change = 1;
      dt2.costs[i][id] = dt2.costs[i][src] + connectcosts2[src];
    }
  }

  printdt2(&dt2);

  if (change) {
    notifyNeighbors();
  }
}

void printdt2(dtptr) struct distance_table* dtptr;
{
  printf("                  via     \n");
  printf("   D0 |    0     1     2     3 \n");
  printf("  ----|-----------------------\n");
  printf("     0|  %3d   %3d   %3d   %3d\n",
         dtptr->costs[0][0],
         dtptr->costs[0][1],
         dtptr->costs[0][2],
         dtptr->costs[0][3]);
  printf("     1|  %3d   %3d   %3d   %3d\n",
         dtptr->costs[1][0],
         dtptr->costs[1][1],
         dtptr->costs[1][2],
         dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d   %3d\n",
         dtptr->costs[2][0],
         dtptr->costs[2][1],
         dtptr->costs[2][2],
         dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d   %3d\n",
         dtptr->costs[3][0],
         dtptr->costs[3][1],
         dtptr->costs[3][2],
         dtptr->costs[3][3]);
}
