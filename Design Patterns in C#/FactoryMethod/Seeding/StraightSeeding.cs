using System;
using System.Collections ;
namespace Seeding
{
	/// <summary>
	/// Summary description for StraightSeeding.
	/// </summary>
	public class StraightSeeding :Seeding 	{
		protected ArrayList    swimmers;
		protected Swimmer[] swmrs;
		protected int       count;
		protected int       numHeats;

		public StraightSeeding(ArrayList sw, int lanes) {
			swimmers = sw;
			numLanes = lanes;
			count    = sw.Count;
			calcLaneOrder();    
			seed();
		}
		//--------------------------------
		protected override void seed() {
			//loads the swmrs array and sorts it
			sortUpwards();

			int lastHeat = count % numLanes;
			if (lastHeat < 3)
				lastHeat = 3;   //last heat must have 3 or more
			int lastLanes = count - lastHeat;
			numHeats = count / numLanes;
			if (lastLanes > 0)
				numHeats++;
			int heats = numHeats;

			//place heat and lane in each swimmer's object
			int j = 0;
			for (int i = 0; i < lastLanes; i++) {
				Swimmer sw = swmrs[i];
				sw.setLane(lanes[j++]);
				sw.setHeat(heats);
				if (j >= numLanes) {
					heats--;
					j=0;
				}
			}
			//Add in last partial heat
			if (j < numLanes)
				heats--;
			j = 0;
			for (int i = lastLanes-1; i<count; i++) {
				Swimmer sw = swmrs[i];
				sw.setLane(lanes[j++]);
				sw.setHeat(heats);     
			}
			//copy from array back into ArrayList
			swimmers = new ArrayList();
			for (int i=0; i< count; i++)
				swimmers.Add(swmrs[i]);
		}
		//--------------------------------
		protected void sortUpwards() {
			swmrs = new Swimmer[count];
			for (int i=0; i < count; i++)
				swmrs[i] = (Swimmer)swimmers[i];
			for ( int i=0; i < count; i++) {
				for (int j = i; j < count; j++) {
					if (swmrs[i].getTime() > swmrs[j].getTime()) {
						Swimmer swtemp = swmrs[i];
						swmrs[i] = swmrs[j];
						swmrs[j] = swtemp;
					}
				}
			}      
		}
		//--------------------------------
		public override int getCount() {
			return swimmers.Count ;
		}

		public override IEnumerator getSwimmers() {
			return swimmers.GetEnumerator ();
		}
		//----------------------------------
		public override int getHeats() {
			return numHeats;
		}

	}
}
