using System;
using System.Collections ;
namespace DeepSexClone
{
	/// <summary>
	/// Summary description for SexSwimData.
	/// </summary>
	public class SexSwimData:SwimData
	{
		public SexSwimData() {
			
		}
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
		
	}
}
