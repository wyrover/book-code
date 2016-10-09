using System;

namespace Facade
{
	/// <summary>
	/// Summary description for StoreFoodPrice.
	/// </summary>
	public class StoreFoodPrice {
		private int storeKey, foodKey;
		private float foodPrice;
		//-----
		public StoreFoodPrice(int sKey, int fKey, float fPrice) {
			storeKey = sKey;
			foodKey = fKey;
			foodPrice = fPrice;
		}
		//-----
		public int getStore() {
			return storeKey;
		}
		//-----
		public int getFood() {
			return foodKey;
		}
		//-----
		public float getPrice() {
			return foodPrice;
		}
	}
}
