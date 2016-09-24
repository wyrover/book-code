using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;

namespace PProtect
{
    /// <summary>
    /// Window1.xaml 的交互逻辑
    /// </summary>
    /// 
    public partial class Window1 : Window
    {
        private System.Windows.Forms.LinkLabel linkProMonRules;

        private System.Windows.Forms.NotifyIcon notifyIcon1;
        private System.Windows.Forms.ContextMenu contextMenu1;
        private System.Windows.Forms.MenuItem menuItem1;
        private System.ComponentModel.IContainer components;

        System.Drawing.Icon icon1;

        private Boolean successInit;
        private int flagControl;
        private Int32 shareMemory;
        private static Boolean startChoose;

        [DllImport("PPspt.dll")]
        public static extern int fnPPspt();
        [DllImport("PPspt.dll")]
        public static extern unsafe Boolean fnInitApp(Int32* pShareMemory);
        [DllImport("PPspt.dll")]
        public static extern unsafe Boolean fnInitReg(int* flag);
        [DllImport("PPspt.dll")]
        public static extern Boolean fnUpdateFlag(int bit, Boolean bDirect);
        [DllImport("PPspt.dll")]
        public static extern Boolean fnHandlePPsptWhitePaper();
        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern Boolean fnHandleProMonChoose(Boolean AllowRun, Boolean TipChecked);
        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern Boolean fnStartChoose();
        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern Boolean fnSigVerif();
        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern int fnGetProcessInRulesState();
        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern unsafe Boolean fnGetlastWriteTime(byte[] lastTime);
        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern unsafe Boolean fnGetIcoPath(byte[] icoPath);
        [DllImport("Kernel32.dll")]
        public static extern uint WaitForSingleObject(int hHandle, uint dwMilliseconds);
        [DllImport("Kernel32.dll")]
        public static extern int OpenEvent(int dwDesiredAccess, Boolean bInheritHandle, string lpName);
        [DllImport("Kernel32.dll")]
        public static extern Boolean ResetEvent(int hEvent);


        public Window1()
        {
            Boolean bRet = false;

            startChoose = false;

            do
            {

                InitializeComponent();

                bRet = InitWindow();
                if (false == bRet)
                {
                    successInit = false;

                    break;
                }

                bRet = InitPPspt();
                if (false == bRet)
                {
                    successInit = false;

                    break;
                }

                successInit = true;


            } while (false);

            if (false == successInit)
            {
                MessageBox.Show("启动失败！是否忘记使用管理员权限执行程序了？");
            }

        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);

            this.DragMove();
        }

