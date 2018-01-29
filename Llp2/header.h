#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

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


void generate_port(int i)
{
	port *p = (port*)malloc(sizeof(port));
	p->portNumber = i;
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
		  printf("Port: %d is open\n",curr->portNumber);
	  	  curr = curr->next;  
		}
		printf("Port: %d is open\n",curr->portNumber);
	}
	else printf("No ports where open\n");
}

void free_list()
{
	port* ptf = get_head();
	port* p = (port*)malloc(sizeof(port));
	
	while(ptf != NULL)
	{
		p = ptf;
		ptf= ptf->next;
		free(p);					
	}
}

#endif