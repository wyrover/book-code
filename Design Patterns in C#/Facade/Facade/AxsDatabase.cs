using System;
using System.Data ;
using System.Data.SqlClient ;
using System.Data.OleDb ;
namespace Facade
{
	/// <summary>
	/// Summary description for AxsDatabase.
	/// </summary>
	public class AxsDatabase :DBase
	{
		public AxsDatabase(string dbName)		{
			 string connectionString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dbName;
			 openConnection(connectionString);
		}
	}
}
