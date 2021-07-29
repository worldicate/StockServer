/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include"csapp.h"
#include"bsttree.h"
#include<string.h>
#include<time.h>


int byte_cnt =0;
void echo(int connfd);
//clock_t start;
typedef struct { /* Represents a pool of connected descr iptors */
     int maxfd; /* Largest descr iptor in read_set */
     fd_set read_set; /* Set of al l act ive descr iptors */
     fd_set ready_set; /* Subset of descr iptors ready for reading */
     int nready; /* Number of ready descr iptors from select */
     int maxi; /* High water index into cl ient array */
     int clientfd[FD_SETSIZE]; /* Set of act ive descr iptors */
      rio_t clientrio[FD_SETSIZE]; /* Set of act ive read buffers */
} pool;

void init_pool(int listenfd, pool * p)
 {
     /* Ini t ial ly, there are no connected descr iptors */
         int i;
        p->maxi = -1;
        for (i = 0; i < FD_SETSIZE; i++) p->clientfd[i] = -1;
    
        /* Ini t ial ly, l istenfd is only member of select read set */
        p->maxfd = listenfd;
        FD_ZERO(&p->read_set);
        FD_SET(listenfd, &p->read_set);
   }

void add_client(int connfd, pool* p)
 {
     int i;
     p->nready--;
     for (i = 0; i < FD_SETSIZE; i++) /* Find an avai lable slot */
         if (p->clientfd[i] < 0) {
         /* Add connected descr iptor to the pool */
             p->clientfd[i] = connfd;
         Rio_readinitb(&p->clientrio[i], connfd);
       
             /* Add the descr iptor to descr iptor set */
            FD_SET(connfd, &p->read_set);
        
            /* Update max descr iptor and pool high water mark */
             if (connfd > p->maxfd) p->maxfd = connfd;
            if (i > p->maxi) p->maxi = i;
         break;
    }
    if (i == FD_SETSIZE) /* Couldn¡¯ t find an empty slot */
        app_error("add_client error : Too many clients");
}

void check_clients(pool* p)
 {
     int i, connfd, n;
     char buf[MAXLINE];
     rio_t rio;

         for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
			connfd = p->clientfd[i];
			rio = p->clientrio[i];
          /* If the descr iptor is ready, echo a text l ine from i t */
             if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) {
             p->nready--;
             if((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
				byte_cnt += n;
			
                 printf("Server received %d (%d total) bytes on fd %d %s",  n, byte_cnt, connfd, buf);

				char * ptr = strtok (buf, " ");
				if(strcmp(ptr, "show\n")==0) {
				Showfunc(connfd);
		
				}
				else if (strcmp(ptr,"buy")==0) {
					ptr = strtok(NULL, " " );
					int num1 = atoi(ptr);
					ptr = strtok(NULL, " ");
					int num2 = atoi (ptr);
		
					int result = Buyfunc(num1, num2);
				if(result ==1) {
				char cmd[100] = "[buy] success\n";
			
					int cmdsize = strlen(cmd);
			
					Rio_writen(connfd, cmd, cmdsize);
				}
				else if (result ==2)  {
				char cmd[100] = "Not enough left stock\n";
				int cmdsize = strlen(cmd);
				Rio_writen(connfd, cmd, cmdsize);
				}
				else {
				char cmd[100] = "NO SUCH STOCK ID\n";
				int cmdsize =strlen(cmd);
					Rio_writen(connfd, cmd, cmdsize);
				} 
				}
				else if (strcmp(ptr, "sell") ==0) {
					ptr= strtok(NULL , " ");
					int num1 = atoi (ptr);
					ptr= strtok(NULL, " ");
					int num2= atoi(ptr);
					int result = Sellfunc(num1, num2);
				if( result ==1) {
					char cmd[100] = "[sell] success\n";

	
					int cmdsize  =strlen(cmd);
					Rio_writen(connfd,cmd, cmdsize);

				}
			
				
				else {
					char cmd[100] = "NO SUCH STOCK ID\n";
					int cmdsize= strlen(cmd);
					Rio_writen(connfd,cmd, cmdsize) ;
				}
				}
			
				}
            
                 /* EOF detected, remove descr iptor from pool */
			 else {
                 Close(connfd);
                 FD_CLR(connfd, &p->read_set);
                 p->clientfd[i] = -1;
			 }
            
           
        }
    
    }
		 Filewrite();
	//	 printf("%0.4fs\n" , (float)(clock() -start)/CLOCKS_PER_SEC);
}


int main(int argc, char **argv) 
{
     
	
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
   // char client_hostname[MAXLINE], client_port[MAXLINE];
    static pool pool;

    if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
    }

    FILE* input = fopen("stock.txt", "rt");
    if (input == NULL) { printf("file open fail"); return 0; }
    BSTInit(&bstRoot);
    int ID, left, price;
    while (1) {
        int ret = fscanf(input, "%d %d %d", &ID, &left, &price);
        if (ret < 0) break;
        Bdata temp = { ID, left, price ,0 };
        BSTInsert(&bstRoot, temp);

    }
    fclose(input);	

    listenfd = Open_listenfd(argv[1]);
    init_pool(listenfd, &pool);
//	start = clock();
    while(1) {
           pool.ready_set = pool.read_set;
            pool.nready = Select(pool.maxfd + 1, &pool.ready_set, NULL, NULL, NULL);
  
            /* If l istening descr iptor ready, add new cl ient to pool */
           if (FD_ISSET(listenfd, &pool.ready_set)) {
			  clientlen = sizeof(struct sockaddr_storage);
             connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
             add_client(connfd, &pool);
           
			 }
   
            /* Echo a text l ine from each ready connected descr iptor */
             check_clients(&pool);
       
    }
    exit(0);
}
/* $end echoserverimain */
