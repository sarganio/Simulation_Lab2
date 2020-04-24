simple Application
{
    parameters:
        double app_msg_interval;
        int app_pck_size;
    gates:
        inout AppIpPort;
}

simple IP
{
    parameters:
        string ip_addr;
    gates:
        inout IpAppPort;
        inout IpEthPort;
}

simple Ethernet
{
    parameters:
        int ArpTTL;
        string MAC_addr;
    gates:
        inout EthHostPort;
        inout EthIpPort;
}

module Host
{
    parameters:
        @display("bgb=38.916,102.258");
        int host_index;
        bool arp_Full;
    gates:
        inout HostGate;
    submodules:
        app: Application {
            @display("p=21.114,13.662");
        }
        ip: IP {
            @display("p=21.114,48.852");
        }
        eth: Ethernet {
            @display("p=21.114,84.456");
        }
    connections:
        app.AppIpPort <--> ip.IpAppPort;
        ip.IpEthPort <--> eth.EthIpPort;
        eth.EthHostPort <--> HostGate;
}

simple Switch
{
    parameters:
        int aging_time;
        double switch_latency_time;
    gates:
        inout switchGate1;
        inout switchGate2;
        inout switchGate3;
        inout switchGate4;
}

network Net
{
    @display("bgb=250,210");
    types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 100Mbps;
        }
    submodules:
        host1: Host {
            @display("i=old/laptop,white;p=60,50");
        }
        host2: Host {
            @display("i=old/laptop,white;p=165,50");
        }
        host3: Host {
            @display("i=old/laptop,white;p=165,147");
        }
        host4: Host {
            @display("i=old/laptop,white;p=60,146");
        }
        switch: Switch {
            @display("i=abstract/switch,cyan;p=109,100");
        }

    connections:
        host1.HostGate <--> Channel <--> switch.switchGate1;
        host2.HostGate <--> Channel <--> switch.switchGate2;
        host3.HostGate <--> Channel <--> switch.switchGate3;
        host4.HostGate <--> Channel <--> switch.switchGate4;
}
