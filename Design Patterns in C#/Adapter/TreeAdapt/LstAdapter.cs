using System;

namespace TreeAdapt
{
	/// <summary>
	/// Summary description for LstAdapter.
	/// </summary>
	public interface LstAdapter 	{
		 void Add(Swimmer sw) ;
		 int SelectedIndex() ;
		 void Clear() ;
		 void clearSelection() ;
	}
}
