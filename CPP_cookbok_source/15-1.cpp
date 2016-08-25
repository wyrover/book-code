// An example of a callback function
bool updateProgress(int pct)
{
    std::cout << pct << "% complete...\n";
    return (true);
}

// A typedef to make for easier reading
typedef bool (*FuncPtrBoolInt)(int);

// A function that runs for a while
void longOperation(FuncPtrBoolInt f)
{
    for (long l = 0; l < 100000000; l++)
        if (l % 10000000 == 0)
            f(l / 1000000);
}

int main()
{
    longOperation(updateProgress); // ok
}