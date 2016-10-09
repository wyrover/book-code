using System;
using System.Data ;
using System.Data.SqlClient ;
using System.Data.OleDb ;

namespace Facade
{
	/// <summary>
	/// Summary description for DBase.
	/// </summary>
	public abstract class DBase 	{
		protected OleDbConnection conn;
		private void openConnection() {
			if (conn.State == ConnectionState.Closed){
				conn.Open ();
			}
		}
		//------
		private void closeConnection() {
			if (conn.State == ConnectionState.Open ){
				conn.Close ();
			}
		}
		//------
		public DataTable openTable (string tableName) {
			OleDbDataAdapter adapter = new OleDbDataAdapter ();
			DataTable dtable = null;
			string query = "Select * from " + tableName;
			adapter.SelectCommand = new OleDbCommand (query, conn);
			DataSet dset = new DataSet ("mydata");
			try {
				openConnection();
				adapter.Fill (dset);
				dtable = dset.Tables [0];
			}
			catch(Exception e) {
				Console.WriteLine (e.Message );
			}
			
			return dtable;
		}
		//------
		public DataTable openQuery(string query) {
			OleDbDataAdapter dsCmd = new OleDbDataAdapter ();
			DataSet dset = new DataSet ();	//create a dataset
			DataTable dtable = null;		//declare a data table
			try {
				//create the command
				dsCmd.SelectCommand = new OleDbCommand(query, conn);
				openConnection();			//open the connection
				//fill the dataset
				dsCmd.Fill(dset, "mine");
				//get the table
				dtable = dset.Tables[0];
				closeConnection();			//always close it
				return dtable;				//and return it
			}
			catch (Exception e) {
				Console.WriteLine (e.Message);
				return null;
			}
		}
		//------
		public void openConnection(string connectionString) {
			conn = new OleDbConnection(connectionString);
		}
		//------
		public OleDbConnection getConnection() {
			return conn;
		}
	}
}
