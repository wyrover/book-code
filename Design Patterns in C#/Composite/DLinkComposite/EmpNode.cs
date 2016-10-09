using System;
using System.Windows.Forms;
namespace Composite
{
	/// <summary>
	/// Summary description for EmpNode.
	/// </summary>
	public class EmpNode:TreeNode 	{
		private AbstractEmployee emp;
		public EmpNode(AbstractEmployee aemp ):base(aemp.getName ()) {
			emp = aemp;	
		}
		//-----
		public AbstractEmployee getEmployee() {
			return emp;
		}
	}
}
