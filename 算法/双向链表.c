#include <stdio.h>

#define n 5

struct record { 
	char name[40];
	struct record *back;
	struct record *next;
}; 

struct record *creat();
void   show(struct record *,int);
struct record *changehead(struct record *);

void main()
{
	struct record *head;

	head	=	creat();
	printf("Next step is :\n");
	show(head,0);

	printf("Up step is   :\n");
	head	=	changehead(head);
	show(head,1);
}

struct record *creat()
{
	int i;
	char *name[n] = {"Peter1","Peter2","Peter3","Peter4","Peter5"};
	struct record *New,*head,*ptr;

	head = (struct record *)malloc(sizeof(struct record *));	
	ptr	 = head;

	head ->back		= NULL;	
	strcpy(head ->name , name[0]);
	
	for (i=1;i<=n-1;i++)
	{
		New = (struct record *)malloc(sizeof(struct record *));	
		New ->back		= ptr;
		New ->next		= NULL;
		strcpy(New ->name , name[i]);
		
		ptr ->next		= New;
		ptr				= New;
	}	
	return (head);
}

void   show(struct record *ptr,int dir)
{
	do
	{
		switch (dir)
		{
		case 0:
			printf("%s->",ptr->name);
			ptr = ptr ->next ;
			break;
		case 1:
			printf("%s->",ptr->name);
			ptr = ptr ->back ;
			break;
		}
	}while(ptr!=NULL);
	printf("NULL\n");
}

struct record *changehead(struct record *head)
{
	while(head->next != NULL)
	{
		head = head->next;
	}
	return head;
}
