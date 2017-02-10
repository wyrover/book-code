using System;
using System.Runtime.InteropServices;
using System.ComponentModel;

public class Leak
{
    public static void Main()
    {
        //
        // Loop indefinitely to simulate a progressive handle leak
        //
        IntPtr userToken = IntPtr.Zero;
        for ( ; ; )
        {
            if (!LogonUserW(
                @"test_user_hl", @"localhost", @"$a1234%BC",
                LOGON32_LOGON_NETWORK_CLEARTEXT, LOGON32_PROVIDER_DEFAULT, 
                out userToken))
            {
                throw new Win32Exception(Marshal.GetLastWin32Error());
            }
            // BUG! missing call to CloseHandle(userToken).
        }
    }

    [DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    private static extern bool LogonUserW(
        string userName, string domain, string password,
        int logonType, int logonProvider,
        out IntPtr userToken
        );

    [DllImport("kernel32.dll")]
    private static extern bool CloseHandle(
        IntPtr handle
        );

    private const int LOGON32_PROVIDER_DEFAULT = 0;
    private const int LOGON32_LOGON_NETWORK_CLEARTEXT = 3;
}
