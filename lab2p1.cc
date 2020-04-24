#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omnetpp.h>
using namespace omnetpp;

class Switch : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

class Application : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

class IP : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

class Ethernet : public cSimpleModule
{
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    bool switchInRange;
};

Define_Module(Switch);
Define_Module(Application);
Define_Module(IP);
Define_Module(Ethernet);


/* -------------- Switch -------------- */
void Switch::initialize()
{
    EV << "Initial switch\n";
}
void Switch::handleMessage(cMessage *msg)
{

}

/* -------------- Application -------------- */
void Application::initialize()
{
    EV << "Create Application message\n";
}
void Application::handleMessage(cMessage *msg)
{

}

/* -------------- IP -------------- */
void IP::initialize()
{
    EV << "Initial IP\n";
}
void IP::handleMessage(cMessage *msg)
{

}

/* -------------- Ethernet -------------- */
void Ethernet::initialize()
{
    EV << "Initial Ethernet\n";
    if(getParentModule()->getIndex() == 0)                         //it is Host_0
    {
        double x = getParentModule()->par("x");
        double y = getParentModule()->par("y");
        double radius = getParentModule()->par("radius");
        switchInRange= sqrt(pow(x,2)+pow(y,2)) <= radius;           //check the range from the host
        cDisplayString& dispStr = getParentModule()->getDisplayString();

        if(!switchInRange)  {
            dispStr.setTagArg("i",1,"red");
        }
        else {
            dispStr.setTagArg("i",1,"green");
        }
    }
}
void Ethernet::handleMessage(cMessage *msg)
{

}
