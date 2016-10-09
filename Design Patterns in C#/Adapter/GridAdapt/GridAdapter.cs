using System;
using System.Windows.Forms ;
using System.Data;
namespace GridAdapt
{
	/// <summary>
	/// Summary description for GridAdapter.
	/// </summary>
	public class GridAdapter:LstAdapter 	{
		private DataGrid grid;
		private DataTable dTable;
		private int row;
		//-----
		public GridAdapter(DataGrid grd) 		{
			grid = grd;
			dTable = (DataTable)grid.DataSource;
			grid.MouseDown +=new System.Windows.Forms.MouseEventHandler (Grid_Click);
			row = -1;		
		}
		//-----
		public void Grid_Click(object sender, System.Windows.Forms.MouseEventArgs  e) {
			DataGrid.HitTestInfo hti = grid.HitTest (e.X, e.Y);
			if(hti.Type  == DataGrid.HitTestType.Cell ){
				row = hti.Row ;
			}
		}
		//-----
		public void Add(Swimmer sw) {
			DataRow row = dTable.NewRow();
			row["Frname"] = sw.getFrname();
			row[1] = sw.getLName();
			row[2] = sw.getAge();  //This one is an integer
			dTable.Rows.Add(row);
			dTable.AcceptChanges();
		}
		//-----
		public int SelectedIndex() {
			return row;
		}
		//-----
		public void Clear() {
			int  count = dTable.Rows.Count ;
			for(int i=0; i< count; i++) {
				dTable.Rows[i].Delete ();
			}
		}
		//-----
		public void clearSelection() {}
	}
}
