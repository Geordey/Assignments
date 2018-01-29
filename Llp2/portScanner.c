#include "header.h"
int main(int argc, char ** argv)
{
	int sockfd=0;
	int min = atoi(argv[1]);
	int max = atoi(argv[2]);
	struct sockaddr_in serv_addr;
	struct hostent *url;

	printf("Scanning ports %d - %d\n",  min, max);

	FILE * file = fopen("/home/llp2/Desktop/LLP Assignment/LLP2/ports.txt", "w");//open log file

	for (unsigned int port = min; port <= max; port++)//loop through and scan all ports
	{
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
			
			// Insert in file. 
			fprintf(file, "Port : %d is open\n", port); 
	
			//add to linked list
			generate_port(port);
		}

  	}
 	close(sockfd);
	fclose(file);//Close the file
	
	printf("\n\n");
	printf("From Linked List:\n");
	printf("--------------------\n");
	print_list();
    return 0;
	free_list();
}
