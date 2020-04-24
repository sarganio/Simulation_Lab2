#include "Eth.h"
#include <math.h>
#include <string.h>
#include <ARP_m.h>

Define_Module(Eth);

Eth::~Eth() {

}


struct ARPentry {
    char MAC_addr[17];
    char IP_addr[7];
    simtime_t entry_time;
    int isEmpty;
};
ARPentry ARPTable[3];

int dstIndex;


void Eth::initialize() {
    int i;
    memset(ARPTable, 0, sizeof(ARPTable));   //clear the  arp table

    ARP *msg = new ARP("initial ARP Request");
    initARP(msg);

    for (i = 0; i < 4; i++)   //initialize arp table
        ARPTable[i].isEmpty = 1; // 1-Empty, 0-NotEmpty

    sendDelayed(msg, simTime() + exponential(0.5), "host_port$o"); //Sending out first arp massage
    EV << "host sent initial ARP message ";
}


void Eth::handleMessage(cMessage *msg) {

    ARP *message = check_and_cast<ARP *>(msg);
    ARP *NewMs = new ARP("new message");
    int my_index = getParentModule()->par("host_index");
    int i;
    simtime_t TimeToFin;
    updateArpTable(message); //Updating the ARP Table for each message we receive

    //TimeToFin is an argument that represent the time it will take the transmission to end
    TimeToFin =
            gate("host_port$o")->getTransmissionChannel()->getTransmissionFinishTime();
    //if the destination host is this one and it is an ARP request make and send an ARP response
    if (my_index == message->getTpa(6) && message->getOper() == 1) {

        // ARP response
        NewMs->setOper(2);

        // Sender IP address
        NewMs->setSpa(0, (char) 172);
        NewMs->setSpa(1, '.');
        NewMs->setSpa(2, (char) 167);
        NewMs->setSpa(3, '.');
        NewMs->setSpa(4, (char) 32);
        NewMs->setSpa(5, '.');
        NewMs->setSpa(6, (char) my_index);

        // Target IP address
        NewMs->setTpa(0, (char) 172);
        NewMs->setTpa(1, '.');
        NewMs->setTpa(2, (char) 167);
        NewMs->setTpa(3, '.');
        NewMs->setTpa(4, (char) 32);
        NewMs->setTpa(5, '.');
        NewMs->setTpa(6, message->getSpa(6));

        //  dmac = destination mac address, smac = source mac address
        for (i = 0; i < 16; i++) {
            NewMs->setSmac(i, message->getSmac(i));
            NewMs->setDmac(i, message->getDmac(i));
        }
        NewMs->setSmac(16, (char) my_index);
        NewMs->setDmac(16, message->getSpa(6));

        NewMs->setName("ARP Reply message");
        sendDelayed(NewMs, simTime() + TimeToFin, "host_port$o");
    }   //if

    //if it's an ARP reply OR a request not destinated to this host
    //if the ARP table is not full
    else if (ARPTable[0].isEmpty || ARPTable[1].isEmpty
            || ARPTable[2].isEmpty) {

        //Deciding the next host to get the address of
        for (i = 0; i < 3; i++) {
            if (dstIndex == (ARPTable[i].MAC_addr[16] - '0')) {
                while ((dstIndex = dstIndex % 4 + 1) == my_index)
                    ;
            }
        }

        // Sender IP address
        NewMs->setOper(1);
        NewMs->setSpa(0, (char) 172);
        NewMs->setSpa(1, '.');
        NewMs->setSpa(2, (char) 167);
        NewMs->setSpa(3, '.');
        NewMs->setSpa(4, (char) 32);
        NewMs->setSpa(5, '.');
        NewMs->setSpa(6, (char) my_index);

        // Target IP address
        NewMs->setTpa(0, (char) 172);
        NewMs->setTpa(1, '.');
        NewMs->setTpa(2, (char) 167);
        NewMs->setTpa(3, '.');
        NewMs->setTpa(4, (char) 32);
        NewMs->setTpa(5, '.');
        NewMs->setTpa(6, (char) dstIndex);

        // Sender & Target MAC address. Tha = target hardware address, smac = sender hardware address
        for (i = 0; i < 16; i++) {
            NewMs->setSmac(i, message->getSmac(i));
        }
        NewMs->setSmac(16, (char) my_index);

        NewMs->setName("ARP Request message");
        sendDelayed(NewMs, simTime() + TimeToFin, "host_port$o"); //ARP Request message
    } // else if
    else {
        full_flag = true;
        checkIfThableFinished();
    }

}

