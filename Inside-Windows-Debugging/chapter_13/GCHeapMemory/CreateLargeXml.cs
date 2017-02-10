using System;
using System.Text;
using System.Xml;

public class Test
{
    public static void Main(
        string[] args
        )
    {
        int n, numberOfElements;

        if (args.Length != 2)
        {
            Console.WriteLine("USAGE: createlargexml.exe OutputFile NumOfElements");
            return;
        }

        numberOfElements = Convert.ToInt32(args[1]);
        using (var writer = new XmlTextWriter(args[0], Encoding.UTF8))
        {
            writer.WriteStartDocument(true);
            writer.WriteStartElement("RootElement");
            for (n = 0; n < numberOfElements; n++)
            {
                writer.WriteElementString("name" + n, "value" + n);
            }
            writer.WriteEndElement();
            writer.WriteEndDocument();
        }
    }
}
