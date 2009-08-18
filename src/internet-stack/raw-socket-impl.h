#ifndef RAW_SOCKET_IMPL_H
#define RAW_SOCKET_IMPL_H

#include "ns3/socket.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-route.h"
#include "ipv4-l3-protocol.h"
#include <list>

namespace ns3 {

class NetDevice;
class Node;

class RawSocketImpl : public Socket
{
public:
  static TypeId GetTypeId (void);

  RawSocketImpl ();

  void SetNode (Ptr<Node> node);

  virtual enum Socket::SocketErrno GetErrno (void) const;
  virtual Ptr<Node> GetNode (void) const;
  virtual int Bind (const Address &address);
  virtual int Bind ();
  virtual int GetSockName (Address &address) const; 
  virtual int Close (void);
  virtual int ShutdownSend (void);
  virtual int ShutdownRecv (void);
  virtual int Connect (const Address &address);
  virtual int Listen (void);
  virtual uint32_t GetTxAvailable (void) const;
  virtual int Send (Ptr<Packet> p, uint32_t flags);
  virtual int SendTo (Ptr<Packet> p, uint32_t flags,const Address &toAddress);
  void SendByInterface (Ptr<Packet> packet, Ptr<const Ipv4Route> route);
  virtual uint32_t GetRxAvailable (void) const;
  virtual Ptr<Packet> Recv (uint32_t maxSize, uint32_t flags);
  virtual Ptr<Packet> RecvFrom (uint32_t maxSize, uint32_t flags,  
				Address &fromAddress);

  void SetProtocol (uint16_t protocol);
  bool ForwardUp (Ptr<const Packet> p, Ptr<NetDevice> device);
private:
  virtual void DoDispose (void);


  struct Data {
    Ptr<Packet> packet;
    Ipv4Address fromIp;
    uint16_t fromProtocol;
  };

  enum Socket::SocketErrno m_err;
  Ptr<Node> m_node;
  Ipv4Address m_src;
  uint16_t m_sport;
  Ipv4Address m_dst;
  uint16_t m_protocol;
  std::list<struct Data> m_recv;
  bool m_shutdownSend;
  bool m_shutdownRecv;
  uint32_t m_icmpFilter;
  std::list< Ptr<Ipv4Interface> > m_interfaces;
  Ptr<Ipv4L3Protocol> m_ipv4;
};

} // namespace ns3

#endif /* RAW_SOCKET_IMPL_H */
