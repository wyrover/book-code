using System;
using System.Collections ;
namespace Composite
{
	/// <summary>
	/// Summary description for Employee.
	/// </summary>
	public class Employee  	{
		protected float salary;
		protected string name;
		protected ArrayList subordinates;
		//------
		public Employee(string nm, float salry) 		{
			subordinates = new ArrayList();
			name = nm;
			salary = salry;
		}
		//------
		public float getSalary() {
			return salary;
		}
		//------
		public string getName() {
			return name;
		}
		//------
		public bool isLeaf() {
			return subordinates.Count == 0;
		}
		//------
		public void add(string nm, float salary) {
			Employee emp = new Employee(nm,salary);
			subordinates.Add (emp);
		}
		//------
		public virtual void add(Employee emp) {
			subordinates.Add (emp);
		}
		//------
		public IEnumerator getSubordinates() {
			return subordinates.GetEnumerator ();
		}
		public virtual Employee getChild() {
			return null;
		}
		//------		
		public float getSalaries() {
			float sum;
			Employee esub;
			//get the salaries of the boss and subordinates
			sum = getSalary();
			IEnumerator enumSub = subordinates.GetEnumerator() ;
			while (enumSub.MoveNext())  {
				esub = (Employee)enumSub.Current;
				sum += esub.getSalaries();
			}
			return sum;
		}
	}
}
