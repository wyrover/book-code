//Func8-1.h 增加的I/O操作
void OutputToFile(ofstream &f, ID c)
{
    f << c.key << ' ' << c.order << endl;
}

