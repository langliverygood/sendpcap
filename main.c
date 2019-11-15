#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <errno.h>

#include "pcap.h"
#include "print.h"

#define SERVER_PORT 6000
#define BUFF_LEN 512
#define SERVER_IP "127.0.0.1"

int main(int argc, char **argv)
{
	FILE *fp;
	char buf[100];
	int n;
	struct pcap_file_header pf;
	struct packete_header ph;
	
	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
        print_errno("Can't open file: %s", argv[1]);
		return 1;
	}
	n = fread(&buf, 1, , fp);
	printf("%d\n", n);
	
	
	return 0;		
}
