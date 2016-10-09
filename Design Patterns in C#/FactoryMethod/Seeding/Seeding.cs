using System;
using System.Collections ;
namespace Seeding
{
	/// <summary>
	/// Summary description for Seeding.
	/// </summary>
	public abstract class Seeding 	{
		protected int       numLanes;                      
		protected int[]     lanes;
		public abstract IEnumerator getSwimmers();
		public abstract int getCount();
		public abstract int getHeats();
		protected abstract void seed();
		//--------------------------------
		protected void calcLaneOrder() {
			lanes = new int[numLanes];
			int mid = numLanes / 2;
			if (odd(numLanes))
				mid = mid + 1;       //start in middle lane
			int incr = 1;
			int ln = mid;
			//create array of lanes from
			//center to outside
			for (int i=0; i< numLanes; i++) {
				lanes[i] = ln;
				ln = mid + incr;
				incr = - incr;
				if (incr > 0)
					incr=incr+1;
			}
		}
		//--------------------------------
		private bool odd(int x) {
			return(((x / 2)*2) != x);
		}
	}
}
