using System;
using System.Text;

namespace CombinationsLib
{
	public class Combination
	{
		private long n = 0;
		private long k = 0;
		private long[] data = null;

		public Combination(long n, long k)
		{
			if (n < 0 || k < 0) // normally require n >= k  
				throw new Exception("Negative parameter in constructor");

			this.n = n;
			this.k = k;
			this.data = new long[k];
			for (long i = 0; i < k; ++i)
				this.data[i] = i;
		} // Combination(n,k)

		public Combination(long n, long k, long[] a) // Combination from a[]
		{
			if (k != a.Length)
				throw new Exception("Array length does not equal k");

			this.n = n;
			this.k = k;
			this.data = new long[k];
			for (long i = 0; i < a.Length; ++i)
				this.data[i] = a[i];

			if (!this.IsValid())
				throw new Exception("Bad value from array");
		} // Combination(n,k,a)

		public bool IsValid()
		{
			if (this.data.Length != this.k)
				return false; // corrupted

			for (long i = 0; i < this.k; ++i)
			{
				if (this.data[i] < 0 || this.data[i] > this.n - 1)
					return false; // value out of range

				for (long j = i+1; j < this.k; ++j)
					if (this.data[i] >= this.data[j])
						return false; // duplicate or not lexicographic
			}

			return true;
		} // IsValid()

		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();
			sb.Append("{ ");
			for (long i = 0; i < this.k; ++i)
				sb.AppendFormat("{0} {1}", this.data[i], " ");
			sb.Append("}");
			return sb.ToString();
		} // ToString()

		public Combination Successor()
		{
			if (this.data.Length == 0 ||
				this.data[0] == this.n - this.k)
				return null;

			Combination ans = new Combination(this.n, this.k);

			long i;
			for (i = 0; i < this.k; ++i)
				ans.data[i] = this.data[i];
 
			for (i = this.k - 1; i > 0 && ans.data[i] == this.n - this.k + i; --i);
 
			++ans.data[i];

			for (long j = i; j < this.k - 1; ++j)
				ans.data[j+1] = ans.data[j] + 1;

			return ans;
		} // Successor()

		public Combination First()
		{
			Combination ans = new Combination(this.n, this.k);
      
			for (long i = 0; i < ans.k; ++i) 
				ans.data[i] = i;

			return ans;
		} // First()

		public string[] ApplyTo(string[] strarr)
		{
			if (strarr.Length != this.n)
				throw new Exception("Array size not equal to Combination order in ApplyTo()");

			string[] result = new string[this.k];

			for (long i = 0; i < result.Length; ++i)
				result[i] = strarr[this.data[i]];

			return result;
		} // ApplyTo()

		public static long Choose(long n, long k)
		{
			if (n < 0 || k < 0)
				throw new Exception("Invalid negative parameter in Choose()"); 
			if (n < k)
				return 0;  // special case
			if (n == k)
				return 1;

			long delta, iMax;

			if (k < n-k) // ex: Choose(100,3)
			{
				delta = n-k;
				iMax = k;
			}
			else         // ex: Choose(100,97)
			{
				delta = k;
				iMax = n-k;
			}

			long ans = delta + 1;

			for (long i = 2; i <= iMax; ++i)
			{
				checked { ans = (ans * (delta + i)) / i; } // Throws OverFlow Exception
				//Console.WriteLine(" i = " + i + " n = " + n + " k = " + k + " delta = " + delta + " iMax = " + iMax + " ans = " + ans);
			}

			return ans;
		} // Choose()

	} // class Combination
} // ns CombinationsLib