#include <stdio.h>

struct node
{
	int number;
	struct node *next;
};

void showmain();
struct node *push(struct node *);
struct node *pop(struct node *);

void main()
{
	struct node *top = NULL;
	do{
		showmain();
		switch (getche())
		{
		case '1':
			top = push(top);
			break;
		case '2':
			top = pop(top);
			break;
		case '3':
			exit(0);	
			
		default:
			printf("\nIt is wrong,pleace input again\n");
			break;
		}
	}while (1);
}

void showmain()
{
	printf("<1>push a value\n");
	printf("<2>pop a value\n");
	printf("<3>exit\n");
}

struct node *push(struct node *top)
{
	struct node *New;
	New = (struct node *)malloc(sizeof(struct node)); 
	printf("pleace input a number");
	scanf("%d",&(New->number));
	New->next = top;
	top = New;
	return (top);
}

struct node *pop(struct node *top)
{
	struct node *freenode;
	if (top != NULL)
	{
		printf("\nThe pop number is %d\n",top->number);
		freenode = top;
		top = top ->next;
		free (freenode);
		return (top);
	}
	else
	{
		printf("\nNo Node\n");
		return (NULL);
	}
}

