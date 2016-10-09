using System;
using System.Collections ;
namespace Composite
{
	/// <summary>
	/// Summary description for Employee.
	/// </summary>
	public class Employee :AbstractEmployee 	{
		protected float salary;
		protected string name;
		protected AbstractEmployee parent;
		protected ArrayList subordinates;
		//------
		public Employee(AbstractEmployee parnt, string nm, float salry) 		{
			subordinates = new ArrayList();
			name = nm;
			salary = salry;
			parent = parnt;
		}
		//------
		public AbstractEmployee getBoss() {
			return parent;
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
		public virtual void add(string nm, float salary) {
			  throw new Exception("No subordinates in base employee class");
		}
		//------
		public virtual void add(AbstractEmployee emp) {
			throw new Exception("No subordinates in base employee class");
		}
		//------
		public IEnumerator getSubordinates() {
			return subordinates.GetEnumerator ();
		}
		public virtual AbstractEmployee getChild() {
			return null;
		}
		//------		
		public float getSalaries() {
			float sum;
			AbstractEmployee esub;
			//get the salaries of the boss and subordinates
			sum = getSalary();
			IEnumerator enumSub = subordinates.GetEnumerator() ;
			while (enumSub.MoveNext())  {
				esub = (AbstractEmployee)enumSub.Current;
				sum += esub.getSalaries();
			}
			return sum;
		}
	}
}
