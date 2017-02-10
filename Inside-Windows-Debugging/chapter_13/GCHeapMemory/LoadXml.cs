using System;
using System.Text;
using System.Xml;

public class Test
{
    public static void Main(
        string[] args
        )
    {
        XmlDocument document;
        int ticks;

        if (args.Length != 1)
        {
            Console.WriteLine("USAGE: loadxml.exe InputFile");
            return;
        }

        ticks = Environment.TickCount;

        document = new XmlDocument();
        document.Load(args[0]);

        Console.WriteLine("XML document load took {0} milliseconds", (Environment.TickCount - ticks));
        Console.WriteLine("Enter any key to continue...");
        Console.ReadLine();

        Console.WriteLine("Name of the first element in the input document is: {0}",
            document.DocumentElement.ChildNodes[0].Name);
    }
}