void Eth::updateArpTable(ARP *msg) {
    int i, TTL = par("ArpTTL");
    int Flag = 0;
    char IP_addr[7], MAC_addr[17];

    //get destination mac and destination ip
    for (i = 0; i < 17; i++) {
        MAC_addr[i] = msg->getDmac(i);
        if (i < 7)
            IP_addr[i] = msg->getTpa(i);
    }
    // matching entry exists
    for (i = 0; i < 3; i++) {
        if (!memcmp(ARPTable[i].MAC_addr, MAC_addr, 17)
                && (simTime() - ARPTable[i].entry_time) <= TTL) {
            ARPTable[i].entry_time = simTime();
            Flag = 1;
            break;
        }
    }

    //if entry doesn't exist or ttl of the entry is expire, we look for an available entry
    if (!Flag) {
        for (i = 0; i < 3; i++) {
            if (ARPTable[i].isEmpty
                    || (simTime() - ARPTable[i].entry_time) > TTL) {
                memcpy(ARPTable[i].IP_addr, IP_addr, 7);
                memcpy(ARPTable[i].MAC_addr, MAC_addr, 17);
                ARPTable[i].isEmpty = 0;
                ARPTable[i].entry_time = simTime();
                break;
            }
        }
    }

    // Update time for each entry
    for (i = 0; i < 3; i++)
        if ((simTime() - ARPTable[i].entry_time) > TTL) {
            ARPTable[i].isEmpty = 1;
        }
}

void Eth::checkIfThableFinished() {
    int i;

    // Checking for every host  if is arp table is full
    Eth *eth = check_and_cast<Eth *>(
            getModuleByPath("system")->getSubmodule("host1")->getSubmodule(
                    "eth"));
    if (!eth)
        error("Module doesn't exist");
    if (!eth->full_flag) // If host1's arp table is not full
        return;

    // Print table
    if (flag1) {
        for (i = 0; i < 3; i++) {
            EV << "host1 table \n";
            EV << "Entry " << i << " is full" << "\n";
        }
        flag1 = false;
    }

    // Checking  host2's arp table
    eth = check_and_cast<Eth *>(
            getModuleByPath("system")->getSubmodule("host2")->getSubmodule(
                    "eth"));
    if (!eth)
        error("Module doesn't exist");
    if (!eth->full_flag)
        return;

    // Print table
    if (flag2) {
        for (i = 0; i < 3; i++) {
            EV << "host2 table \n";
            EV << "Entry " << i << " is full" << "\n";
        }
        flag2 = false;
    }

    // Checking host3's arp table is full
    eth = check_and_cast<Eth *>(
            getModuleByPath("system")->getSubmodule("host3")->getSubmodule(
                    "eth"));
    if (!eth)
        error("Module doesn't exist");
    if (!eth->full_flag)
        return;

    // Print host3's table
    if (flag3) {
        for (i = 0; i < 3; i++) {
            EV << "host3 table \n";
            EV << "Entry " << i << " is full" << "\n";
        }
        flag3 = false;
    }

    // Checking host4's arp table is full
    eth = check_and_cast<Eth *>(
            getModuleByPath("system")->getSubmodule("host4")->getSubmodule(
                    "eth"));
    if (!eth)
        error("Module doesn't exist");
    if (!eth->full_flag)
        return;
    // Print table
    if (flag4) {
        for (i = 0; i < 3; i++) {
            EV << "host4 table: \n";
            EV << "Entry " << i << " is full" << "\n";
        }
        flag4 = false;
    }

    EV << "ALL TABLES ARE FULL";
    endSimulation();
}
//This function initialize an ARP message
void Eth::initARP(ARP *msg) {
    int my_index = getParentModule()->par("host_index");
    const char *sender_MAC = par("MAC_addr");
    int i;

    dstIndex = my_index % 4 + 1;

    msg->setOper(1);

    for (i = 0; i < 17; i++) {
        msg->setSmac(i, sender_MAC[i]);
    }

    msg->setSpa(0, (char) 172);
    msg->setSpa(1, '.');
    msg->setSpa(2, (char) 167);
    msg->setSpa(3, '.');
    msg->setSpa(4, (char) 32);
    msg->setSpa(5, '.');
    msg->setSpa(6, (char) my_index);

    msg->setTpa(0, (char) 172);
    msg->setTpa(1, '.');
    msg->setTpa(2, (char) 167);
    msg->setTpa(3, '.');
    msg->setTpa(4, (char) 32);
    msg->setTpa(5, '.');
    msg->setTpa(6, (char) dstIndex);

}
