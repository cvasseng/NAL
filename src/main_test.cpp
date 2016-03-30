/*
	
	Copyright (c) 2007, Chris V

	Licensed under the MIT license. See bundled LICENSE file for details.
	
*/

#include "nal_enet.h"
#include "nal_stream.h"

cNALHostEnet *connection = new cNALHostEnet;

struct testStruct
{
	char a;
	short b;
};

void recPacket(const sNALPacketHeader& header, void* data )
{
	if (header.size == sizeof(testStruct))
	{
		testStruct *res = (testStruct*)data;
		std::cout << res->a << " " << res->b << std::endl;
	}



	if (header.opCode == NAL_OP_PACKET_TEXT)
		std::cout << "Received Text: " << (char*)data << std::endl;
}

void serverLoop()
{
	connection->onPacket(recPacket);
	connection->setInfo(6000, "192.168.0.100", true);
	connection->enable(true);
	
	while (1)
	{
		connection->update();
		Sleep(100);
	}
}

void clientLoop()
{
	connection->onPacket(recPacket);
	connection->setInfo(6000, "192.168.0.100", false);
	connection->enable(true);


	testStruct testS;
	testS.a = 'H';
	testS.b = 5555;

	testStruct testS2;
	testS2.a = 'E';
	testS2.b = 1331;

	cNALPacket* test = new cNALPacket;
	test->setOpCode(1337);
	test->attachData(&testS, sizeof(testS));

	cNALPacket* test2 = new cNALPacket;
	test2->setOpCode(1337);
	test2->attachData(&testS2, sizeof(testS2));
	
	connection->PacketQueue.push(test);
	connection->PacketQueue.push(test2);
	
	while(1)
	{
	
		if (GetKeyState(VK_F5) & 0x800 )
		{
			std::string buffer = "";
			std::cin >> buffer;
		//	connection->sendString(buffer, true);
			if (buffer == "quit")
				break;
		}
		
		connection->update();	
	}
}





int main(int argc, char** argv)
{
	initNetworking();
	/*
	testStruct testS;
	testS.a = 'H';
	testS.b = 5555;
	cNALPacket* test = new cNALPacket;
	test->setOpCode(1337);
	test->attachData(&testS, sizeof(testS));

	char* data = test->serialize();

	sNALPacketHeader *header = (sNALPacketHeader*)data;
	testStruct *res = (testStruct*)(data + sizeof(sNALPacketHeader));

	std::cout << header->opCode << " " << header->size << std::endl;
	std::cout << res->a << " " << res->b << std::endl;
*/
	//std::cout << test->data.data << std::endl;
	
	std::cout << "Is this a server or a client? ";
	std::string buff;
	std::cin >> buff;
	if (buff == "server")
		serverLoop();
	else
		clientLoop();//*/

	uninitNetworking();
	return 0;
}