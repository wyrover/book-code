using System;

namespace Visitor
{
	/// <summary>
	/// Summary description for VacationVisitor.
	/// </summary>
	public class VacationVisitor : Visitor 	{
		private int totalDays;
		//-----
		public VacationVisitor() 	{
			totalDays = 0;
		}
		//-----
		public int getTotalDays() {
			return totalDays;
		}
		//-----
		public override void visit(Employee emp){
			totalDays += emp.getVacDays ();
		}
		//-----
		public override void visit(Boss bos){
			totalDays += bos.getVacDays ();
		}
	}
}
