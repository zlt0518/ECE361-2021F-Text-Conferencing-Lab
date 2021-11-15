#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <stdlib.h>

struct packet{
	unsigned int type;
	unsigned int size;
	unsigned char source[1050];
	unsigned char filedata[1050];
};

#endif