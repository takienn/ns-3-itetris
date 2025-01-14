/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009-2010, Uwicore Laboratory (www.uwicore.umh.es),
 * University Miguel Hernandez, EU FP7 iTETRIS project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author:  Michele Rondinone <mrondinone@umh.es>
 */

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/event-id.h"
#include "ns3/service-list.h"
#include "ns3/iTETRISns3Facilities.h"
#include "ns3/CAMmanagement.h"
#include "ns3/C2C-IP-app.h"
#include "ns3/c2c-address.h"
#include "ns3/service-management.h"
#include "ns3/itetris-types.h"
#include "ns3/vehicle-sta-mgnt.h" 
#include "ns3/ip-base-sta-mgnt.h" 
#include "ns3/rsu-sta-mgnt.h"
#include "ns3/node-list.h"
#include "ns3/ipv4.h"
#include "addressing-support.h"

NS_LOG_COMPONENT_DEFINE ("AddressingSupport");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (AddressingSupport);

TypeId 
AddressingSupport::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::AddressingSupport")
    .SetParent<Object> ()
    .AddConstructor<AddressingSupport> ()
    ;
  return tid;
}

AddressingSupport::AddressingSupport ()
{
}

AddressingSupport::~AddressingSupport ()
{
}

void
AddressingSupport::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_node = 0;
  Object::DoDispose ();
}

void 
AddressingSupport::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this);
  m_node = node;
}

void
AddressingSupport::NotifyNewAggregate ()
{
  if (m_node == 0)
    {
      Ptr<Node>node = this->GetObject<Node>();
      if (node != 0)
        {
          this->SetNode (node);
        }
    }
  Object::NotifyNewAggregate ();
}


Ptr<c2cAddress> 
AddressingSupport::getC2Caddress(uint32_t destination)
{  
  if (m_node->IsMobileNode ())
    {
      Ptr<VehicleStaMgnt> staMgnt = m_node->GetObject<VehicleStaMgnt> ();
      NS_ASSERT_MSG (staMgnt, "VehicleStaMgnt object not found in the vehicle");
      Ptr<c2cAddress> c2caddress = staMgnt->GetC2cAddress (destination);
      return c2caddress; 
    }
  else
    {      
      Ptr<RsuStaMgnt> staMgnt = m_node->GetObject<RsuStaMgnt> ();
      NS_ASSERT_MSG (staMgnt, "RsuStaMgnt object not found in the RSU");
      Ptr<c2cAddress> c2caddress = staMgnt->GetC2cAddress (destination);
      return c2caddress; 
    }
}

Ptr<c2cAddress>
AddressingSupport::getC2CGeoBroadcastAddress(CircularGeoAddress destination)
{
   Ptr<c2cAddress> c2caddress = Create<c2cAddress>();
   c2caddress->Set (c2cAddress::BROAD, destination.lat, destination.lon);
   c2caddress->SetAreaSize (destination.areaSize);
   return c2caddress; 
}

Ptr<c2cAddress>
AddressingSupport::getC2CGeoAnycastAddress(CircularGeoAddress destination)
{
   Ptr<c2cAddress> c2caddress = Create<c2cAddress>();
   c2caddress->Set (c2cAddress::ANYCAST, destination.lat, destination.lon);
   c2caddress->SetAreaSize (destination.areaSize);
   return c2caddress; 
}

Ptr<c2cAddress>
AddressingSupport::getC2CTopoBroadcastAddress(uint32_t NHops)
{
  Ptr<c2cAddress> c2caddress = Create<c2cAddress>();
  c2caddress->Set (c2cAddress::BROAD, NHops);
  return c2caddress; 
}
                

Ipv4Address* 
AddressingSupport::getIPaddress(uint32_t destination)
{
	Ipv4Address* ipAddress;
	if (m_node->IsMobileNode ())
	{
		//Given that the "destination" is the destination universtal node id,
		//We can find that node and its allocated address. This might seem ugly BUT
		//The rational is that a vehicle's packets we pass through the eNB anyway, so it doesn't
		//make sense to modify LTE so we can probe all eNBs to request destination ipv4address
		Ptr<Node> dstNode = NodeList::GetNode(destination);

		for(uint32_t i = 1; i < dstNode->GetNDevices(); ++i)
		{
			Ptr<NetDevice> device = dstNode->GetDevice(i);
			std::string name = device->GetInstanceTypeId().GetName();
			if( name == "ns3::LteUeNetDevice" || name == "ns3::LteEnbNetDevice")
			{

				Ipv4Address addr = m_node->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();
				ipAddress = &addr;
				return ipAddress;
			}
		}

		Ptr<VehicleStaMgnt> staMgnt = m_node->GetObject<VehicleStaMgnt> ();
		NS_ASSERT_MSG (staMgnt, "VehicleStaMgnt object not found in the vehicle");
		ipAddress = staMgnt->GetIpAddress (destination);
		if(ipAddress != NULL)
		{
			NS_LOG_LOGIC ("ipadress "<<ipAddress->Get());
		}
		if(ipAddress != NULL)
			return ipAddress;
	}

	Ptr<IpBaseStaMgnt> staMgnt = m_node->GetObject<IpBaseStaMgnt> ();
	NS_ASSERT_MSG (staMgnt, "IpBaseStaMgnt object not found in the CIU");
	ipAddress = staMgnt->GetIpAddress (destination);
	return ipAddress;

}

} //namespace ns3
