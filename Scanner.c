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
#include<signal.h>

int main(int argc, char **argv){
	int probport = 0;
	struct hostent *host;
	int err, i, net;
	struct sockaddr_in sa;

	for(i = 1; i < 1024; ++i){
		strncpy((char *)&sa, "", sizeof(sa));
		sa.sin_family = AF_INET;
		if(isdigit(*argv[1])){
			sa.sin_addr.s_addr = inet_addr(argv[1]);
		}
		else if((host = gethostbyname(argv[1])) != 0) 
			strncpy((char *)&sa.sin_addr, (char *)host->h_addr, sizeof(sa.sin_addr));
		else{
			herror(argv[1]);
			exit(2);
		}

		sa.sin_port = htons(i);
		
		net = socket(AF_INET, SOCK_STREAM, 0);
		if(net < 0){
			perror("socket");
			exit(2);
		}

		err = connect(net, (struct sockaddr *)&sa, sizeof(sa));
		if(err < 0){
		//	printf("%s %-5d %s\r", argv[1], i, strerror(errno));
		}else{
			printf("%s %-5d accepted\n", argv[1], i);
			if(shutdown(net, 2) < 0){
				perror("shutdown");
				exit(2);
			}
		}

		close(net);
	}

	printf("\r");
	fflush(stdout);
	return 0;
}
