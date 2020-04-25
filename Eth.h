//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __EX2_ETH_H_
#define __EX2_ETH_H_

#include <omnetpp.h>
#include <ARP_m.h>
#include <unistd.h>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
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
    virtual void initARP(ARP *msg);
    virtual void updateArpTable(ARP *msg);
    virtual void checkIfThableFinished();

};


#endif
