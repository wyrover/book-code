using System;
using System.Collections ;
namespace BasicBridge
{
	/// <summary>
	/// Summary description for SortBridge.
	/// </summary>
	public class SortBridge:ListBridge 	{
		//-----
		public SortBridge(VisList v):base(v){
		}
		//-----
		public override void addData(ArrayList ar) {
			int max = ar.Count ;
			Product[] prod = new Product[max];
			for(int i=0; i< max ; i++) {
				prod[i] = (Product)ar[i];
			}
			for(int i=0; i < max ; i++) {
				for (int j=i; j < max; j++) {
					if(prod[i].CompareTo (prod[j])>0)  {
						Product pt = prod[i];
						prod[i]= prod[j];
						prod[j] = pt;
					}
				}
			}
			for(int i = 0; i< max; i++) {
				vis.addLine (prod[i]);
			}
		}
	}
}
