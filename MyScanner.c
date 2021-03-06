/*************************************************************************
    > File Name: MyScanner.c
    > Author: yanghuan
    > Mail: yanghuancoder@163.com 
    > Created Time: Thu 04 May 2017 03:24:42 PM DST
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<ctype.h>

FILE *host_list;

struct sockaddr_in *fillSockAddr(char *input);
void connectAndScannerTestByRange(unsigned int min, unsigned int max);
void connectAndScanner(unsigned int IP);

int main(int argc, char *argv[]){
	
	struct hostent *host;

	if(argc == 2){
		//just scanner one IP's port
		struct sockaddr_in *sa = fillSockAddr(argv[1]);
		connectAndScanner(sa->sin_addr.s_addr);
	}else if(argc == 3){
		//scanner IP from argv[1] to argv[2]
		struct sockaddr_in *sa = fillSockAddr(argv[1]);
		unsigned int min_addr = sa->sin_addr.s_addr;
		sa = fillSockAddr(argv[2]);
		unsigned int max_addr = sa->sin_addr.s_addr;

		if(min_addr > max_addr){
			int temp = min_addr;
			min_addr = max_addr;
			max_addr = temp;
		}
		
		connectAndScannerTestByRange(min_addr, max_addr);

	}else{
		fprintf(stdout, "invaild input arg\n");
		exit(0);
	}
}

struct sockaddr_in *fillSockAddr(char *input){
	static struct sockaddr_in sock_in;
	struct hostent *host;
	memset(&sock_in, 0, sizeof(sock_in));
	
	sock_in.sin_family = AF_INET;

	if(isdigit(input[0])){
		sock_in.sin_addr.s_addr = inet_addr(input);
	}else if((host = gethostbyname(input)) != 0){
		strncpy((char *)&sock_in.sin_addr, (char *)host->h_addr, sizeof(sock_in.sin_addr));
	}else{
		fprintf(stdout, "invilad IP");
		exit(1);
	}

	return &sock_in;
}

void connectAndScannerTestByRange(unsigned int min, unsigned int max){
	for(unsigned int i = min; i <= max; ++i){
		connectAndScanner(i);
	}
}

void connectAndScanner(unsigned int IP){
	int net;
	int error;
	char *host;
	
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_addr.s_addr = IP;
	sa.sin_family = AF_INET;

	for(int j = 1; j < 1024; ++j){
		net = socket(AF_INET, SOCK_STREAM, 0);
		if(net < 0){
			perror("socket connect fail");
			exit(3);
		}
		sa.sin_port = htons(j);
		error = connect(net, (struct sockaddr *)&sa, sizeof(sa));
		if(error == 0){
			host = strcat(inet_ntoa(sa.sin_addr), ".txt");
			host_list = fopen(host, "a+");
			fprintf(host_list, "port: %d\n", j);
			fclose(host_list);

			if(shutdown(net, 2) < 0){
				perror("shutdown");
				exit(3);
			}
		}

	}

	close(net);
}
