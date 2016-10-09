using System;
using System.Collections ;
using System.Windows.Forms;
namespace Interpreter
{
	/// <summary>
	/// Summary description for Sort.
	/// </summary>
	public class Sort : Verb
	{
		private Kid[] kids;
		public Sort(string s, Data kd, ListBox pt) : base(s, kd, pt) {
			value = SORT;
		}
		
		//-----
		public override void Execute() {
			kids = data.getData();
			for(int a = 0; a < args.Count ; a++) {
				ParseVar v = (ParseVar) args[a];
				process(v);
			}
		}
		//-----
		private void process(MultVar v) {
			ArrayList mvec =  v.getVector ();
			for(int k = mvec.Count -1; k >= 0; k--) {
				ParseVar pv = (ParseVar)mvec[k];
				int sortKey = pv.getValue ();
				sortByKey(sortKey);
			}
		}
		//-----
		private void process(ParseVar v) {
			int sortKey = v.getValue ();
			sortByKey(sortKey);
		}
		//-----
		private void sortByKey(int key) {
			for(int i=0; i< kids.Length ; i++ ) {
				for(int j=i; j < kids.Length ; j++) {
					if(kids[i].compare (kids[j], key)) {
						Kid tmp = kids[i];
						kids[i] = kids[j];
						kids[j] = tmp;
					}
				}
			}
		}
	}
}
