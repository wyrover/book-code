#include <stdio.h>
void showdata(int [5][6]);
void showNew(int  [10][3]);
void process(int  [5][6],int [10][3]);

void main()
{
	int data[5][6]	={ 0,0,7,0,0,
			   0,0,0,0,0,
			   0,0,0,0,0,
			   0,0,0,3,0,
			   0,0,0,0,0,
			   0,0,0,0,4};
	int Newdata[10][3];
	showdata(data);
	process(data,Newdata);
	showNew(Newdata);
}

void showdata(int data[5][6])
{
	int i,j;
	printf("data is \n");
	for (i=0;i<5;i++)
	{
		for(j=0;j<6;j++)
		{
			printf(" %d ",data[i][j]);
		}
		printf("\n");
	}
}
void process(int data[5][6],int Newdata[10][3])
{
	int i,j,index =0;
	
	Newdata[0][0] = 5;
	Newdata[0][1] = 6;

	for (i=0;i<5;i++)
	{
		for(j=0;j<6;j++)
		{
			if (data[i][j] != 0)
			{
				index++;
				Newdata[index][0] = i;
				Newdata[index][1] = j;
				Newdata[index][2] = data[i][j];
			}
		}
	}
	Newdata[0][2] = index;

}
void showNew(int Newdata[10][3])
{
	int i,j;
	printf("Newdata is \n");
	for (i=0;i<=Newdata[0][2];i++)
	{
		for(j=0;j<=2;j++)
		{
			printf(" %d ",Newdata[i][j]);
		}
		printf("\n");
	}
}

