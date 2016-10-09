using System;
using System.Collections ;
namespace BasicBridge
{
	/// <summary>
	/// Summary description for ListBridge.
	/// </summary>
	public class ListBridge : Bridger 	{
		private VisList vis;
		//------
		public ListBridge(VisList v)		{
			vis = v;
		}
		//-----
		public void addData(ArrayList ar) {
			for(int i=0; i< ar.Count ; i++) {
				Product p = (Product)ar[i];
				vis.addLine (p);
			}
		}
	}
}
