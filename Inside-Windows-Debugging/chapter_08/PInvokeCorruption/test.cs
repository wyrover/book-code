using System;
using System.Runtime.InteropServices;
using System.Text;

public class Test
{
    public static void Main()
    {
        Console.WriteLine("Starting...");
        byte[] buffer = new byte[8];
        IntPtr hwnd = FindWindow("notepad", null);
        if (hwnd != IntPtr.Zero)
        {
            // BUG! the nMaxCount parameter passed in exceeds the destination buffer’s size
            GetWindowText(hwnd, buffer, 256);
            Console.WriteLine(System.Text.UnicodeEncoding.Unicode.GetString(buffer));
            Console.WriteLine("Success");
        }
        else
        {
            Console.WriteLine("Failed to acquire notepad window handle");
        }
    }

    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    internal static extern int GetWindowText(
        IntPtr hWnd,
        [Out] byte[] lpString,
        int nMaxCount
        );

    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    internal static extern IntPtr FindWindow(
        string lpClassName,
        string lpWindowName
        );
}
