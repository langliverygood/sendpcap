#ifndef _PCAP_H_
#define _PCAP_H_

#pragma pack(push)
#pragma pack(1)

/* pcap文件头部 */
struct pcap_file_header 
{
	uint32_t magic;
	uint16_t version_major;
	uint16_t version_minor;
	int32_t thiszone;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t linktype;
};

/* packet头部 */
struct packete_header 
{
	uint32_t timestamp_high;
	uint32_t timestamp_low;
	uint32_t caplen;
	uint32_t len;
};
#pragma pack(pop)

#endif
