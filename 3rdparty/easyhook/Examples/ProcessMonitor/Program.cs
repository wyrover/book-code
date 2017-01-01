using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;
using EasyHook;

namespace ProcessMonitor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                Config.Register(
                    "A simple ProcessMonitor based on EasyHook!",
                    "ProcMonInject.dll",
                    "ProcessMonitor.exe");
            }
            catch (ApplicationException)
            {
                MessageBox.Show("This is an administrative task!", "Permission denied...", MessageBoxButtons.OK);

                System.Diagnostics.Process.GetCurrentProcess().Kill();
            }

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
