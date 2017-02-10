//
// SystemCall.cs
//
using System;
using System.IO;

public class Test
{
    public static void Main()
    {
        Console.ReadLine();
        File.WriteAllText(@"c:\temp\abc.txt", "abc");
        Console.WriteLine("Success.");
    }
}
