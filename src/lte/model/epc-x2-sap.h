/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#ifndef EPC_X2_SAP_H
#define EPC_X2_SAP_H

#include "ns3/packet.h"
#include "ns3/eps-bearer.h"
#include "ns3/ipv4-address.h"

namespace ns3 {


class Node;

/**
 * The X2 SAP defines the service between the X2 entity and the RRC entity.
 * 
 * The X2 SAP follows the specification 3GPP TS 36.423: "X2 application protocol (X2AP)"
 * 
 * The service primitives corresponds to the X2AP procedures and messages and 
 * the service parameters corresponds to the Information Elements
 * 
 * Note: Any reference in this file refers to the 3GPP TS 36.423 specification
 */

/**
 * \brief Common structures for EpcX2SapProvider and EpcX2SapUser
 */
class EpcX2Sap
{
public:
  virtual ~EpcX2Sap ();
  
  /**
   * E-RABs to be setup item as
   * it is used in the HANDOVER REQUEST message.
   * See section 9.1.1.1 for further info about the parameters
   */
  struct ErabToBeSetupItem
  {
    uint16_t    erabId;
    EpsBearer   erabLevelQosParameters;
    bool        dlForwarding;
    Ipv4Address transportLayerAddress;
    uint32_t    gtpTeid;
    
    ErabToBeSetupItem ();
  };

  /**
   * E-RABs admitted item as
   * it is used in the HANDOVER REQUEST ACKNOWLEDGE message.
   * See section 9.1.1.2 for further info about the parameters
   */
  struct ErabAdmittedItem
  {
    uint16_t    erabId;
    uint32_t    ulGtpTeid;
    uint32_t    dlGtpTeid;
  };
  
  /**
   * E-RABs not admitted item as
   * it is used in the HANDOVER REQUEST ACKNOWLEDGE message.
   * See section 9.1.1.2 for further info about the parameters
   */
  struct ErabNotAdmittedItem
  {
    uint16_t    erabId;
    uint16_t    cause;
  };

  /**
   * UL Interference OverloadIndication as
   * it is used in the LOAD INFORMATION message.
   * See section 9.2.17 for further info about the values
   */
  enum UlInterferenceOverloadIndicationItem
  {
    HighInterference,
    MediumInterference,
    LowInterference
  };

  /**
   * UL High Interference Information as
   * it is used in the LOAD INFORMATION message.
   * See section 9.1.2.1 for further info about the parameters
   */
  struct UlHighInterferenceInformationItem
  {
    uint16_t    targetCellId;
    std::vector <bool> ulHighInterferenceIndicationList;
  };

  /**
   * Relative Narrowband Tx Power (RNTP) as
   * it is used in the LOAD INFORMATION message.
   * See section 9.2.19 for further info about the parameters
   * 
   * Note: You can use INT16_MIN value for -infinite value
   *       in the rntpThreshold field
   */
  struct RelativeNarrowbandTxBand
  {
    std::vector <bool> rntpPerPrbList;
    int16_t     rntpThreshold;
    uint16_t    antennaPorts;
    uint16_t    pB;
    uint16_t    pdcchInterferenceImpact;
  };

  /**
   * Cell Information Item as
   * it is used in the LOAD INFORMATION message.
   * See section 9.1.2.1 for further info about the parameters
   */
  struct CellInformationItem
  {
    uint16_t    sourceCellId;
    std::vector <UlInterferenceOverloadIndicationItem> ulInterferenceOverloadIndicationList;
    std::vector <UlHighInterferenceInformationItem> ulHighInterferenceInformationList;
    RelativeNarrowbandTxBand relativeNarrowbandTxBand;
  };


  enum IdCause
  {
    HandoverDesirableForRadioReason,
    TimeCriticalHandover
  };


  /**
   * \brief Parameters of the HANDOVER REQUEST message.
   *
   * See section 9.1.1.1 for further info about the parameters
   */
  struct HandoverRequestParams
  {
    uint16_t            oldEnbUeX2apId;
    uint16_t            cause;
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    uint64_t            ueAggregateMaxBitRateDownlink;
    uint64_t            ueAggregateMaxBitRateUplink;
    std::vector <ErabToBeSetupItem> bearers;
    Ptr<Packet>         rrcContext;
  };
  
  /**
   * \brief Parameters of the HANDOVER REQUEST ACKNOWLEDGE message.
   *
   * See section 9.1.1.2 for further info about the parameters
   */
  struct HandoverRequestAckParams
  {
    uint16_t            oldEnbUeX2apId;
    uint16_t            newEnbUeX2apId;
    uint16_t            sourceCellId;
    uint16_t            targetCellId;
    std::vector <ErabAdmittedItem> admittedBearers;
    std::vector <ErabNotAdmittedItem> notAdmittedBearers;
    Ptr<Packet>         rrcContext;
  };

