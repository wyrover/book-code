using System;
using System.Windows.Forms;
namespace Composite
{
	/// <summary>
	/// Summary description for EmpNode.
	/// </summary>
	public class EmpNode:TreeNode 	{
		private Employee emp;
		public EmpNode(Employee aemp ):base(aemp.getName ()) {
			emp = aemp;	
		}
		//-----
		public Employee getEmployee() {
			return emp;
		}
	}
}
