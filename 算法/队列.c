#include <stdio.h>

struct node
{
	int number;
	struct node *next;
};

void showmain();
struct node *add(struct node *);
struct node *out(struct node *);

void main()
{
	struct node *real = NULL,*front = NULL;
	do{
		showmain();
		switch (getche())
		{
		case '1':
			real  = add(real);
			if (front == NULL)
			{
				front = real;
			}
			break;
		case '2':
			front = out(front);
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
	printf("<1>add a value\n");
	printf("<2>output a value\n");
	printf("<3>exit\n");
}

struct node *add(struct node *real)
{
	struct node *New;
	New = (struct node *)malloc(sizeof(struct node)); 
	printf("pleace input a number");
	scanf("%d",&(New->number));
	New->next = NULL;
	if (real != NULL)
	{
		real ->next = New;
	}
	real = New;
	return (real);
}
struct node *out(struct node *front)
{
	struct node *freenode;
	if (front != NULL)
	{
		printf("\nThe pop number is %d\n",front->number);
		freenode = front;
		front = front ->next;
		free(freenode);
		return (front);
	}
	else
	{
		printf("\nNo Node\n");
		return (NULL);
	}

}

