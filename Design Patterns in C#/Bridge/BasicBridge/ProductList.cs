using System;
using System.Windows.Forms;
namespace BasicBridge
{
	/// <summary>
	/// Summary description for ProductList.
	/// </summary>
	//A VisList class for the ListBox
	public class ProductList : VisList 	{
		private ListBox list;
		//-----
		public ProductList(ListBox lst) 		{
			list = lst;
		}
		//-----
		public void addLine(Product p) {
			list.Items.Add (p.getName() );
		}
		//-----
		public void removeLine(int num) {
			if(num >=0 && num < list.Items.Count ){
				list.Items.Remove (num);
			}
		}
	}
}
