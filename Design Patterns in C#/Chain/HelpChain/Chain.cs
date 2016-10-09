using System;
using System.Windows.Forms;
namespace HelpChain
{
	public abstract class Chain	{
		//describes how all chains work
		private bool hasLink;
		protected Control control;
		protected Chain chn;
		protected string message;
		
		public Chain(Control c, string mesg)	{
			hasLink = false;
			control = c;		//save the control
			message = mesg;
		}
		
		public abstract void sendToChain();
		//-----
		public void addToChain(Chain c) {
			//add new element to chain
			chn = c;
			hasLink = true;		//flag existence
		}
		//-----
		public Chain getChain() {
			return chn;	//get the chain link
		}
		//-----
		public bool hasChain() {
			return hasLink;		//true if linked to another
		}
		//-----
		protected void sendChain() {
			//send message on down the chain
			if(chn != null)
				chn.sendToChain ();
		}
	}
}
