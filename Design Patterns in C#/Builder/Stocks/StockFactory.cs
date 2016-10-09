using System;


	/// <summary>
	/// Summary description for StockFactory.
	/// </summary>
	public class StockFactory
	{
		public static MultiChoice getBuilder(Equities stocks)
		{
			if (stocks.count ()<=3) {
				return new CheckChoice (stocks);
			}
			else {
				return new ListChoice(stocks);
			}
		}
	}

