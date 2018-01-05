#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Ports
{ 
	int portNumber;
	char service[10]; 
	struct Ports* next;
}port;


/*List head */
port *list_head = NULL;

port* get_head()
{
	return list_head;
}

/* First Element */
void set_head(port* new_port)
{
	list_head = new_port;
}

port* find_last()
{
	if(get_head() == NULL)
		return NULL;

	port *curr = get_head(),
	     *last = get_head();

	while(curr != NULL)
	{	
		if(curr->next != NULL)
		last = curr->next;
		curr = curr->next;
	}
	
	return last;
}

void add_port(port* new_port)
{
	if(get_head() == NULL)
		set_head(new_port);
	else 
		find_last()->next = new_port;
}


void generate_port(int i, char *ser)
{
	port *p = (port*)malloc(sizeof(port));
	p->portNumber = i;
	strcpy(p->service,ser);
	p->next = NULL;
	add_port(p);
}

int list_size()
{
	int total = 0;
	port* curr = get_head();

	while(curr)
	{
		total++;
		curr = curr->next;
	}
	return total;
}

void print_list()
{
	port* curr = get_head();

	if(curr != NULL)
	{
	  while(curr->next != NULL)
		{
		  printf("Port: %d Service %s\n",curr->portNumber,curr->service);
	  	  curr = curr->next;  
		}
	}

	printf("Port: %d Service %s\n",curr->portNumber,curr->service);
}

int main(int argc, char ** argv)
{
	int sockfd=0;
	int min = atoi(argv[1]);
	int max = atoi(argv[2]);
	struct sockaddr_in serv_addr;
	struct hostent *url;

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return 1;
	}

	url = gethostbyname(argv[3]); /* does not support IP6 */
	if (url == NULL) {
		fprintf(stderr, "ERROR: Host not found\n");
		return 2;
	}

	printf("Scanning ports %d - %d\n",  min, max);

	FILE * file = fopen("/home/llp2/Desktop/Assignment/ports.txt", "w");//open log file

	for (unsigned int port = min; port < max; port++)//loop through and scan all ports
	{
			memset(&serv_addr, 0, sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			memcpy(&serv_addr.sin_addr, url->h_addr, url->h_length);
			serv_addr.sin_port = htons(port);
	/* Connect to the server */
		if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		{		    		
			printf("Port %d Closed\n", port);
		}
		else
		{
			//if the port is open then get information about that port and print it to the log file
	    		char host[128];
	    		char service[128];
	    		getnameinfo((struct sockaddr*)&serv_addr, sizeof serv_addr, host, (sizeof host), service, sizeof service, 0);
			// Insert in file. 
			fprintf(file, "Port : %d, Service : %s, Open\n", port, service); 
	
			//print in console.
			printf("Port : %d, Service : %s, Open\n", port, service);
	
			//add to linked list
			generate_port(port,service);
		}

  	}
	
	close(sockfd); 
    	//fclose(file);//Close the file

	print_list();
    return 0;
}
