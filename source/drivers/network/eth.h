/* CakeOS */

#ifndef ETH_H
#define ETH_H

struct eth
{
     unsigned long ioaddr; //Default I/O address
     unsigned char packet[0x8000+16]; //Data packet
     unsigned int packet_len; //Packet length
     int id; //NIC ID
     int irq_num; //IRQ number
     char string[32]; //NIC card string
     unsigned char mac[6]; //MAC address
};

typedef struct eth eth_t;

//setup network devices
eth_t eth0, eth1;


#endif
