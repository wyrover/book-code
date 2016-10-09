using System;

namespace Visitor
{
	/// <summary>
	/// Summary description for Boss.
	/// </summary>
	public class Boss : Employee	{
		private int bonusDays;
		public Boss(string name, float salary, int vdays, int sdays):base(name, salary, vdays, sdays)
		{ }
		public void setBonusDays(int bdays) {
			bonusDays = bdays;
			  }
		public int getBonusDays() {
			return bonusDays;
		}
		public override void accept(Visitor v ) {
			v.visit(this);
		}
	}
}
