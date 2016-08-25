#include <stdio.h>

#define n 3

struct record { 
	int number;
	char name[40];
	struct record *next;
}; 


struct record *creat();
void show(struct record *,int);

void main()
{
	struct record *head;
	int count;
	head = creat();
	printf("How many time do you want to Run?:");
	scanf("%d",&count);
	show(head,count);
}

struct record *creat()
{
	char *Name[n] = {"Peter1","Peter2","Peter3"};
	struct record *New,*ptr,*head; 
	int i = 1;

	head	= (struct record *) malloc (sizeof(struct record));
	strcpy(head->name,Name[0]);
	head->number	= i;
	
	ptr = head;
	

	for (i=2;i<=n;i++)
	{
		New = (struct record *)malloc(sizeof(struct record));
		strcpy(New->name,Name[i-1]);
		New->number = i;		
		ptr->next = New;	
		ptr = New;
	}
	ptr ->next = head;
	return head;
}

void show(struct record *head,int count)
{
	int i;
	struct record *ptr = head;

	for (i=0;i<count;i++)
	{
		do{
			printf("%s->",ptr->name);
			ptr = ptr ->next;
		}while(ptr != head);		
	}
}
