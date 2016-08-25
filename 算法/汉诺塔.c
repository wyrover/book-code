#include <stdio.h>

void Hanoi(int,char,char,char);

void main()
{
	int number;
	printf("Pleace input the number of disks:");
	scanf("%d",&number);
	Hanoi(number,'x','y','z');
}

void Hanoi(int n,char x,char y,char z)
{
	if (n > 1)
	{
		
		Hanoi(n-1,x,z,y);	
		printf("Move disk %d form %c to %c.\n",n,x,y);		
		Hanoi(n-1,y,x,z);		
	}
	else
	{
		printf("Move disk %d form %c to %c.\n",n,x,y);
	}

}
