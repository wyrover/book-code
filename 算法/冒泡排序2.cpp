void BubbleSort(int Array[], const int Size);
void PrintArray(int Array[], const int Size);

int main(void)
{
	int i;
	const int Size = 20;
	int Array[Size];
	//Fill the Array with rendom values between 0 and 99
	for (i = 0; i < Size; i++)
	{
		Array[i] = random() % 100;
	}
	printf("The Array with random order:\n\n");
	PrintArray(Array, Size);
	// Wait for key Press...
	printf("\n Press and key...");
	//Sort the Array using Bubble Sort
	BubbleSort(Array, Size);
	printf("The Array after Bubble Sort:\n\n");
	PrintArray(Array, Size);
	printf("\nPress any key to quit..");
	return 0;
}

/* Uses the classic bubble sort algorithm */
void BubbleSort(int Array[], const int Size)
{
	int i, j, temp;
	for (i = 0; i < Size -1; i++)
	{
		for (j = 0; j < Size - i + 1; j++)
		{
			if (Array[j] > Array[j+1])
			{
				temp = Array[j];
				Array[j] = Array[j+1];
				Array[j+1] = temp;
			}
		}
	}
}

/* Prints an integer Array line by line */
void PrintArray(int Array[], const int Size)
{
	int i;
	
	for (i = 0; i < Size; i++)
	{
		printf("Array[%i] = %i\n", i, Array[i]);
	}
}