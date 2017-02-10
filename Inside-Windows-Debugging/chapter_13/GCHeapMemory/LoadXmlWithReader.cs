using System;
using System.Text;
using System.Xml;

public class Test
{
    public static void Main(
        string[] args
        )
    {
        XmlReader reader;
        int ticks;

        if (args.Length != 1)
        {
            Console.WriteLine("USAGE: loadxml.exe InputFile");
            return;
        }

        ticks = Environment.TickCount;

        using (reader = new XmlTextReader(args[0]))
        {
            // Read the XML definition
            reader.Read();
            // Read the root element
            reader.Read();
            // Read the first element
            reader.Read();

            Console.WriteLine("XML document load took {0} milliseconds", (Environment.TickCount - ticks));
            Console.WriteLine("Enter any key to continue...");
            Console.ReadLine();

            Console.WriteLine("Name of the first element in the input document is: {0}",
                reader.Name);
        }
    }
}
