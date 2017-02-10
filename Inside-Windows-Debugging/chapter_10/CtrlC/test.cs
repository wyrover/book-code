using System;
using System.Threading;

public class Test
{
    public static void Main()
    {
        Console.CancelKeyPress += delegate
        {
            Console.WriteLine("Stopping...");
        };
        Console.WriteLine("Starting...");
        Thread.Sleep(300000);
    }
}
