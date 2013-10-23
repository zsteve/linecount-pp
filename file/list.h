#ifndef LIST_H
#define LIST_H

#define MAXPATH 260
#ifndef NULL
#define NULL 0
#endif

class node{
public:
	node *next, *last;
	void *data;
};

enum{OK, LISTEND, LISTSTART};

typedef unsigned long ulong;
typedef unsigned char uchar;

class list{
private:
	node* cp, *start, *end; /* cp is the pointer to the current operating node
					and start is the pointer to the first node. */
	bool init;		// flag for initialized yes/no
public:
	int count;
	/*** inline functions ***/
	void ResetStart(){cp=start;}
	int Next(){
		if(cp->next==NULL)
			return LISTEND;
		cp=cp->next;
		return OK;
	}
	int Last(){
		if(cp->last==NULL)
			return LISTSTART;
		cp=cp->last;
		return OK;
	}
	bool IsInit(){return init;}
	node *GetCP(){return cp;}
	node *GetStart(){return start;}
	node *GetEnd(){return end;}
	void SetCP(node* n){cp=n;}
	void SetCPStart(){cp=start;}
	void SetCPEnd(){cp=end;}
	int EnumNodes()
	{
		int i;
		if(!init || !count)
			return 0;
		node *ptr=start;
		for(i=0; ;i++)
		{
			ptr=ptr->next;
			if(!ptr)
				break;
		}
		return i;
	}
	list()
	{
		cp=start=end=NULL;
		init=false;
		count=0;
	}
	void* GetDP()
	{
		if(!init)
			return 0;
		return cp->data;
	}
	/*** member functions ***/
	bool InitList(int size, int datasize);
	bool AddNode(int datasize);
	bool InsertNode(int datasize);
	bool KillNode();
	bool DestroyList();
	void Clear(char* data, ulong len);
	bool IsLastNode();
	bool IsFirstNode();
};
#endif
