 
#include <stdio.h>
#include <string.h>
#include <memory.h>

struct PhoneInfo
{
	int no;
	char name[30];
	char phone[30];
};

void main()
 {
	 FILE * fp = fopen("c:\\temp.dat", "wb");
	 PhoneInfo info[2];
	 info[0].no = 10;
	 strcpy(info[0].name, "h1");
	 strcpy(info[0].phone, "123");
	 info[1].no = 2;
	 strcpy(info[1].name, "h2");
	 strcpy(info[1].phone, "456");
	 for (int i = 0; i < 2; i++)
	 {
		 fwrite(&info[i], sizeof(PhoneInfo), 1, fp);
	 }	 
	 fclose(fp);

	 fp = fopen("c:\\temp.dat", "rb");
	 memset(info, 0, sizeof(info));
	 for (int i = 0; i < 2; i++)
	 {
		 fread(&info[i], sizeof(PhoneInfo), 1, fp);
		 printf("no: %d\n", info[i].no);
		 printf("name: %s\n", info[i].name);
		 printf("phone: %s\n", info[i].phone);
	 }	 
	 fclose(fp);
 }
