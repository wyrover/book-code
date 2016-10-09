using System;
using System.Data ;
using System.Data.SqlClient ;
using System.Data.OleDb ;
using System.Collections ;

namespace Facade
{
	/// <summary>
	/// Summary description for DBTable.
	/// </summary>
	public class DBTable 	{
		protected DBase db;
		protected string tableName;
		private bool filled, opened;
		private DataTable dtable;
		private int rowIndex;
		private Hashtable names;
		private string columnName;
		private DataRow row;
		private OleDbConnection conn;
		private int index;
	//-----
		public DBTable(DBase datab, string tb_Name) 		{
			db = datab;
			tableName = tb_Name;
			filled =false;
			opened = false;
			names = new Hashtable();
		}
		//-----
		public void createTable() {
			try {
				dtable = new DataTable(tableName);
				dtable.Clear();
			}
			catch (Exception e) {
				Console.WriteLine (e.Message );
			}
		}
		//-----
		public bool hasMoreElements() {
			if(opened)
				return (rowIndex < dtable.Rows.Count) ;
			else
				return false;
		}
		//-----
		public virtual string getValue(string cname) {
		 //returns the next name in the table
        //assumes that openTable has already been called
			if (opened) {
				DataRow row = dtable.Rows[rowIndex++];
				return row[cname].ToString().Trim ();
			}
			else
				return "";
         }
		//-----
		public int getKey(string nm, string keyname){							
			DataRow row;
			int key;
			if(! filled)
				return (int)names[ nm];
			else {
				string query = "select * from " + tableName + " where " + columnName + "=\'" + nm + "\'";
				dtable = db.openQuery(query);
				row = dtable.Rows[0];
				key = Convert.ToInt32 (row[keyname].ToString());
				return key;
				}
		}
		//-----	
		public virtual void makeTable(string cName) {
			columnName = cName;
			//stores current hash table values in data table
			DataSet dset = new DataSet(tableName);   //create the data set
			dtable = new DataTable(tableName);   //and a datatable
	        dset.Tables.Add(dtable);             //add to collection
		    conn = db.getConnection();
			openConn();                      //open the connection
			OleDbDataAdapter adcmd = new OleDbDataAdapter();
			//open the table
			adcmd.SelectCommand = 
				new OleDbCommand("Select * from " + tableName, conn);
			OleDbCommandBuilder olecb = new OleDbCommandBuilder(adcmd);
			adcmd.TableMappings.Add("Table", tableName);
			//load current data into the local table copy
			adcmd.Fill(dset, tableName);
			//get the Enumerator from the Hashtable
			IEnumerator ienum = names.Keys.GetEnumerator();
			//move through the table, adding the names to new rows
			while (ienum.MoveNext()) {
				string name = (string)ienum.Current;
				row = dtable.NewRow();     //get new rows
				row[columnName] = name;
				dtable.Rows.Add(row);    //add into table
			}
        //Now update the database with this table
			try {
				adcmd.Update(dset);
				closeConn();
				filled = true;
			}
			catch (Exception e) {
				Console.WriteLine (e.Message);
			}

		}
		//-----
		private void closeConn() {
		 if( conn.State == ConnectionState.Open) {
					 conn.Close();
		 }
        }
		//-----
		private void openConn() {
			if(conn.State == ConnectionState.Closed ) {
				conn.Open();
			}
		}
		//-----
		public void addTableValue(string nm) {
		//accumulates names in hash table
			try {
				names.Add(nm, index++);
			}
			catch (ArgumentException) {}
            //do not allow duplicate names to be added
          
		}
		public void openTable() {
			dtable = db.openTable(tableName);
			rowIndex = 0;
			if(dtable != null)
				opened = true;
		}
		//-----
		public void delete() {
			//deletes entire table
			conn = db.getConnection();
			openConn();
			if (conn.State == ConnectionState.Open ) {
				OleDbCommand adcmd = 
					new OleDbCommand("Delete * from " + tableName, conn);
				try{
					adcmd.ExecuteNonQuery();
					closeConn();
				}
				catch (Exception e) {
					Console.WriteLine (e.Message);
				}
			}
		}
	}
}
