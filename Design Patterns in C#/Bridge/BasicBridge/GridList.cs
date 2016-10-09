using System;
using System.Windows.Forms;
using System.Data;
namespace BasicBridge
{
	/// <summary>
	/// Summary description for GridList.
	/// </summary>
	public class GridList:VisList 	{
		private DataGrid grid;
		private DataTable dtable;
		private GridAdapter gAdapter;
		//-----
		public GridList(DataGrid grd) {
			grid = grd;
			dtable = new DataTable("Products");
			DataColumn column = new DataColumn("ProdName");
			dtable.Columns.Add(column);
			column = new DataColumn("Qty");
			dtable.Columns.Add(column);
			grid.DataSource = dtable;
			gAdapter = new GridAdapter (grid);
		}
		//-----
		public void addLine(Product p) {
			gAdapter.Add (p);
		}
		//-----
		public void removeLine(int num) {
			
		}
	}
}
