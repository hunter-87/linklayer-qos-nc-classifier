//
// Copyright (C) 2005 Andras Varga
// Copyright (C) 2010 Alfonso Ariza
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//


#include <omnetpp.h>
#include "HiLoClassifier.h"
#include "IPDatagram.h"
#include "ICMPMessage.h"
#include "IPv6Datagram.h"
#include "ICMPMessage.h"
#include "IPv6NDMessage_m.h"
#include "UDPPacket_m.h"
#include "TCPSegment.h"

Register_Class(HiLoClassifier);

#define DEFAULT 0

HiLoClassifier::HiLoClassifier()
{
	std::cout << " HILO CLASSIFIER CREATED " << std::endl;
    defaultAC=DEFAULT;
}


int HiLoClassifier::getNumQueues()
{
    return 4;
}

int HiLoClassifier::classifyPacket(cMessage *msg)
{
	std::cout << " HILO CLASSIFIER ENABLED " << std::endl;

    IPDatagram *ipv4data = dynamic_cast<IPDatagram *>(PK(msg)->getEncapsulatedPacket());
    IPv6Datagram *ipv6data = dynamic_cast<IPv6Datagram *>(PK(msg)->getEncapsulatedPacket());

    if (ipv4data==NULL && ipv6data==NULL)
    {
        return 2;
    }
	UDPPacket *udp=NULL;
    TCPSegment *tcp=NULL;
    ICMPMessage *icmp=NULL;
    ICMPv6Message *icmpv6=NULL;
    if (ipv4data)
    {
        icmp = dynamic_cast<ICMPMessage *>(ipv4data->getEncapsulatedPacket());
    	udp=dynamic_cast<UDPPacket *>(ipv4data->getEncapsulatedPacket());
        tcp=dynamic_cast<TCPSegment *>(ipv4data->getEncapsulatedPacket());
    }
    if (ipv6data)
    {
        icmpv6 = dynamic_cast<ICMPv6Message *>(ipv6data->getEncapsulatedPacket());
    	udp=dynamic_cast<UDPPacket *>(ipv6data->getEncapsulatedPacket());
        tcp=dynamic_cast<TCPSegment *>(ipv6data->getEncapsulatedPacket());

    }
    if (icmp || icmpv6)
        return 2;

    if (udp)
    {
        if (udp->getDestinationPort() == 698 || udp->getSourcePort() == 698)
    	   return 3;
        else
           return 0;
    }
    if (tcp)
    {
            return 2;
    }
   	return defaultAC;
}


