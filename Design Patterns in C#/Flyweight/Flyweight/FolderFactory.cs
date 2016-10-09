using System;
using System.Drawing ;
namespace Flyweight
{
	/// <summary>
	/// Summary description for FolderFactory.
	/// </summary>
	public class FolderFactory	{
		private Folder selFolder, unselFolder;
		//-----
		public FolderFactory() 		{
			//create the two folders
			selFolder = new Folder(Color.Brown);
			unselFolder = new Folder(Color.Bisque);
		}
		//-----
		public Folder getFolder(bool selected) {
			if(selected)
				return selFolder;
			else
				return unselFolder;
		}
	}
}
