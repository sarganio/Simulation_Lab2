#ifndef __EX2_ETH_H_
#define __EX2_ETH_H_

#include <omnetpp.h>
#include <ARP_m.h>
#include <unistd.h>

using namespace omnetpp;

class Eth : public cSimpleModule
{

    virtual ~Eth();

public:
    bool full_flag = false;
    bool flag1=true;
    bool flag2=true;
    bool flag3=true;
    bool flag4 = true;

  protected:

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    //virtual void initARP(ARP *msg);
    //virtual void updateArpTable(ARP *msg);
    virtual void checkIfThableFinished();

};


#endif
