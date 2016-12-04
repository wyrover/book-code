#include <stdio.h>
#include <string.h>

#define _FileError -2
#define _OpenRead "rb"
#define _OpenWrite "wb"
#define _OpenAppend "ab"
#define _OpenR_W "rb+"
#define _OpenC_R_W "wb+" //delete old file write/read
#define _OpenA_R_W "ab+"
#define _FromCurrent SEEK_CUR
#define _FromStart SEEK_SET
#define _FromEnd SEEK_END

class FileStream
{
public:
  FILE *fp;//place here is not good, but now place here.
  bool openResult;//place here is not good, but now place here.
  FileStream(char * fileName, char * openMode);
  ~FileStream();
  int read(void * buf, long size);
  int write(void * buf, long size);
  int locate(long offset, int from);
  bool eof();
};

FileStream::FileStream(char * fileName, char * openMode)
{
	fp = fopen(fileName, openMode); 
	openResult = fp != 0;
};

FileStream::~FileStream()
{
	if (fp != 0) 
	  fclose(fp);
};

int FileStream::read(void * buf, long size)
{
	if (fp == 0) return _FileError;
	return fread(buf, 1, size, fp);
};

int FileStream::write(void * buf, long size)
{
    if (fp == 0) return _FileError;
	return fwrite(buf, 1, size, fp) ;
};

int FileStream::locate(long offset, int from)
{
   if (fp == 0) return _FileError;
   return fseek(fp, offset, from);
};

bool FileStream::eof()
{
    if (fp == 0) return true;
	return feof(fp) != 0;
};

void main()
{
	char enter[2] = {(char)13, (char)10};// line feed in dos format text file
	FileStream * f;
	char buf[100];
	f = new FileStream("c:\\temp.txt", _OpenWrite);
	if (!f->openResult)
	{
		printf("open error\n");
		return;
	}
	f->write("hello", strlen("hello"));
	f->write(enter, 2);
    f->write("file stream", strlen("file stream"));
    f->write(enter, 2); 
	delete f;
	{
		FileStream fStream("c:\\temp.txt", _OpenRead);
		int count;
		if (!fStream.openResult)
		{
		  printf("open error\n");
		  return;
		};
        while (!fStream.eof())
		{
			count  = fStream.read(buf, 100);
			if ((count > 0) && (count < 100))
			{  
				buf[count] = 0;
			   printf("%s", buf);
            } 
		}
	}
	getchar();
};