/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
struct  OPTIONS
{
WCHAR key;
WCHAR *message;
void(*handler)();
} ;

class AppInfo
{
public: 
    AppInfo(OPTIONS* options)
    {
            this->option = options;
    }

        void Loop()
        {
            wprintf(L"Press: \n");
           for (OPTIONS * current = option;;)
            {
                wprintf(L"  %C\t%s\n",current->key,current->message);
                if (current->handler == NULL) break;
                current++;
            }

    while(true)
    {
        wprintf(L"\n> ");
        WCHAR command = _getwch();

      OPTIONS * lastCommand = option;
         for (;lastCommand->handler!=NULL && lastCommand->key != command;lastCommand++)
         {
         }
         if (lastCommand->handler == NULL) break;
         (*lastCommand->handler)();

        }

    }

private:
        OPTIONS *option;
} ;
