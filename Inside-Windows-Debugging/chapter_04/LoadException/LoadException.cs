using System;
using System.IO;
using System.Reflection;

public class Test
{
    public static void Main()
    {
        try
        {
            Assembly.Load("MissingAssembly");
        }
        catch (Exception ex)
        {
            if (ex is FileNotFoundException || ex is ArgumentException)
            {
                throw new ApplicationException("A generic fatal error", ex);
            }
            else
            {
                throw;
            }
        }
    }
}
