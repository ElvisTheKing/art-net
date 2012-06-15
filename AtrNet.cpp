#include "ArtNet.h"
#include <SPI.h>
#include <Ethernet.h>
#include <Ethernet_udp.h> 



const char ART_NET_HEAD[8]="Art-Net";
const short ART_NET_OP_OUTPUT=0x5000;
const int ART_NET_HEADER_SIZE=17;

ArtNet::ArtNet() {
	_num_channels = 512;
	_universe = 0;
	_port = 6454;
}

void ArtNet::begin(EthernetUDP udp, IPAddress destination_ip) {
	_udp = udp;
	_dest_ip = destination_ip;
	construct_packet();
}

void ArtNet::queue_channel(int channel, int value) {
	_data_buffer[channel] = value;	
}

void ArtNet::construct_packet() {
	for (int i=0;i<7;i++) {
		_artnet_buffer[i]=ART_NET_HEAD[i];
	}   

	//Operator code low byte first  
	_artnet_buffer[8]= ART_NET_OP_OUTPUT;
	_artnet_buffer[9]= ART_NET_OP_OUTPUT >> 8;
	 //protocole
	_artnet_buffer[10]=0;
	_artnet_buffer[11]=14;
	 //sequence
	_artnet_buffer[12]=0;
	 //physical 
	_artnet_buffer[13] = 0;
	 // universe 
	_artnet_buffer[14]= _universe;//or 0
	_artnet_buffer[15]= _universe>> 8;
	//data length
	_artnet_buffer[16] = _num_channels>> 8;
	_artnet_buffer[17] = _num_channels;

	for (int t= 0;t<_num_channels;t++)
	{
		_artnet_buffer[t+ART_NET_HEADER_SIZE+1]=_data_buffer[t];    
	}
}

void ArtNet::flush() {
	construct_packet();

	_udp.beginPacket(_dest_ip, _port);
	_udp.write(_artnet_buffer,ART_NET_HEADER_SIZE+_num_channels+1);
	_udp.endPacket();	
}