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
using System.Runtime.InteropServices;

namespace PProtect
{
    /// <summary>
    /// Window2.xaml 的交互逻辑
    /// </summary>
    public partial class windowProMon : Window
    {
        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern unsafe Boolean fnGetProMonInfo(byte[] info);

        [DllImport("PPspt.dll", CharSet = CharSet.Unicode)]
        public static extern Boolean fnHandleProMonChoose(Boolean AllowRun, Boolean TipChecked);


        public windowProMon()
        {
            InitializeComponent();

            InitWindow();
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);

            this.DragMove();
        }

        private void InitWindow()
        {
            Boolean bRet = false;

            byte[] detailInfo = new byte[500];

            try
            {
                bRet = fnGetProMonInfo(detailInfo);
                if (false == bRet)
                {
                    return;
                }

                Array.Resize(ref detailInfo, GetByteArrayValidCount(detailInfo));

                System.Text.UnicodeEncoding convert1 = new UnicodeEncoding();

                textBlockDetail.Text = convert1.GetString(detailInfo);
            }
            catch
            {
               // MessageBox.Show("exception");
            }


        }


        private void btnAllow_Click(object sender, RoutedEventArgs e)
        {
            Boolean bTipChecked = false;

            bTipChecked = (Boolean)checkBoxTip.IsChecked;

            fnHandleProMonChoose(true, bTipChecked);

            this.Close();
        }

        private void btnDeny_Click(object sender, RoutedEventArgs e)
        {
            Boolean bTipChecked = false;

            bTipChecked = (Boolean)checkBoxTip.IsChecked;

            fnHandleProMonChoose(false, bTipChecked);

            this.Close();
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
