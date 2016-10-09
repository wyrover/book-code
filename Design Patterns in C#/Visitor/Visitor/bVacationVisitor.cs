using System;

namespace Visitor
{
	/// <summary>
	/// Summary description for bVacationVisitor.
	/// </summary>
	public class bVacationVisitor :Visitor 	{
		private int totalDays;
		public bVacationVisitor() 	{
			totalDays = 0;
		}
		//-----
		public override void visit(Employee emp) {
			 totalDays += emp.getVacDays();
			try {
				Manager mgr = (Manager)emp;
				totalDays += mgr.getBonusDays();
			}
			catch(Exception ){}
        }
		//-----
		public override void visit(Boss bos) {
			totalDays += bos.getVacDays();
			totalDays += bos.getBonusDays();
		}
		//-----
		public int getTotalDays() {
			return totalDays;
		}
	}
}
