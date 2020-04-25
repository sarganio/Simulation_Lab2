
#include "Switch.h"
#include <ARP_m.h>
#include <string.h>

typedef struct {
    simtime_t entryTime;
    char port[7];
    char MAC_Addr[17];
    int isEmpty = 1; //1-if empty, 0-not empty
} FilteringDatabase;

FilteringDatabase table[4];

Define_Module(Switch);

void Switch::initialize() {

}

void Switch::handleMessage(cMessage *msg) {

    char Port[7];
    int i, aging;
    char src_MAC_Addr[17], dst_MAC_Addr[17];
    double delay = par("switch_latency_time");
    ARP *message = check_and_cast<ARP *>(msg);

    // extract the incoming port name
    memcpy(Port, message->getArrivalGate()->getFullName(), 7);


    for (i = 0; i < 17; i++) {
        src_MAC_Addr[i] = message->getSmac(i);
        dst_MAC_Addr[i] = message->getDmac(i);
    }

    // Updating switch table

    aging = par("aging_time");
    for (i = 0; i < 4; i++) {   //checking if the source MAC is in the table
        if (!memcmp(table[i].MAC_Addr, src_MAC_Addr, 17)
                && (simTime() - table[i].entryTime) <= aging) {
            table[i].entryTime = simTime();
            break;
        }
        if(table[i].entryTime > aging){
            table[i].isEmpty=1;
        }
    }

    // update the first available entry
    if (i == 4) {
        for (i = 0; i < 4; i++) {
            if (table[i].isEmpty || table[i].entryTime > aging) {
                memcpy(table[i].MAC_Addr, src_MAC_Addr, 17);
                memcpy(table[i].port, Port, 7);
                table[i].entryTime = simTime();
                table[i].isEmpty = 0;

                break;
            }
        }
    }

    // Route message to the correct port
    for (i = 0; i < 4; i++)
        if (memcmp(dst_MAC_Addr, table[i].MAC_Addr, 17) == 0 && !table[i].isEmpty) {
            table[i].port[6] = 'o';
            sendDelayed(message->dup(), simTime() + delay, table[i].port);
            table[i].port[6] = 'i';
            break;
        }

    // broadcast
    if (i == 4) {
        //send only from a port that is not the port the packet arrive from
        if (memcmp(Port, "port1$i",7)!=0) {
            sendDelayed(message->dup(), simTime() + delay, "port1$o");
        }
        if (memcmp(Port, "port2$i",7)!=0) {
            sendDelayed(message->dup(), simTime() + delay, "port2$o");
        }
        if (memcmp(Port, "port3$i",7)!=0) {
            sendDelayed(message->dup(), simTime() + delay, "port3$o");
        }
        if (memcmp(Port, "port4$i",7)!=0) {
            sendDelayed(message->dup(), simTime() + delay, "port4$o");
        }
    }

}

