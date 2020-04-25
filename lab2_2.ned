simple App
{
    parameters:
        double app_msg_interval;
        int app_pck_size;
    gates:
        inout port;
}
simple IP
{
    parameters:
        string ip_addr;
    gates:
        inout app_port;
        inout eth_port;
}
simple Eth
{
    parameters:
        int ArpTTL;
        string MAC_addr;

    gates:
        inout ip_port;
        inout host_port;
}

simple Switch
{
    parameters:
        int aging_time;
        double switch_latency_time;
    gates:
        inout port1;
        inout port2;
        inout port3;
        inout port4;
}


module host
{
    parameters:

        int host_index;
        bool arp_Full;
        @display("bgb=210.105,308.43");
    gates:
        inout host_gate;

    submodules:
        ip: IP {
            @display("p=79.695,144.9");
        }
        app: App {
            @display("p=79.695,55.89");
        }
        eth: Eth {
            @display("p=79.695,230.805");
        }

    connections:
        app.port <--> ip.app_port;
        ip.eth_port <--> eth.ip_port;
        eth.host_port <--> host_gate;

}


network system
{
    @display("bgb=300,300");
    types:
        channel chan extends ned.DatarateChannel
        {
            datarate = 100 Mbps;
        }
    submodules:
        switch: Switch {
            @display("i=abstract/switch,cyan;p=132,139");
        }
        host1: host {
            @display("i=old/comp,black;p=225,225");
        }
        host2: host {
            @display("i=old/bwcomp,red;p=225,46");
        }
        host3: host {
            @display("i=old/comp_a,yellow;p=41,46");
        }
        host4: host {
            @display("i=old/comp,blue;p=41,225");
        }
    connections:
        host1.host_gate <--> chan <--> switch.port1;
        host2.host_gate <--> chan <--> switch.port2;
        host3.host_gate <--> chan <--> switch.port3;
        host4.host_gate <--> chan <--> switch.port4;
}
