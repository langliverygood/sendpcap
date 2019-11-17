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
#include <fcntl.h>


#include "pcap.h"
#include "print.h"

static char debug;
static char pcap_file_name[256];
static char interface[128];
static char ip_addr[16];
static uint16_t port;
static int udp_sock_fd;
static int cut_len;
static int loop_times = 1;
static int sleep_time;

void create_udp_socket()
{
	struct sockaddr_in addr;

	udp_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_sock_fd < 0)
	{
		print_errno("%s", "Create socket failed!");
		exit(-1);
	}
    if(debug)
    {
        printf("Socket created successfully!\n");
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
    if(debug)
    {
        printf("Socket bind %s:%d successfully!\n", ip_addr, port);
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
    opterr = 0;
	while ((ret = getopt_long(argc, argv, "f:o:i:p:c:l:s:vhd", long_options, &index)) != -1)
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
			case 'c':
				cut_len = atoi(optarg);
				break;
			case 'l':
				loop_times = atoi(optarg);
				break;
			case 's':
				sleep_time = atoi(optarg);
				break;
			case 'v':
				printf("%s version 1.0\n", PROMGRAM_NAME);
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
		printf(ARGUMENTS);
		exit(-1);
	}
	create_udp_socket();

	return;
}


int main(int argc, char *argv[])
{
	int file_fd;
	int ret, cnt;
	char buf[1500];
	struct pcap_file_header pf;
	struct packete_header ph;

	init(argc, argv);
	file_fd = open(pcap_file_name, O_RDONLY);
	if(file_fd < 0)
	{
        print_errno("Can't open file: %s", pcap_file_name);
		exit(-1);
	}

    if(debug)
    {
        printf("File: %s open successfully!\n", pcap_file_name);
    }
	ret = read(file_fd, &pf, sizeof(struct pcap_file_header));
	if(ret < 0)
	{
	    print_errno("Read file : %s error", pcap_file_name);
	}
	if(pf.magic != 0xa1b2c3d4)
	{
		print_error("File : %s isn't a pcap file!", pcap_file_name);
	}

	cnt = 0;
	while(1)
	{
		lseek(file_fd, sizeof(struct pcap_file_header), SEEK_SET);
		while((ret = read(file_fd, &ph, sizeof(struct packete_header))) > 0)
		{
			ret = read(file_fd, &buf, ph.caplen);
			if((ret = send(udp_sock_fd, buf + cut_len, ph.caplen - cut_len, 0)) < 0)
			{
				print_errno("%s", "Send packer error!");
				exit(-1);
			}
            if(debug)
            {
                printf("Send %d bytes.\n", ret);
            }
			usleep(sleep_time);
		}
        cnt++;
        if(debug)
        {
            printf("File: %s sent successfully! (%d)\n", pcap_file_name, cnt);
        }
		if(cnt == loop_times)
		{
			break;
		}
		if(loop_times <= 0)
		{
			cnt--;
		}
	}
	close(udp_sock_fd);
	
	return 0;		
}
