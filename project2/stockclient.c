/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;
	FILE * input = fopen ( "stock.txt", "rt");
	if(input ==NULL) {
	printf("FILE OPEN FAIL\n");
	return 0;
	}
	int filenum =0;
	while(1) {
	int a,b,c;
	int res= fscanf(input ,"%d %d %d", &a, &b, &c);
	if( res==EOF) break;
	filenum++;
	}
	fclose(input);
	
    if (argc != 3) {
	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while (Fgets(buf, MAXLINE, stdin) != NULL) {
	Rio_writen(clientfd, buf, strlen(buf));
	if(strcmp(buf, "show\n")==0) {
	
		for(int i= 0 ; i< filenum ;i++) {
		Rio_readlineb(&rio, buf, MAXLINE);
		Fputs(buf, stdout);
		}
	}
	else if (strcmp(buf, "exit\n")==0) {
		break;
	}
	else{ 
		
		Rio_readlineb(&rio, buf, MAXLINE);
		Fputs(buf, stdout);}

	
	} 
	
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */
