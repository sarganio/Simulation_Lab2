

#include <App.h>
#include <App_pck_m.h>
Define_Module(App);

void App::initialize() {

}

void App::handleMessage(cMessage *msg) {
    App_pck *test = check_and_cast<App_pck *>(msg);
    EV << "Received a packet from IP layer `" << msg->getName()
              << "', sending packet out again to IP\n";
    send(msg, "port_out");

}
