using System;
using System.Collections ;
namespace BasicBridge
{
	/// <summary>
	/// Summary description for ListBridge.
	/// </summary>
	public class ListBridge : Bridger 	{
		protected VisList vis;
		//------
		public ListBridge(VisList v)		{
			vis = v;
		}
		//-----
		public virtual void addData(ArrayList ar) {
			for(int i=0; i< ar.Count ; i++) {
				Product p = (Product)ar[i];
				vis.addLine (p);
			}
		}
	}
}