        private unsafe Boolean InitWindow()
        {
            Boolean bRet = true;
            linkProMonRules = new System.Windows.Forms.LinkLabel();

            BitmapImage bi = new BitmapImage();
            String stringIco = "/res/ProMonOpen.ico";

            BitmapImage biBtn = new BitmapImage();
            String stringIcoBtn = "/res/SystemOpen.ico";
            String stringBtn = "关闭";
            double Radius = 0;

            System.Windows.Media.Effects.BlurBitmapEffect myBlurBitmapEffect =
                new System.Windows.Media.Effects.BlurBitmapEffect();

            this.Closed += new EventHandler(Window1_Closed);
            this.MouseEnter += new MouseEventHandler(OnMouseEnterHandler);

            //
            //
            //textBlock&btn
            fixed (Int32* iflag = &flagControl)
            {
                bRet = fnInitReg(iflag);
                if (false == bRet)
                {
                    return false;
                }
            }

            if ((flagControl & (0x01 << 0)) != 0)   //fPProtect : 0x01 << 0
            {
                textProMonState.Text = "打开";
            }
            else
            {
                textProMonState.Text = "关闭";
                stringIco = "/res/ProMonClose.ico";
            }


            bi.BeginInit();
            bi.UriSource = new Uri(stringIco, UriKind.RelativeOrAbsolute);
            bi.EndInit();
            imageProMon.Source = bi;

            Byte[] lastTime = new byte[40];

            bRet = fnGetlastWriteTime(lastTime);
            if (false == bRet)
            {
                textLastWriteTime.Text = "";
            }
            else
            {
                Array.Resize(ref lastTime, GetByteArrayValidCount(lastTime));

                System.Text.UnicodeEncoding convert1 = new UnicodeEncoding();

                textLastWriteTime.Text = convert1.GetString(lastTime);
            }

            if ((flagControl & (0x01 << 0)) != 0)
            {
                stringIcoBtn = "/res/SystemOpen.ico";
                stringBtn = "关闭";
                Radius = 0;
            }
            else
            {
                stringIcoBtn = "/res/SystemClose.ico";
                stringBtn = "打开";
                Radius = 1;
            }

            biBtn.BeginInit();
            biBtn.UriSource = new Uri(stringIcoBtn, UriKind.RelativeOrAbsolute);
            biBtn.EndInit();
            imageSysSwitch.Source = biBtn;

            buttonSysSwitch.Content = stringBtn;

            myBlurBitmapEffect.Radius = Radius;
            myBlurBitmapEffect.KernelType = System.Windows.Media.Effects.KernelType.Box;

            this.BitmapEffect = myBlurBitmapEffect;

            //
            //
            //LinkLabel
            linkProMonRules.LinkColor = System.Drawing.Color.SlateGray;

            linkProMonRules.Text = "查看监控规则库";

            linkProMonRules.LinkClicked +=
                new System.Windows.Forms.LinkLabelLinkClickedEventHandler(link_LinkClicked);


            hostProMonRecord.Child = linkProMonRules;

            //
            //
            //NotifyIcon
            try
            {
                this.components = new System.ComponentModel.Container();
                this.contextMenu1 = new System.Windows.Forms.ContextMenu();
                this.menuItem1 = new System.Windows.Forms.MenuItem();


                this.contextMenu1.MenuItems.AddRange(
                            new System.Windows.Forms.MenuItem[] { this.menuItem1 });

                this.menuItem1.Index = 0;
                this.menuItem1.Text = "退出";
                this.menuItem1.Click += new System.EventHandler(this.menuItemExit_Click);


                this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);

                byte[] icoPath = new Byte[260];
                fnGetIcoPath(icoPath);

                Array.Resize(ref icoPath, GetByteArrayValidCount(icoPath));

                System.Text.UnicodeEncoding convert1 = new UnicodeEncoding();

                icon1 = new System.Drawing.Icon(convert1.GetString(icoPath));

                notifyIcon1.Icon = icon1;

                notifyIcon1.ContextMenu = this.contextMenu1;

                notifyIcon1.Text = "PProtect for Win7";
                notifyIcon1.Visible = true;


                notifyIcon1.DoubleClick += new System.EventHandler(this.notifyIcon1_DoubleClick);
            }
            catch
            {
                return false;

            }


