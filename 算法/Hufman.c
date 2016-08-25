#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

//struct node{

void char_clear( char *, int );

void int_clear( int *, int );
void ch_code( char *, int * );
int max_find( int * );
int sure( char, char * );

int main( void )
{
    char read;
    char ch_record[MAX];
    int in_record[MAX], count = 0;
    int order, tem, numofword = 0;
    FILE *fp;

    char_clear(ch_record, MAX);
    int_clear(in_record, MAX);

    if ((fp = fopen("huf.jea", "r")) == NULL)
    {
        puts("Can't find the file.\n");
        system("pause");
        return;
    }

    while (!feof(fp))
    {
        read = fgetc(fp);
        printf("%c", read);
        numofword++;

        if ((order = sure(read, ch_record)) == -1)
        {
            ch_record[count] = read;
            in_record[count] += 1;
            count++;
        }
        else
        {
            in_record[order] += 1;
        }
    }

    //================================================
    printf("Count = %d\tNumofword = %d\n", count, numofword);
    printf("字符\t字符码\t出现率\n");

    for (tem = 0; tem<count; tem++)
        printf("%c \t%d  \t%d\n", ch_record[tem], ch_record[tem], in_record[tem]);

    printf("占用空间为%d字节\n", numofword * 8);
    //================================================   
    fclose(fp);

    //************************************************
    ch_code(ch_record, in_record);
    system("pause");
    return 0;
}

void char_clear( char *str, int len )
{
    int i;

    for (i = 0; i<len; i++)
    {
        *(str + i) = '\0';
    }

    return;
}

void int_clear( int *str, int len )
{
    int i;

    for (i = 0; i<len; i++)
    {
        *(str + i) = 0;
    }

    return;
}

int sure( char read, char *record )
{
    int i = 0;

    while (record[i] != '\0')
    {
        if (read == record[i])
            return i;

        i++;
    }

    return -1;
}

int max_find( int *str )
{
    int i, max = 0, rec = -1;

    for (i = 0; i<MAX; i++)
    {
        if (*(str + i)>=0 && max<*(str + i))
        {
            max = *(str + i);
            rec = i;
        }
    }

    if (rec != -1)
        printf("%d\t", *(str + rec));

    return rec;
}

void ch_code( char *cre, int *ire )
{
    int order;
    int result[MAX];
    int len = 1;
    int i;
    int space;
    int_clear(result, MAX);

    while ((order = max_find(ire)) != -1)
    {
        result[order] = len * ire[order];
        printf("%d\t", result[order]);

        for (i = 0; i<len - 1; i++)
            printf("0");

        puts("1");
        len++;
        ire[order] = 0;
    }

    space = 0;

    for (i = 0; i<MAX; i++)
    {
        space += result[i];
    }

    printf("HUFMAN编码所占用的总空间为%d字节\n", space);
}
