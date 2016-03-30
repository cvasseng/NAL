/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#include "nal_enet.h"

NALHostEnet::NALHostEnet()
{
	m_peers = new NALPeerListEnet;
	m_host = 0;
	m_enabled  = false;
	m_connected = false;
}

NALHostEnet::~NALHostEnet()
{
	if (m_enabled)
		enet_host_destroy(m_host);
}

bool NALHostEnet::enable( bool flag )
{
	if (flag)
	{
		if (!m_addressString.empty() && m_port > 0)
		{
			if (enet_address_set_host(&m_address, m_addressString.c_str()) < 0)
			{
				log("Could not set address info");
				return false;
			}
			m_address.port = m_port;
		}
		else
		{
			log("No address info found!");	
			return false;
		}
	}
	

	if (m_isServer)
	{
		if (flag)
		{
			log("Enabling networking host");
			m_host = enet_host_create(&m_address, 100, 0, 0, 0);
			if (!m_host)
			{
				log("An error occurred when starting networking server");
				flag = false;
			}

		}
		else if (m_enabled)
		{
			log("Disabling networking host");
			enet_host_destroy(m_host);
		}
	}
	else
	{
		if (flag)
		{
			m_host = enet_host_create(0, 2, 2 , 57600, 57600 );
			if (m_host == NULL)
			{
				log("Error when creating network client");
				return false;
			}
			else
			{
				log("Network client created");
				if (!clientConnect())
					return false;
			}
		}
		else if (m_enabled)
		{
			log("Disabling networking client");
			enet_host_destroy(m_host);
		}
	}

	m_enabled = flag;
	if (m_enabled)
		log("Network Service Successfully Started");
	if (m_isServer)
		m_connected = flag;
	return m_enabled;
}


void NALHostEnet::listen()
{
	ENetEvent evnt;
	while (enet_host_service (m_host, &evnt, 5) > 0)
	{
		if (!evnt.peer)
		{
			return;
		}

		switch (evnt.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				{
					//Generate ID Hash
					std::stringstream ss;
					ss << evnt.peer->address.host << evnt.peer->address.port << "hSlhf9#=)/#jfSfjisla";
					std::string idHash = md5(ss.str());

					//Create the peer
					NALPeerEnet *tmp = new NALPeerEnet(idHash, evnt.peer);
					m_peers->add(tmp);
					//TODO: doOnConnect should return a bool, and the peer should only be added on true return
					//Do connect callback
					doOnConnect(tmp);

					break;
				}

			case ENET_EVENT_TYPE_RECEIVE:
				{
					///Extract the packet:

					NALPeer* sender = 0;
					for (unsigned int i = 0; i < m_peers->peerCount(); i++)
					{
						if (NALPeerEnet* peer = dynamic_cast<NALPeerEnet*>(m_peers->getPeer(i)))
						{
							if (evnt.peer->address.host == peer->getEnetPeer()->address.host &&
								evnt.peer->incomingPeerID == peer->getEnetPeer()->incomingPeerID)
							{
								sender = peer;
								break;
							}
						}

					}

					if (sender)
						NALPacketHeader* header = extractPackets(sender, (char*)evnt.packet->data, evnt.packet->dataLength);
					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy (evnt.packet);

					break;
				}
			case ENET_EVENT_TYPE_DISCONNECT:
//				std::cout << (char*)evnt.peer->data << " disconnected." << std::endl;

				/* Reset the peer's client information. */
				//TODO: remove from peer list
				NALPeer* sender = 0;
					for (unsigned int i = 0; i < m_peers->peerCount(); i++)
					{
						if (NALPeerEnet* peer = dynamic_cast<NALPeerEnet*>(m_peers->getPeer(i)))
						{
							if (evnt.peer->address.host == peer->getEnetPeer()->address.host &&
								evnt.peer->incomingPeerID == peer->getEnetPeer()->incomingPeerID)
							{
								sender = peer;
								break;
							}
						}

					}

					if (sender)
						doOnDisconnect(sender);

				evnt.peer -> data = NULL;
			}
	}
}

bool NALHostEnet::clientConnect()
{
	m_connected = false;

	if (!m_host)
		return false;

	log("Attempting to connect to server");
	ENetEvent evnt;

	//enet_address_set_host (&address, ip.c_str());
	//address.port = port;

	/* Initiate the connection, allocating the two channels 0 and 1. */
	ENetPeer* peer = enet_host_connect (m_host, &m_address, 2, 0);

	if (peer == NULL)
	{
		log("No available peers for initiating an ENet connection.\n");
		
		return false;
	}

	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service (m_host, &evnt, 5000) > 0 && evnt.type == ENET_EVENT_TYPE_CONNECT)
	{
		m_connected = true;
		m_enabled = true;

		//Add the peer to the peer list
		NALPeerEnet* tmp = new NALPeerEnet("LOCAL:ID", peer);
		m_peers->add(tmp);
		doOnConnect(tmp);

		return true;
	}
	else
	{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		log("Client connection failed");
		enet_peer_reset (peer);

		return false;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////

void NALPeerListEnet::sendPacket( NALPacket *packet )
{
	if (!packet)
		return;

	char* data = packet->serialize();
	unsigned int size = packet->size();
	if (!data)
		return;

	char flag = ENET_PACKET_FLAG_RELIABLE;
	if (packet->getDeliveryModel() == dsUNRELIABLE)
		flag = 0;
	ENetPacket *epacket = enet_packet_create((void*)data, size, flag);

	ENetPeer* peer = 0;
	if (packet->getDestination() != 0)
	{
		if (NALPeerEnet* dest = dynamic_cast<NALPeerEnet*>(packet->getDestination()) )
		{
			//std::cout << "Sending packet to " << dest->getEnetPeer()->address.host << std::endl;
			peer = dest->getEnetPeer();
			enet_peer_send(peer, 0, epacket);
		}
	}
	else
	{//The packet is a broadcast packet (no destination = send to all)

	//	enet_host_broadcast(m_)

		//std::cout << "Broadcasting packet" << std::endl;
		for (unsigned int i = 0; i < m_peers.size(); i++)
		{
			if (NALPeerEnet* dest = dynamic_cast<NALPeerEnet*>(m_peers[i]) )
			{
				peer = dest->getEnetPeer();
				enet_peer_send(peer, 0, epacket);
			//	break;
			}
			else
			{
				//Fuck, its all going to shit
				break;
			}
		}
	}

	//TODO: Clean up memory here!
	//packet and data MUST be deleted!
	delete packet;
	delete[] data;
}
