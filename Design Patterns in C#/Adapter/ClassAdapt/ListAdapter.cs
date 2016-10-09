using System;
using System.Windows.Forms;

namespace ClassAdapt
{
	/// <summary>
	/// Summary description for ListAdapter.
	/// </summary>
	public interface ListAdapter {
		void Add(Swimmer sw) ;
		void Clear() ;
		void clearSelection() ;
	}		
}
