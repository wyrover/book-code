using System;

namespace Chain
{
	public abstract class Chain	{
		//describes how all chains work
		private bool hasLink;
		protected Chain chn;
		public Chain()	{
			hasLink = false;	
		}
		//you must implement this in derived classes
		public abstract void sendToChain(string mesg);
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
		protected void sendChain(string mesg) {
			//send message on down the chain
			if(chn != null)
				chn.sendToChain (mesg);
		}
	}
}
