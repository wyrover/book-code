using System;
using System.Collections ;
namespace DeepSexClone
{
	/// <summary>
	/// Summary description for SexSwimData.
	/// </summary>
	public class SexSwimData:SwimData
	{
		public SexSwimData(string filename):base(filename){}
		public SexSwimData(ArrayList ssd):base(ssd){}
		public void sort(bool isFemale) {
			ArrayList swd = new ArrayList();
			for (int i = 0; i < swdata.Count ; i++) {
				Swimmer sw =(Swimmer)swdata[i];
				if (isFemale == sw.isFemale() ) {
					swd.Add (sw);
				}
			}
			swdata = swd;
		}
		public object Clone() {
			//create a new ArrayList
			ArrayList swd = new ArrayList ();
			//copy in swimmer objects
			for(int i=0; i< swdata.Count ; i++) 
				swd.Add (swdata[i]);
			//create new SwimData object with this array
			SexSwimData newsd = new SexSwimData (swd);
			return newsd;
		}
	}
}
