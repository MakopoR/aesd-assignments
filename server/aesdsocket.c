#include "aesdsocket.h"

bool is_daemon = false;

/*SIGNAL FUNC*****************/
bool caught_sigint = false;
bool caught_sigterm = false;
static void signal_handler (int signal_number)
{
	if(signal_number == SIGINT) {
		caught_sigint = true;
	} else if (signal_number == SIGTERM) {
		caught_sigterm = true;
	}
}

int main(int argc, char *argv[])
{
/*OPEN LOG*****************/
openlog(NULL,0,LOG_USER);
/*REGISTER SIGNALS*****************/
struct sigaction new_action;
memset(&new_action,0,sizeof(struct sigaction));
new_action.sa_handler = signal_handler;
if( sigaction(SIGTERM, &new_action, NULL) != 0) {
	syslog(LOG_ERR,"Error %d (%s) registering for SIGTERM",errno,strerror(errno));
	return -1;
}
if( sigaction(SIGINT, &new_action, NULL) != 0) {
        syslog(LOG_ERR,"Error %d (%s) registering for SIGINT",errno,strerror(errno));
        return -1;
}
/*CHECK ARGS*****************/
if(argc == 2)
{
	if(strcmp( argv[1], "-d") == 0){
		is_daemon = true;
	}
}
/*CREATE SOCKET*****************/
int sockfd = socket(PF_INET,SOCK_STREAM,0);

/*CREATE ADDRESS*****************/
int status;
struct addrinfo hints;
struct addrinfo *servinfo;

memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;

if((status = getaddrinfo(NULL, "9000", &hints, &servinfo)) != 0) {
	return -1;
	printf("ERROR getaddrinfo");
}

/*BIND ADDRESS*****************/
if(bind(sockfd, servinfo->ai_addr,sizeof(struct sockaddr)) != 0) {
	syslog(LOG_ERR,"ERROR bind");
	return -1;
}
int kidpid = 0;
if(is_daemon == true) {
	printf("is -d\n");
	kidpid = fork();
	if(kidpid == -1) {
		syslog(LOG_ERR,"UNABLE CREATE DAEMON");
		printf("unable");
		return -1;
	}
	else if(kidpid != 0)
	{
		printf("parent");
		freeaddrinfo(servinfo);
		close(sockfd);
		if(waitpid(kidpid,&status,0) > 0) {
                if(WIFEXITED(status) && !WEXITSTATUS(status)) {
                        if(WEXITSTATUS(status) != 0){
                                return -1;
                        }
                        else{
                                return 0;
                        }
                }
                else {
                        return -1;
                }
        	}	
        	else{
                	return -1;
        	}

	}
}

if(kidpid == 0)
{
printf("child");
printf("addr: %s\n",servinfo->ai_addr->sa_data);
if(listen(sockfd, 10) != 0) {
        syslog(LOG_ERR,"ERROR listen");
        //return -1;
	exit(1);
}
//
/*OPEN FILE*****************/
char filename[] = "/var/tmp/aesdsocketdata";
int packet_memory_fd=open(filename,O_RDWR|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);

if(packet_memory_fd == -1) {
 syslog(LOG_ERR,"Error opening file");
}
////////////////////////////////
int connfd = 0;
struct sockaddr_in clnt_addr;
int buffersize = 128;
socklen_t clnt_addr_size = sizeof(clnt_addr);
while(1)
{
	connfd = accept(sockfd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	syslog(LOG_INFO,"Accepted connection from %d",clnt_addr.sin_addr.s_addr);
	
	int i = 1;
	char *buffer = (char*) calloc(buffersize * i, sizeof(char));
	int received_packet_size = 0;
	while(1)
	{
		received_packet_size = recv(connfd,buffer+(buffersize * (i - 1)),buffersize, 0);
		//send(connfd,buffer,received_packet_size,0);
		//break;
		
		if(received_packet_size == buffersize) {
			char *buffer_t = (char*) calloc(buffersize * i, sizeof(char));
			memcpy(buffer_t, buffer, buffersize * i);
			i++;
			free(buffer);
			buffer = (char*) calloc(buffersize * i, sizeof(char));
			memcpy(buffer, buffer_t, buffersize * (i-1));
		}
		else {
			write(packet_memory_fd,buffer,buffersize*(i-1)+received_packet_size); 
		    	lseek(packet_memory_fd, 0, SEEK_SET); //get back to the beginning
			char buf[100];
    			int read_bytes;
    			int file_read=0;
			while(file_read == 0)
			{
				read_bytes = read(packet_memory_fd,buf,100);
				if(read_bytes < 100) {
					send(connfd,buf,read_bytes,0);
					break;
				}
				else {
					send(connfd,buf,100,0);
				}
			}
			break;
		}
	}

	close(connfd);
	syslog(LOG_INFO,"Closed connection from %d",clnt_addr.sin_addr.s_addr);
	
	if(caught_sigint) {
		syslog(LOG_INFO,"Caught signal, exiting");
		break;
	}
	if(caught_sigterm){
		syslog(LOG_INFO,"Caught signal, exiting");
		break;
	}

}

close(packet_memory_fd);
remove(filename);
freeaddrinfo(servinfo);
close(sockfd);
exit(0);
}
}
