
#include "list.h"
#include <stdlib.h>
#include <string.h>

void list::Clear(char* data, ulong len)
{
	for(ulong i=0; i<len; i++)
		*(data+i)=NULL;
}

bool list::InitList(int size, int datasize)
{
	/* InitList initializes a linked list of size elements
	returns TRUE if successful, and FALSE if failed */
	if(init)
		return false;
	/* first case is special */
	node *last, *ptr;
	start=ptr=last=end=(node*)malloc(sizeof(node));
	ptr->next=ptr->last=NULL;
	//Clear(ptr->path, MAXPATH);
	ptr->data=(void*)malloc(datasize);
	Clear((char*)ptr->data, (int)datasize);
	for(int i=1; i<size; i++)
	{
		ptr=(node*)malloc(sizeof(node));
		ptr->data=(void*)malloc(datasize);
		Clear((char*)ptr->data, (int)datasize);
		ptr->next=ptr->last=NULL;
		ptr->last=last;
		last->next=ptr;
		last=end=ptr;
	}
	cp=start;
	init=true;
	count=size;
	return true;
}

bool list::AddNode(int datasize)
{
	/* AddNode adds a node to the end of the list.
	Returns TRUE if successful, and FALSE if failed */
	if(!init)
		return false;
	node *last, *ptr;
	last=end;
	ptr=(node*)malloc(sizeof(node));
	ptr->data=(void*)malloc(datasize);
	ptr->next=ptr->last=NULL;
	Clear((char*)ptr->data, (int)datasize);
	ptr->last=last;
	if(last)
		last->next=ptr;
	count++;
	end=ptr;
	return true;
}

bool list::InsertNode(int datasize)
{
	/* InsertNode adds a node in front of the current node pointed to by cp.
	after inserting, cp points to the new node.
	Returns TRUE if successful, and FALSE if failed */
	if(!init)
		return false;
	node *last, *next, *ptr;
	last=cp;
	next=cp->next;
	ptr=(node*)malloc(sizeof(node));
	ptr->data=(void*)malloc(datasize);
	if(next)
		next->last=ptr;
	last->next=ptr;
	ptr->last=last;
	ptr->next=next;
	if(last==NULL) // if it was the first node
		start=ptr;
	if(next==NULL) // if it was the last node
		end=ptr;
	count++;
	cp=ptr;
	return true;
}

bool list::KillNode()
{
	/* KillNode deletes the current node pointed to by cp.
	after deleting, cp will point to the next item in the list AFTER the
	item which was deleted. if the item deleted is the last,
	it will point to the one previous.
	Returns TRUE if successful, and FALSE if failed */
	if(!init)
		return false;
	node *last, *next;
	next=cp->next;
	last=cp->last;
	free(cp->data);
	free((void*)cp); /* problem here */
	if(last)
		last->next=next;
	if(!last) // item which was deleted was = START
		start=next;
	if(next)
		next->last=last;
	if(!next) // item which was deleted was = END
		end=last;
	if(next)
		cp=next;
	else if(!next)
		cp=last;
	count--;
	return true;
}

bool list::DestroyList()
{
	/* DestroyList destroys the entire list.
		Returns TRUE if successful, and FALSE if failed */
	node *c;
	node *next;
	if(!init)
		return false;
	cp=start;
	c=cp;
	for(;;)
	{
		next=c->next;
		free(c->data);
		free(c);
		if(!next)
			break;
		c=next;
	}
	init=false;
	count=0;
	return true;
}

bool list::IsLastNode()
{
	if(cp==end)
		return true;
	if(!cp->next)
		return true;
	return false;
}

bool list::IsFirstNode()
{
	if(cp==start)
		return true;
	if(!cp->last)
		return true;
	return false;
}
