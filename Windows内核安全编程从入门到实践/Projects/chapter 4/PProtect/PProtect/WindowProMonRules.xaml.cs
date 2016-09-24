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
using System.Windows.Shapes;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Collections.ObjectModel;


namespace PProtect
{
    /// <summary>
    /// WindowProMonRules.xaml 的交互逻辑
    /// </summary>
    public partial class WindowProMonRules : Window
    {
        [StructLayout(LayoutKind.Sequential)]
        public unsafe struct ProMonItem
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
            public byte[] processName;
            public UInt32 oper;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 42)]
            public byte[] sha1;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 400)]
            public byte[] processPath;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
            public byte[] lastTime;
        }

        [DllImport("PPspt.dll")]
        public static extern UInt32 fnGetRegProItemCount();

        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern unsafe Boolean fnGetRegProItem(UInt32 index, ref ProMonItem item);

        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern Boolean fnDelRelateKey(string processName);


        public WindowProMonRules()
        {
            InitializeComponent();

            InitWindow();
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);

            this.DragMove();
        }

        private unsafe void InitWindow()
        {
            GridView myGridView = new GridView();
            myProcessMonInfo myProcessMonInfo1 = new myProcessMonInfo();

            myGridView.AllowsColumnReorder = true;

            GridViewColumn gc2 = new GridViewColumn();
            gc2.Header = "进程名";
            gc2.DisplayMemberBinding = new Binding("ProcessName");
            gc2.Width = 150;
            myGridView.Columns.Add(gc2);

            GridViewColumn gc3 = new GridViewColumn();
            gc3.Header = "操作";
            gc3.DisplayMemberBinding = new Binding("Oper");
            gc3.Width = 80;
            myGridView.Columns.Add(gc3);

            GridViewColumn gc4 = new GridViewColumn();
            gc4.Header = "路径";
            gc4.DisplayMemberBinding = new Binding("ProcessPath");
            gc4.Width = 200;
            myGridView.Columns.Add(gc4);

            GridViewColumn gc5 = new GridViewColumn();
            gc5.Header = "SHA1 值";
            gc5.DisplayMemberBinding = new Binding("SHA1");
            gc5.Width = 100;
            myGridView.Columns.Add(gc5);

            GridViewColumn gc6 = new GridViewColumn();
            gc6.Header = "最近修改时间";
            gc6.DisplayMemberBinding = new Binding("LastTime");
            gc6.Width = 150;
            myGridView.Columns.Add(gc6);

            listViewProRules.View = myGridView;
            listViewProRules.ItemsSource = myProcessMonInfo1;


            UInt32 cbSubKeys = 0;
            UInt32 i = 0;

            cbSubKeys = fnGetRegProItemCount();
            if (cbSubKeys != 0)
            {

                ProMonItem proMonItem1 = new ProMonItem();

                System.Text.UnicodeEncoding convert1 = new UnicodeEncoding();

                for (i = 0; i < cbSubKeys; ++i)
                {
                    fnGetRegProItem(i, ref proMonItem1);

                    string oper = (proMonItem1.oper == 0) ? "拦截" : "放过";


                    Array.Resize(ref proMonItem1.processName, GetByteArrayValidCount(proMonItem1.processName));
                    Array.Resize(ref proMonItem1.processPath, GetByteArrayValidCount(proMonItem1.processPath));
                    Array.Resize(ref proMonItem1.lastTime, GetByteArrayValidCount(proMonItem1.lastTime));

                    myProcessMonInfo1.AddItem(
                        convert1.GetString(proMonItem1.processName),
                        oper,
                        convert1.GetString(proMonItem1.processPath),
                        convert1.GetString(proMonItem1.sha1),
                        convert1.GetString(proMonItem1.lastTime)
                    );

                    Array.Resize(ref proMonItem1.processName, 100);
                    Array.Resize(ref proMonItem1.processPath, 400);
                    Array.Resize(ref proMonItem1.lastTime, 40);


                }

            }


        }

        private void btn_del_Click(object sender, RoutedEventArgs e)
        {
            myProcessMonInfo myProcessMonInfo1 = listViewProRules.ItemsSource as myProcessMonInfo;

            ProcessMonInfo ProcessMonInfo1 = listViewProRules.SelectedItem as ProcessMonInfo;
            if (null == ProcessMonInfo1)
            {
                return;
            }

            fnDelRelateKey(ProcessMonInfo1.ProcessName);

            myProcessMonInfo1.RemItem(listViewProRules.SelectedIndex);
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

        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

    }

    public class ProcessMonInfo
    {
        private string _processName;
        private string _oper;
        private string _processPath;
        private string _sha1;
        private string _lastTime;

        public string ProcessName
        {
            get { return _processName; }
            set { _processName = value; }
        }

        public string Oper
        {
            get { return _oper; }
            set { _oper = value; }
        }

        public string ProcessPath
        {
            get { return _processPath; }
            set { _processPath = value; }
        }


        public string SHA1
        {
            get { return _sha1; }
            set { _sha1 = value; }
        }

        public string LastTime
        {
            get { return _lastTime; }
            set { _lastTime = value; }
        }

        public ProcessMonInfo(string processName, string oper, string processPath, string sha1, string lastTime)
        {
            _processName = processName;
            _oper = oper;
            _processPath = processPath;
            _sha1 = sha1;
            _lastTime = lastTime;
        }
    }

    public class myProcessMonInfo :
            ObservableCollection<ProcessMonInfo>
    {
        [DllImport("Kernel32.dll", CharSet = CharSet.Unicode)]
        public static extern void OutputDebugString(string lpOutputString);

        public void AddItem(string processName, string oper, string processPath, string sha1, string lastTime)
        {
            OutputDebugString(processName);

            Add(new ProcessMonInfo(processName, oper, processPath, sha1, lastTime));

        }

        public void RemItem(Int32 index)
        {
            RemoveItem(index);
        }
    }
}
