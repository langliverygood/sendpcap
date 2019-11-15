#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>

#include "pcap.h"
#include "print.h"

static char debug;
static char pcap_file_name[256];
static char interface[128];
static char ip_addr[16];
static uint16_t port;
static int udp_sock_fd;

void create_udp_socket()
{
	struct sockaddr_in addr;

	udp_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_sock_fd < 0)
	{
		print_errno("%s", "Create socket failed!");
		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_addr);
    addr.sin_port = htons(port);
    if(connect(udp_sock_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0)
    {
		print_errno("%s", "Create socket failed!");
		exit(-1);
    }

	return;
}

void init(int argc, char *argv[])
{
	int ret, index, i;
	static struct option long_options[] = {
		{"interface", required_argument, NULL, 'o'},
		{"port", required_argument, NULL, 'p'},
		{"ip", no_argument, NULL, 'i'},
		{"file", required_argument, NULL, 'f'},
		{"loop", required_argument, NULL, 'l'},
		{"sleep", required_argument, NULL, 's'},
		{"cut", required_argument, NULL, 'c'},
		{"debug", no_argument, NULL, 'd'},
		{"version", no_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{0,0,0,0}
	};

	i = 0;
	while ((ret = getopt_long(argc, argv, "f:o:i:p:vhd", long_options, &index)) != -1)
	{
		switch (ret)
		{
			case 'f':
				strncpy(pcap_file_name, optarg, sizeof(pcap_file_name));
				i++;
				break;
			case 'o':
				strncpy(interface, optarg, sizeof(interface));
				i++;
		    	break;
			case 'i':
				strncpy(ip_addr, optarg, sizeof(ip_addr));
				if(strcmp(ip_addr, "localhost") == 0)
				{
				   strcpy(ip_addr, "127.0.0.1");
				}
				i++;
				break;
	  		case 'p':
				port = atoi(optarg);
				i++;
		   	 	break;
			case 'v':
				printf("%s version 1.0\n", argv[0]);
				exit(0);
				break;
			case 'h':
				printf(ARGUMENTS);
				exit(0);
				break;
			case 'd':
				debug = 1;
				break;
			default:
				break;
		}
	}
	if(i != 4){
		perror(ARGUMENTS);
		exit(-1);
	}
	create_udp_socket();

	return;
}


int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[1500];
	int n;
	int i;
	struct pcap_file_header pf;
	struct packete_header ph;

	init(argc, argv);
	
	fp = fopen(pcap_file_name, "r");
	if(fp == NULL)
	{
        print_errno("Can't open file: %s", pcap_file_name);
		return 1;
	}
	n = fread(&pf, 1, sizeof(struct pcap_file_header), fp);
	if(n < 0)
	{
	    print_errno("Read file : %s error", pcap_file_name);
	}
	if(pf.magic != 0xa1b2c3d4)
	{
		print_error("File : %s isn't a pcap file!", pcap_file_name);
	}

	i = 0;
	while((n = fread(&ph, 1, sizeof(struct packete_header), fp)) > 0)
	{
		i++;
		n = fread(&buf, 1, ph.caplen, fp);
		n = send(udp_sock_fd, buf + 42, n - 42, 0);
	}
	printf("send sum :%d\n", i);
	close(udp_sock_fd);
	
	return 0;		
}
