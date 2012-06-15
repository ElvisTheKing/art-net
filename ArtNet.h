#ifndef ArtNet_h
#define ArtNet_h

#include <SPI.h> 
#include <Ethernet.h>
#include <EthernetUdp.h> 

class ArtNet 
{
  public:
  ArtNet();
  void begin(EthernetUDP, IPAddress);
  void queue_channel(int,int);
  void flush();

protected:
  void construct_packet();
  EthernetUDP _udp;
  IPAddress _dest_ip;
  int _num_channels;
  int _universe;
  int _port;
  byte _data_buffer[512]; 
  byte _artnet_buffer[(17+512)+8+1];
};

#endif
// END OF FILE 
