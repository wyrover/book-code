using System;
using CsharpPats;

namespace Facade
{
	/// <summary>
	/// Summary description for DataLoader.
	/// </summary>
	public class DataLoader 	{
		private csFile vfile;
		private Stores store;
		private Foods fods;
		private Prices price;
		private DBase db;
		//-----
		public DataLoader(DBase datab) 	{
			db = datab;
			store = new Stores(db);
			fods = new Foods (db);
			price = new Prices(db);
		}
		//-----
		public void load(string dataFile) {
			string sline;
			int storekey, foodkey;
			StringTokenizer tok;

			//delete current table contents
			store.delete();
			fods.delete();
			price.delete();
			//now read in new ones
			vfile = new csFile(dataFile);
			vfile.OpenForRead();
			sline = vfile.readLine();
			while (sline != null){
				tok = new StringTokenizer(sline, ",");
				store.addTableValue(tok.nextToken());   //store name
				fods.addTableValue(tok.nextToken());   //food name
				sline = vfile.readLine();
			}
			vfile.close();
			//construct store and food tables
			store.makeTable();
			fods.makeTable();
			vfile.OpenForRead();
			sline = vfile.readLine();
		while (sline != null) {
			//get the gets and add to storefoodprice objects
			tok = new StringTokenizer(sline, ",");
			storekey = store.getKey(tok.nextToken(), "Storekey");
			foodkey = fods.getKey(tok.nextToken(), "Foodkey");
			price.addRow(storekey, foodkey, Convert.ToSingle (tok.nextToken()));
			sline = vfile.readLine();
		}
        //add all to price table
        price.makeTable();
        vfile.close();
		}
	}
}
