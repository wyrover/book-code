typedef void (*FuncPtr)(char * msg);

char * gMsg;

void f2(FuncPtr pfunc)
{
  pfunc(gMsg);
}

void f1(FuncPtr pfunc, char * msg)
{
  gMsg = msg;
  f2(pfunc);  
}