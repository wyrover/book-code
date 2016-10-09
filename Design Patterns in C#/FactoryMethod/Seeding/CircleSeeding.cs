using System;
using System.Collections ;

namespace Seeding
{
	/// <summary>
	/// Summary description for CircleSeeding.
	/// </summary>
	public class CircleSeeding:StraightSeeding
	{
		public CircleSeeding(ArrayList sw, int lanes):base(sw,lanes) {
			seed();
		}
		//----------------------------
		protected override void seed() {
			int circle; 

			base.seed();        //do straight seed as default
			if (numHeats >= 2 ) {
				if (numHeats >= 3)
					circle = 3;
				else
					circle = 2; 
				int i = 0;
				for (int j = 0; j < numLanes; j++) {
					for (int k = 0; k < circle; k++) {
						swmrs[i].setLane(lanes[j]);
						swmrs[i++].setHeat(numHeats - k);
					}
				}
			}
		}

	}
}
