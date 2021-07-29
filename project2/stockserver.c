#include "csapp.h"
#include"bsttree.h"
#include"sbuf.h"
#include<time.h>
static int byte_cnt; /* Byte counter */
static sem_t mutex; /* and the mutex that protects it */
sbuf_t sbuf; /* Shared buffer of connected descriptors */
void* thread(void* vargp);
static void init_echo_cnt(void);
void echo_cnt(int connfd);
//clock_t start;
int main(int argc, char** argv)
{	
    int i, listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    FILE* input = fopen("stock.txt", "rt");
    if (input == NULL) {
        printf("fileopenfail"); exit(0);
    }
    int ID, left, price;
    while (1) {

        int ret = fscanf(input, "%d %d %d", &ID, &left, &price);
        if (ret < 0) break;
        Bdata temp = { ID, left, price, 0 };
        BSTInsert(&bstRoot, temp);
    }
    fclose(input);

    listenfd = Open_listenfd(argv[1]);
    sbuf_init(&sbuf, SBUFSIZE);
//	start = clock();
    for (i = 0; i < NTHREADS; i++) /* Create worker threads */
        Pthread_create(&tid, NULL, thread, NULL);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);

        sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
    }
}

void* thread(void* vargp)
{
    Pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buf */
        echo_cnt(connfd); /* Service client */
        Close(connfd);
	//	printf("%0.4f", (float) (clock()-start)/CLOCKS_PER_SEC);
    }

}

static void init_echo_cnt(void)
{
    Sem_init(&mutex, 0, 1);
    byte_cnt = 0;
}

void echo_cnt(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    Pthread_once(&once, init_echo_cnt);
    Rio_readinitb(&rio, connfd);

    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0){

		byte_cnt += n;

		printf("Server received %d (%d total) bytes on fd %d, %s", n, byte_cnt, connfd, buf);

		
	

		char* ptr = strtok(buf, " ");
		if (strcmp(ptr, "show\n") == 0) {
		
			Showfunc(connfd);
		}
		else if (strcmp(ptr, "buy") == 0) {

			ptr = strtok(NULL, " ");
			int num1 = atoi(ptr);
			ptr = strtok(NULL, " ");
			int num2 = atoi(ptr);

			int result;
		    result = Buyfunc(num1, num2);
			if (result == 1) {
				char cmd[100] = "[buy] success\n";
				int cmdsize = strlen(cmd);
				Rio_writen(connfd, cmd, cmdsize);
			}
			else if (result == 2) {
				char cmd[100] = "Not enough left stock\n";
				int cmdsize = strlen(cmd);
	
				Rio_writen(connfd, cmd, cmdsize);
			
			}
			else {
				char cmd[100] = "NO SUCH STOCK ID\n";
				int cmdsize = strlen(cmd);
				Rio_writen(connfd, cmd, cmdsize);
		
			}
		}
		else if (strcmp(ptr, "sell") == 0) {
	
			ptr = strtok(NULL, " ");
			int num1 = atoi(ptr);
			ptr = strtok(NULL, " ");
			int num2 = atoi(ptr);
			int result =0;
			 result = Sellfunc(num1, num2);
	
			if (result == 1) {
				char cmd[100] = "[sell] success\n";
	
				int cmdsize = strlen(cmd);
				Rio_writen(connfd, cmd, cmdsize);

			}


			else {
				char cmd[100] = "NO SUCH STOCK ID\n";
				int cmdsize = strlen(cmd);
				Rio_writen(connfd, cmd, cmdsize);
			}
		
		}
	
	
    }
	Filewrite();
}
