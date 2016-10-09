using System;

namespace Person
{
	/// <summary>
	/// Summary description for Person.
	/// </summary>
	public class Person
	{
		private string name;
		private int age;
		//-----
		public Person(string nm, int ag) 		{
			name = nm;
			age = ag;
		}
		public string makeJob() {
			return "hired";
		}
		public int getAge() {
			return age;
		}
		public void splitNames() {
		}

	}
}
