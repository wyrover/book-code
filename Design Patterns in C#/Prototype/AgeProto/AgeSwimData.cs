using System;
using System.Collections ;
namespace DeepSexClone
{
	/// <summary>
	/// Summary description for SexSwimData.
	/// </summary>
	public class AgeSwimData:SwimData
	{
		ArrayList swd;
		public AgeSwimData() {
			swdata = new ArrayList ();
		}
		//------
		public AgeSwimData(string filename):base(filename){}
		public AgeSwimData(ArrayList ssd):base(ssd){}
		//------
		public override void cloneMe(SwimData swdat) {
			swd = swdat.getData ();
		}
		//------
		public override void sort() {
			Swimmer[] sws = new Swimmer[swd.Count ];
			//copy in swimmer objects
			for(int i=0; i < swd.Count ; i++) {
				sws[i] = (Swimmer)swd[i];
			}
            //sort into increasing order
			for( int i=0; i< sws.Length ; i++) {
				for (int j = i; j< sws.Length ; j++) {
					if (sws[i].getAge ()>sws[j].getAge ()) {
						Swimmer sw = sws[i];
						sws[i]=sws[j];
						sws[j]=sw;
					}
				}
			}
			int age = sws[0].getAge ();
			int agecount = 0;
			 int k = 0;
			swdata = new ArrayList ();
			bool quit = false;
			
			while( k < sws.Length && ! quit ) {
				while(sws[k].getAge() ==age && ! quit) {
					agecount++;
					if(k < sws.Length -1)
						k++;
					else
						quit= true;
				}
				//create a new Swimmer with a series of X's for a name
				//for each new age
				string name = "";
				for(int j = 0; j < agecount; j++)
					name +="X";
				Swimmer sw = new Swimmer(age.ToString() + " " +
					name + "," + age.ToString() + ",club,0,F");
				swdata.Add (sw);
				agecount = 0;
				if(quit)
					age = 0;
				else
					age = sws[k].getAge ();
			}
		
		}
	}
}