  /**
   * \brief Parameters of the UE CONTEXT RELEASE message.
   *
   * See section 9.1.1.5 for further info about the parameters
   */
  struct UeContextReleaseParams
  {
    uint16_t            oldEnbUeX2apId;
    uint16_t            newEnbUeX2apId;
    uint16_t            sourceCellId;
  };

  /**
   * \brief Parameters of the LOAD INFORMATION message.
   *
   * See section 9.1.2.1 for further info about the parameters
   */
  struct LoadInformationParams
  {
    uint16_t            targetCellId;
    std::vector <CellInformationItem> cellInformationList;
  };

};


/**
 * These service primitives of this part of the X2 SAP
 * are provided by the X2 entity and issued by RRC entity
 */
class EpcX2SapProvider : public EpcX2Sap
{
public:
  virtual ~EpcX2SapProvider ();
  
  /**
   * Service primitives
   */

  virtual void SendHandoverRequest (HandoverRequestParams params) = 0;

  virtual void SendHandoverRequestAck (HandoverRequestAckParams params) = 0;

  virtual void SendUeContextRelease (UeContextReleaseParams params) = 0;

  virtual void SendLoadInformation (LoadInformationParams params) = 0;
};


/**
 * These service primitives of this part of the X2 SAP
 * are provided by the RRC entity and issued by the X2 entity
 */
class EpcX2SapUser : public EpcX2Sap
{
public:
  virtual ~EpcX2SapUser ();

  /**
   * Service primitives
   */

  virtual void RecvHandoverRequest (HandoverRequestParams params) = 0;

  virtual void RecvHandoverRequestAck (HandoverRequestAckParams params) = 0;

  virtual void RecvUeContextRelease (UeContextReleaseParams params) = 0;

  virtual void RecvLoadInformation (LoadInformationParams params) = 0;
};

///////////////////////////////////////

template <class C>
class EpcX2SpecificEpcX2SapProvider : public EpcX2SapProvider
{
public:
  EpcX2SpecificEpcX2SapProvider (C* x2);

  //
  // Interface implemented from EpcX2SapProvider
  //

  virtual void SendHandoverRequest (HandoverRequestParams params);

  virtual void SendHandoverRequestAck (HandoverRequestAckParams params);

  virtual void SendUeContextRelease (UeContextReleaseParams params);

  virtual void SendLoadInformation (LoadInformationParams params);
  
private:
  EpcX2SpecificEpcX2SapProvider ();
  C* m_x2;
};

template <class C>
EpcX2SpecificEpcX2SapProvider<C>::EpcX2SpecificEpcX2SapProvider (C* x2)
  : m_x2 (x2)
{
}

template <class C>
EpcX2SpecificEpcX2SapProvider<C>::EpcX2SpecificEpcX2SapProvider ()
{
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverRequest (HandoverRequestParams params)
{
  m_x2->DoSendHandoverRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendHandoverRequestAck (HandoverRequestAckParams params)
{
  m_x2->DoSendHandoverRequestAck (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendUeContextRelease (UeContextReleaseParams params)
{
  m_x2->DoSendUeContextRelease (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapProvider<C>::SendLoadInformation (LoadInformationParams params)
{
  m_x2->DoSendLoadInformation (params);
}

///////////////////////////////////////

template <class C>
class EpcX2SpecificEpcX2SapUser : public EpcX2SapUser
{
public:
  EpcX2SpecificEpcX2SapUser (C* rrc);

  //
  // Interface implemented from EpcX2SapUser
  //

  virtual void RecvHandoverRequest (HandoverRequestParams params);

  virtual void RecvHandoverRequestAck (HandoverRequestAckParams params);
  
  virtual void RecvUeContextRelease (UeContextReleaseParams params);

  virtual void RecvLoadInformation (LoadInformationParams params);

private:
  EpcX2SpecificEpcX2SapUser ();
  C* m_rrc;
};

template <class C>
EpcX2SpecificEpcX2SapUser<C>::EpcX2SpecificEpcX2SapUser (C* rrc)
  : m_rrc (rrc)
{
}

template <class C>
EpcX2SpecificEpcX2SapUser<C>::EpcX2SpecificEpcX2SapUser ()
{
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverRequest (HandoverRequestParams params)
{
  m_rrc->DoRecvHandoverRequest (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvHandoverRequestAck (HandoverRequestAckParams params)
{
  m_rrc->DoRecvHandoverRequestAck (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvUeContextRelease (UeContextReleaseParams params)
{
  m_rrc->DoRecvUeContextRelease (params);
}

template <class C>
void
EpcX2SpecificEpcX2SapUser<C>::RecvLoadInformation (LoadInformationParams params)
{
  m_rrc->DoRecvLoadInformation (params);
}

} // namespace ns3

#endif // EPC_X2_SAP_H
