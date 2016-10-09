using System;
using System.Windows.Forms ;
using System.Data;
namespace GridAdapt
{
	/// <summary>
	/// Summary description for GridAdapter.
	/// </summary>
	public class GridAdapter:LstAdapter
	{
		private DataGrid grid;
		private DataTable dTable;
		public GridAdapter(DataGrid grd) 		{
			grid = grd;
			dTable = (DataTable)grid.DataSource;
		}
		
		public void Add(Swimmer sw) {
		DataRow row = dTable.NewRow();
        row["Frname"] = sw.getFrname();
        row[1] = sw.getLName();
        row[2] = sw.getAge();  //This one is an integer
        dTable.Rows.Add(row);
        dTable.AcceptChanges();

		}
		public int SelectedIndex() {
		return 0;
		}
		public void Clear() {}
		public void clearSelection() {}
	}
}
