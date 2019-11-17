#ifndef _PRINT_
#define _PRINT_

#define print_errno(fmt, args...) printf("\033[;31m[errno = %d(%s)]:\033[0m"#fmt"\r\n", errno, strerror(errno), ##args)
#define print_error(fmt, args...) printf("\033[;31m"#fmt"\033[0m\r\n", ##args)

#define PROMGRAM_NAME "sendpcap"

#define ARGUMENTS "\
Usage: sendpcap -o device_name -i ip_address -p port -f pcap_file_name\n\
                [-l loop_times] [-s sleep_time] [-c cut_length] [-d] [-v] [-h]\n\
\nDESCRIPTION:\n\
                  --interface <select a network device.] -o <necessary>\n\
                  --ip <target ip>] -i <necessary>\n\
                  --port <target port>] -p <necessary>\n\
                  --file <select a pcap file.>] -f <necessary>\n\
                  --loop <How many times to send pcap files.Default one time.>] -l <optional>\n\
                  --sleep <Interval for sending each pacekt(um).Default 0um>] -s <optional>\n\
                  --cut <Length of header to be removed for each pacekt.Default 0.>] -c <optional>\n\
                  --debug <Print some debug message.>] -d <optional>\n\
                  --version <Show current sendpcap's version.>] -v <optional>\n\
                  --help <Show this description>] -h <optional>\n"
#endif
