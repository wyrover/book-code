using System;
using System.Diagnostics;

public class Kill
{
    public static void Main(
        string[] args
        )
    {
        Process[] processes;

        if (args.Length != 1)
        {
            Console.WriteLine("USAGE: kill.exe [ProcessName]");
        }
        else
        {
            processes = Process.GetProcessesByName(args[0]);
            if (processes.Length == 0)
            {
                Console.WriteLine("No matching processes.");
            }
            foreach (Process process in processes)
            {
                Console.WriteLine("Killing process {0}", process.Id);
                process.Kill();
            }
        }
    }
}
