using System;
using System.Data ;
using System.Data.SqlClient ;
using System.Data.OleDb ;
using System.Collections ;

namespace Facade
{
	/// <summary>
	/// Summary description for Prices.
	/// </summary>
	public class Prices : DBTable 	{
		private ArrayList priceList;
		public Prices(DBase db) : base(db, "Prices") 	{
			priceList = new ArrayList ();
		}
		//-----
		public void makeTable() {
			//stores current array list values in data table
        OleDbConnection adc = new OleDbConnection();
        
        DataSet dset = new DataSet(tableName);
        DataTable dtable = new DataTable(tableName);

        dset.Tables.Add(dtable);
        adc = db.getConnection();
        if (adc.State == ConnectionState.Closed) 
			adc.Open();
        OleDbDataAdapter adcmd = new OleDbDataAdapter();

        //fill in price table
        adcmd.SelectCommand = 
			new OleDbCommand("Select * from " + tableName, adc);
        OleDbCommandBuilder custCB = new OleDbCommandBuilder(adcmd);
        adcmd.TableMappings.Add("Table", tableName);
        adcmd.Fill(dset, tableName);
        IEnumerator ienum = priceList.GetEnumerator();
        //add new price entries
			while (ienum.MoveNext() ) {
				StoreFoodPrice fprice = (StoreFoodPrice)ienum.Current;
				DataRow row = dtable.NewRow();
				row["foodkey"] = fprice.getFood();
				row["storekey"] = fprice.getStore();
				row["price"] = fprice.getPrice();
				dtable.Rows.Add(row);    //add to table
			}
        adcmd.Update(dset);      //send back to database
        adc.Close();
        }
		//-----
		public DataTable getPrices(string food) {
			string query=
        "SELECT Stores.StoreName, " +
        "Foods.Foodname, Prices.Price " +
        "FROM (Prices INNER JOIN Foods ON " +
        "Prices.Foodkey = Foods.Foodkey) " +
        "INNER JOIN Stores ON Prices.StoreKey = Stores.StoreKey " +
        "WHERE(((Foods.Foodname) = \'" + food + "\')) " +
        "ORDER BY Prices.Price";
        return db.openQuery(query);
		}
		//-----
		public void addRow(int storeKey, int foodKey, float price) {
			priceList.Add (new StoreFoodPrice (storeKey, foodKey, price));
		}
	}
}
