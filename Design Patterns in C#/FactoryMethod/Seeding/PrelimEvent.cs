using System;

namespace Seeding
{
	/// <summary>
	/// Summary description for PrelimEvent.
	/// </summary>
	public class PrelimEvent:Event
	{
		public PrelimEvent(string filename, int lanes):base(filename,lanes) {
					}
		//return circle seeding
		public override Seeding getSeeding() {
			return new CircleSeeding(swimmers, numLanes);
		}
		public override bool isPrelim() {
			return true;
		}
		public override bool isFinal() {
			return false;
		}
		public override bool isTimedFinal() {
			return false;
		}
	}
}
