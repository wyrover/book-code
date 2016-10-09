using System;
using System.Collections ;
namespace Composite
{
	/// <summary>
	/// Summary description for Boss.
	/// </summary>
	public class Boss:Employee
	{
		public Boss(string name, float salary):base(name,salary) 	{
		}
		//------
		public Boss(AbstractEmployee emp):base(emp.getName() , emp.getSalary()) 	{
		}
		//------
		public override void add(string nm, float salary) {
			AbstractEmployee emp = new Employee(nm,salary);
			subordinates.Add (emp);
		}
		//------
		public override void add(AbstractEmployee emp){
			subordinates.Add(emp);
		}
		//------
		public override AbstractEmployee getChild() {
			bool found;
			AbstractEmployee tEmp = null;
			IEnumerator esub ;
        
        if (getName().Equals (getName())) 
            return this;
        else {
            found = false;
            esub = subordinates.GetEnumerator ();
            while (! found && esub.MoveNext()) {
				tEmp = (AbstractEmployee)esub.Current; 
				found = (tEmp.getName().Equals(name));
                if (! found) {
                    if (! tEmp.isLeaf()) {
                        tEmp = tEmp.getChild();
						found = (tEmp.getName().Equals(name));
					 }      
				}                             
			}
			if (found) 
			   return tEmp;
			  else                
			   return new Employee("New person", 0);
            }
		}
	}
}
