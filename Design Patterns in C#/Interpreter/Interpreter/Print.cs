using System;
using System.Windows.Forms;
using System.Collections ;
namespace Interpreter
{
	/// <summary>
	/// Summary description for Print.
	/// </summary>
	public class Print :Verb
	{
		private Kid[] kids;
		private string pline;
		public Print(string s, Data kd, ListBox lb) : base(s, kd, lb) 	{
			value = PRINT;
		}
		
		//-----
		public override void Execute(){
			kids = data.getData ();
			ptable.Items.Clear ();
			for(int i=0; i< kids.Length ; i++) {
				pline = "";
				for(int j=0; j< args.Count ; j++) {
					ParseVar v = (ParseVar)args[j];
					if(v.getType () == ParseObject.MULTVAR)
						processM((MultVar)v, i);
					else
						process(v, i);
					ptable.Items.Add(pline);
				}
			}
		}
		//------
		private void processM(MultVar v, int i) {
			ArrayList vlist = v.getVector ();
			for(int k=0; k< vlist.Count ; k++ ) {
				ParseVar pv = (ParseVar) vlist[k];
				process(pv, i);
			}
		}
		private void process(ParseVar v, int i) {
			   pline = pline + kids[i].getData(v.getValue()).ToString() + " ";
		}

	}
}
