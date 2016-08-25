#include <stdio.h>

struct record
{
	int number;
	int name[40];
	struct record *next;
};

struct record *creat(char *[],int,int);
void   process(struct record *,struct record *);
void   show(struct record *);

void main()
{
	char *data1[4]	=	{"Peter1","Peter2","Peter3","Peter4"};
	char *data2[4]	=	{"Peter5","Peter6","Peter7","Peter8"};
	struct record *head1,*head2;
	head1	=	creat(data1,1,4);
	head2	=	creat(data2,5,8);
	printf("The head1 is: \n");
	show(head1);
	printf("The head2 is: \n");
	show(head2);
	process(head1,head2);
	printf("Affter Process,The head1 is: \n");
	show(head1);
}

struct record *creat(char *data[],int start,int end)
{
	struct record *New,*ptr,*head; 	
	int i,j=1;

	head	= (struct record *) malloc (sizeof(struct record));
	strcpy(head->name,data[0]);
	head->number	= start;	
	ptr = head;	

	for (i=start+1;i<=end;i++)
	{
		New = (struct record *)malloc(sizeof(struct record));
		strcpy(New->name,data[j++]);
		New->number = i;
		New->next	= NULL;
		ptr->next = New;	
		ptr = New;
	}
	return head;

}

void   process(struct record *ptr,struct record *head2)
{
	while(ptr ->next != NULL)
	{
		ptr = ptr->next;
	}
	ptr ->next = head2;	
}

void show(struct record *ptr)
{
	while(ptr != NULL)
	{
		printf("Number %d is %s\n",ptr->number,ptr->name);		
		ptr = ptr->next ;
	}

}

