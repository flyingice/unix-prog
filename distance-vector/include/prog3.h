#ifndef PROG3_H_
#define PROG3_H_

#define LINKCHANGES 0

/* possible events: */
#define FROM_LAYER2 2
#define LINK_CHANGE 10

/* infinite costs between two nodes */
#define INFINITY 999

struct event {
  float evtime;           /* event time */
  int evtype;             /* event type code */
  int eventity;           /* entity where event occurs */
  struct rtpkt* rtpktptr; /* ptr to packet (if any) assoc w/ this event */
  struct event* prev;
  struct event* next;
};

/* a rtpkt is the packet sent from one routing update process to
   another via the call tolayer3() */
struct rtpkt {
  int sourceid;   /* id of sending router sending this pkt */
  int destid;     /* id of router to which pkt being sent
                     (must be an immediate neighbor) */
  int mincost[4]; /* min cost to node 0 ... 3 */
};

struct distance_table {
  int costs[4][4];
};

float jimsrand();
void insertevent(struct event* p);
void printevlist();
void init();
void creatertpkt(struct rtpkt* initrtpkt, int srcid, int destid, int mincosts[]);
void tolayer2(struct rtpkt packet);

#endif