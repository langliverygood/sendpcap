#ifndef _PRINT_
#define _PRINT_

#define print_errno(fmt, args...) printf("\033[;31m[errno = %d(%s)]:\033[0m"#fmt"\r\n", errno, strerror(errno), ##args)
#define print_error(fmt, args...) printf("\033[;31m"#fmt"\033[0m\r\n", ##args)

#define ARGUMENTS "\
Usage:\n\
$ ./NetFlow2MySQL -i <listen interface> -p <listen port>\n\
                  [--receive-buffer-size <receive buffer size of UDP socket>]\n\
                  [--mysql-host <mysql host name>]\n\
                  [--mysql-user <mysql-user>]\n\
                  [--mysql-password <mysql password>]\n\
                  [--mysql-database <mysql database>]\n\
                  [--mysql-port <mysql port>]\n\
                  [--store-exporter-address]\n\
                  [-d]\n\
                  [--packet-sampling-rate <sampling rate>]\n\
$ ./NetFlow2MySQL -v\n\
See\n\
http://cluster19.aist-nara.ac.jp/public/NetFlow2MySQL/NetFlow2MySQL.html\n\
for details.\n"


#endif
