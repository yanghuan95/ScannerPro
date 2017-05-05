/*************************************************************************
    > File Name: Scanner.c
    > Author: yh
    > Mail: yanghuancoder@163.com 
    > Created Time: Fri 05 May 2017 09:34:04 AM CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>

void connectAndScannerByRange(unsigned int min, unsigned int max);
void connectAndScanner(unsigned int IP);
unsigned int getNetworkIp(char *input); 

int main(int argc, char *argv[]){

	if(argc == 2){
		unsigned int IP = getNetworkIp(argv[1]);
		connectAndScanner(IP);
	}else if(argc == 3){
		unsigned int  min = getNetworkIp(argv[1]);
		unsigned int  max = getNetworkIp(argv[2]);
		min = ntohl(min);
		max = ntohl(max);
		if(min > max){
			unsigned int temp = min;
			min = max;
			max = temp;
		}
		connectAndScannerByRange(min, max);
	}else{
		printf("error args\n");
	}

	return 0;
}

void connectAndScannerByRange(unsigned int min, unsigned int max){
	unsigned int temp;
	for(int i = min; i <= max; ++i){
		temp = htonl(i);
		connectAndScanner(temp);
	}
}

void connectAndScanner(unsigned int IP_address){
	int probport = 0;
	int err, i, net;
	struct sockaddr_in sa;

	for(i = 1; i < 1024; ++i){
		strncpy((char *)&sa, "", sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_addr.s_addr = IP_address;
		sa.sin_port = htons(i);
		
		net = socket(AF_INET, SOCK_STREAM, 0);
		if(net < 0){
			perror("socket");
			exit(2);
		}

		err = connect(net, (struct sockaddr *)&sa, sizeof(sa));
		if(err < 0){
			printf("%s %-5d %s\r", inet_ntoa(sa.sin_addr), i, strerror(errno));
		}else{
			printf("%s %-5d accepted\n", inet_ntoa(sa.sin_addr), i);
			if(shutdown(net, 2) < 0){
				perror("shutdown");
				exit(2);
			}
		}

		close(net);
	}
}

unsigned int getNetworkIp(char *input){
	struct in_addr *ip;
	struct hostent *host;

	ip = (struct in_addr *) malloc(sizeof(ip));

	if(isdigit(input[0])){
		inet_aton(input, ip);
	}
	else if((host = gethostbyname(input)) != 0) {
		ip = (struct in_addr *) host->h_addr_list[0];
	}
	else{
		herror(input);
		exit(2);
	}

	return ip->s_addr;
}
