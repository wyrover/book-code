using System;
using System.Data ;
using System.Data.SqlClient ;
using System.Data.OleDb ;

namespace Facade
{
	/// <summary>
	/// Summary description for SQLServerDatabase.
	/// </summary>
	public class SQLServerDatabase:DBase 	{
		string connectionString;
		//-----
		public SQLServerDatabase(String dbName) 		{
			 connectionString = "Persist Security Info = False;" +
                "Initial Catalog =" + dbName + ";" +
                "Data Source = myDataServer;User ID = myName;" +
                "password=";
			openConnection(connectionString);
		}
		//-----
		public SQLServerDatabase(string dbName, string serverName, string userid, string pwd) {
			 connectionString = "Persist Security Info = False;" +
                "Initial Catalog =" + dbName + ";" +
                "Data Source =" + serverName + ";" +
                "User ID =" + userid + ";" +
                "password=" + pwd;
			openConnection(connectionString);
        }
	}
}