            return true;
        }

        private unsafe Boolean InitPPspt()
        {
            Boolean bRet = false;


            fixed (Int32* iShareMemory = &shareMemory)
            {
                bRet = fnInitApp(iShareMemory);
                if (false == bRet)
                {
                    return false;
                }
            }


            Thread threadProMon = new Thread(new ThreadStart(threadProMonProc));

            threadProMon.Start();

            return true;
        }

        void Window1_Closed(object sender, EventArgs e)
        {
            fnUpdateFlag(0x01 << 1, false);              //fAppRun: 0x01 << 1

            try
            {
                components.Dispose();

                Process curProcess = Process.GetCurrentProcess();

                curProcess.Kill();
            }
            catch (System.Exception ep)
            {
                return;
            }
        }

        private void Minimum_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();

            this.ShowInTaskbar = false;
        }

        unsafe void OnMouseEnterHandler(object sender, MouseEventArgs e)
        {
            Boolean bRet = false;
            BitmapImage bi = new BitmapImage();
            String stringIco = "/res/ProMonOpen.ico";

            fixed (Int32* iflag = &flagControl)
            {
                bRet = fnInitReg(iflag);
                if (false == bRet)
                {
                    return;
                }
            }

            if ((flagControl & (0x01 << 0)) != 0)
            {
                textProMonState.Text = "打开";
            }
            else
            {
                textProMonState.Text = "关闭";
                stringIco = "/res/ProMonClose.ico";
            }


            bi.BeginInit();
            bi.UriSource = new Uri(stringIco, UriKind.RelativeOrAbsolute);
            bi.EndInit();
            imageProMon.Source = bi;


            Byte[] lastTime = new byte[40];

            bRet = fnGetlastWriteTime(lastTime);
            if (false == bRet)
            {
                textLastWriteTime.Text = "";
            }
            else
            {
                System.Text.UnicodeEncoding convert1 = new UnicodeEncoding();

                textLastWriteTime.Text = convert1.GetString(lastTime);
            }

        }

        void link_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
        {
            System.Windows.Forms.LinkLabel tempLink = sender as System.Windows.Forms.LinkLabel;

            if (tempLink == linkProMonRules)
            {
                WindowProMonRules windowProRules = new WindowProMonRules();

                windowProRules.Owner = this;
                windowProRules.ShowDialog();
            }

        }

        private void menuItemExit_Click(object sender, EventArgs e)
        {
            fnUpdateFlag(0x01 << 1, false);

            try
            {
                components.Dispose();

                Process curProcess = Process.GetCurrentProcess();

                curProcess.Kill();
            }
            catch (System.Exception ep)
            {
                return;
            }


        }

        private void notifyIcon1_DoubleClick(object sender, EventArgs e)
        {
            this.Show();

            this.ShowInTaskbar = true;
        }

        private static void threadProMonProc()
        {
            Boolean bRet = false;
            Int32 iRet = 0;

            int eventProMonA = OpenEvent(0x1F0003, false, "Global\\EventProMon");
            if (0 == eventProMonA)
            {
                return;
            }


            while (true)
            {
                WaitForSingleObject(eventProMonA, 0xffffffff);
                ResetEvent(eventProMonA);

                do
                {
                    if (false == startChoose)
                    {
                        bRet = fnStartChoose();
                        if (false == bRet)
                        {
                            fnHandleProMonChoose(true, false);

                            break;
                        }

                        startChoose = true;
                    }

                    bRet = fnSigVerif();
                    if (true == bRet)
                    {
                        fnHandleProMonChoose(true, false);

                        break;
                    }

                    iRet = fnGetProcessInRulesState();
                    if (iRet == 0)
                    {
                        fnHandleProMonChoose(false, false);
                    }
                    else if (iRet == 1)
                    {
                        fnHandleProMonChoose(true, false);
                    }
                    else
                    {
                        Thread threadProcMonShow = new Thread(new ThreadStart(threadProMonShowProc));
                        threadProcMonShow.SetApartmentState(ApartmentState.STA);
                        threadProcMonShow.Start();
                    }

                } while (false);

            }
        }

        private static void threadProMonShowProc()
        {

            Window windowProMon1 = new windowProMon();

            windowProMon1.ShowDialog();

        }

        private void buttonSysSwitch_Click(object sender, RoutedEventArgs e)
        {
            BitmapImage bi = new BitmapImage();
            String stringIco = "/res/SystemOpen.ico";
            String stringBtn = "关闭";
            Boolean bPProtect = true;

            System.Windows.Media.Effects.BlurBitmapEffect myBlurBitmapEffect =
                new System.Windows.Media.Effects.BlurBitmapEffect();

            double Radius = 0;

            if (buttonSysSwitch.Content.ToString() == "关闭")
            {
                stringIco = "/res/SystemClose.ico";
                stringBtn = "打开";
                bPProtect = false;

                Radius = 1;
            }

            fnUpdateFlag(0x01 << 0, bPProtect);

            bi.BeginInit();
            bi.UriSource = new Uri(stringIco, UriKind.RelativeOrAbsolute);
            bi.EndInit();
            imageSysSwitch.Source = bi;

            buttonSysSwitch.Content = stringBtn;

            myBlurBitmapEffect.Radius = Radius;
            myBlurBitmapEffect.KernelType = System.Windows.Media.Effects.KernelType.Box;

            this.BitmapEffect = myBlurBitmapEffect;

        }

        private Int32 GetByteArrayValidCount(byte[] byteArray)
        {

            for (Int32 i = 0; i < byteArray.Length; i++)
            {
                if (
                    (i > 0) &&
                    (byteArray[i] == 0) &&
                    (byteArray[i - 1] == 0)
                    )
                {
                    return i;
                }
            }

            return 0;
        }

    }
}
