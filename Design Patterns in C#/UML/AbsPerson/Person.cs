using System;

namespace AbsPerson
{
	/// <summary>
	/// Summary description for Person.
	/// </summary>
	public abstract class Person
	{
				protected string name;
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
				public abstract string getJob();	//must override

		
	}
}
