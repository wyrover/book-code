using System;

namespace Visitor
{
	/// <summary>
	/// Summary description for Manager.
	/// </summary>
	public class Manager : Employee
	{
		private int bonusDays;
		public Manager(string nm, float slry, int vDays, int sDays) :
			base(nm, slry, vDays, sDays)
		{
		}
		public void setBonusDays(int bd) {
			bonusDays = bd;
		}
		public int getBonusDays() {
			return bonusDays;
		}
		public override void accept(Visitor v) {
			v.visit (this);
		}
	}
}
