using System;
using System.Collections ;
using CsharpPats;

namespace DeepSexClone
{
	/// <summary>
	/// Summary description for SwimData.
	/// </summary>
	public class SwimData:ICloneable 	{
		protected ArrayList swdata;
		private int index;
		//-----
		public SwimData() {
				swdata = new ArrayList ();
		}
		//-----
		public SwimData(ArrayList swd) {
			swdata = swd;
			index=0;
		}
		//-----
		public int count() {
			return swdata.Count ;
		}
		//-----
		public SwimData(string filename) 		{
			swdata = new ArrayList ();
			csFile fl = new csFile(filename);
			fl.OpenForRead ();
			string s = fl.readLine ();
			while(s != null) {
				Swimmer sw = new Swimmer(s);
				swdata.Add (sw);
				s = fl.readLine ();
			}
			fl.close ();
		}
		//-----
		public object Clone() {
			//create a new ArrayList
			ArrayList swd = new ArrayList ();
			//copy in swimmer objects
			for(int i=0; i< swdata.Count ; i++) 
				swd.Add (swdata[i]);
			//create new SwimData object with this array
			SwimData newsd = new SwimData (swd);
			return newsd;
		}
		//-----
		public void moveFirst() {
			index = 0;
		}
		//-----
		public bool hasMoreElements() {
			return (index < swdata.Count-1 );
		}
		//-----
		public void sort() {
		//sort using IComparable interface of Swimmer
			swdata.Sort (0, swdata.Count , null);
		}
		//-----
		public Swimmer getSwimmer() {
			if(index < swdata.Count )
				return (Swimmer)swdata[index++];
			else
				return null;
		}
	}
}
