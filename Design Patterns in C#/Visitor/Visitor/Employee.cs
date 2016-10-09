using System;

namespace Visitor
{
	/// <summary>
	/// Summary description for Employee.
	/// </summary>
	public class Employee 	{
		int sickDays, vacDays;
		float salary;
		string name;
		public Employee(string name, float salary, int vDays, int sDays) {
			this.name = name;
			this.salary = salary;
			sickDays = sDays;
			vacDays = vDays;
		}
		//-----
		public string getName() {
			return name;
		}
		public int getSickDays() {
			return sickDays;
		}
		public int getVacDays() {
			return vacDays;
		}
		public float getSalary() {
			return salary;
		}
		public virtual void accept(Visitor v) {
			v.visit(this);
		}
	}
}
