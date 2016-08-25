#include <stdio.h>

struct record { 
	int number;
	char name[40];
	struct record *next;
}; 

struct record *creat();
void   process(struct record *);
void   show(struct record *);
void   insert(struct record *);
void   del(struct record *);
struct record *reverse(struct record *);
void   showMain();

void main()
{
	struct record *head;
	head = creat();
	process(head);
}

struct record *creat()
{
	char *Name[10] = {"Peter1","Peter2","Peter3","Peter4","Peter5","Peter6","Peter7","Peter8","Peter9","Peter10"};
	struct record *New,*ptr,*head; 
	int i = 1;

	head	= (struct record *) malloc (sizeof(struct record));
	strcpy(head->name,Name[0]);
	head->number	= i;
	
	ptr = head;
	

	for (i=2;i<=10;i++)
	{
		New = (struct record *)malloc(sizeof(struct record));
		strcpy(New->name,Name[i-1]);
		New->number = i;
		New->next	= NULL;
		ptr->next = New;	
		ptr = New;
	}
	return head;
}

void   process(struct record *head)
{
	do{
		showMain();
		switch (getche())
		{
		case '1':
			printf("\n");
			show(head);			
			break;
		case '2':
			printf("\n");
			insert(head);
			break;
		case '3':
			printf("\n");
			del(head);
			break;
		case '4':
			printf("\n");
			head = reverse(head);
			break;
		case '5':
			exit(0);
			break;
		default :
			printf("\n");
			printf("It is Wrong option!!!");
			printf("\n");
		}
	}while (1);
}

void show(struct record *ptr)
{
	while(ptr != NULL)
	{
		printf("Number %d is %s\n",ptr->number,ptr->name);		
		ptr = ptr->next ;
	}
}
void   showMain()
{
	printf("(1)show\n");
	printf("(2)insert\n");
	printf("(3)Del\n");
	printf("(4)reverse\n");
	printf("(5)quit\n");
	printf("Pleace input your option:");
}

void   insert(struct record *head)
{
	int node,flag = 1;	
	char name[10];
	struct record *ptr,*ptrNext;
	struct record *New = (struct record *) malloc (sizeof(struct record));
	
	printf("what node is you want to insert:");
	scanf("%d",&node);	

	ptr		= head;

	while (ptr->number != node)
	{
		ptr = ptr ->next ;	
		if (ptr == NULL)
		{
			printf("The node is wrong!!!\n");
			flag = 0;
			break;
		}
	}
	if (flag)
	{
		printf("what is Number:");
		scanf("%d",&(New->number));
		printf("what is Name:");
		scanf("%s",&name);
			
		strcpy(New->name,name);

		ptrNext = ptr ->next ;
		
		ptr ->next = New;
		New ->next = ptrNext;
	}
}

void   del(struct record *head)
{
	int node,flag = 1;
	struct record *ptr,*ptrBack;
		
	printf("what node is you want to Del:");
	scanf("%d",&node);	

	ptr		= head;

	while (ptr->number != node)
	{
		ptrBack = ptr;
		ptr		= ptr ->next ;	
		if (ptr == NULL)
		{
			printf("The node is wrong!!!\n");
			flag = 0;
			break;
		}
	}
	if (ptr == head)
	{
		printf("You can't del head,but you can try\n");
	}
	else if (flag)
	{
		ptrBack ->next = ptr ->next ;
		free(ptr);
	}
}

struct record *reverse(struct record *head)
{
	struct record *ptrBack,*ptr,*ptrNext;

	ptrBack = head;	
	ptr		= ptrBack ->next;
	ptrNext = ptr ->next;

	do
	{		
		ptr ->next = ptrBack;
		ptrBack	   = ptr;
		ptr		   = ptrNext;		
		ptrNext    = ptrNext ->next;
	}while ( ptrNext != NULL);

	ptr ->next = ptrBack;
	head->next = NULL;	
	return (ptr);
}


