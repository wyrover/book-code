using System;
using System.Collections ;
using System.Windows.Forms;
using CsharpPats;
namespace Interpreter
{
	/// <summary>
	/// Summary description for Parser.
	/// </summary>
	public class Parser
	{
		private Stack stk;
		private ArrayList actionList;
		//private KidData kdata;
		private Data dat;
		private ListBox ptable;
		private Chain chn;

		public Parser(string line, KidData kd, ListBox pt) 		{
			stk = new Stack ();
			actionList = new ArrayList ();
			setData(kd, pt);
			buildStack(line);
			buildChain();
		}
		//-----
		private void buildChain() {
			chn = new VarVarParse(); //start of chain
			VarMultvarParse vmvp = new VarMultvarParse();
			chn.addToChain(vmvp);
			MultVarVarParse mvvp = new MultVarVarParse();
			vmvp.addToChain(mvvp);
			VerbMultvarParse vrvp = new VerbMultvarParse();
			mvvp.addToChain(vrvp);
			VerbVarParse vvp = new VerbVarParse();
			vrvp.addToChain(vvp);
			VerbAction va = new VerbAction(actionList);
			vvp.addToChain(va);
			Nomatch nom = new Nomatch ();
			va.addToChain (nom);
		}
		//-----
		public void setData(KidData kd, ListBox pt) {
			dat = new Data(kd.getData ());
			ptable = pt;
		}
		//-----
		public void Execute() {
			while(stk.hasMoreElements () ) {
				chn.sendToChain (stk);
			}
			//now execute the verbs
			for(int i=0; i< actionList.Count ; i++ ) {
				Verb v = (Verb)actionList[i];
				v.setData (dat, ptable);
				v.Execute ();
			}
		}
		//-----
		private void buildStack(string s) {
			StringTokenizer tok = new StringTokenizer (s);
			while(tok.hasMoreElements () ) {
				ParseObject token = tokenize(tok.nextToken ());
				stk.push (token);
			}
		}
		//-----
		protected ParseObject tokenize(string s) {
			ParseObject obj;
			int type;
			try {
				obj = getVerb(s);
				type = obj.getType ();
			}
			catch(NullReferenceException) {
				obj = getVar(s);
			}
			return obj;
		}
		//-----
		protected ParseVerb getVerb(string s) {
			ParseVerb v = new ParseVerb (s, dat, ptable);
			if(v.isLegal () )
				return v.getVerb (s);
			else
				return null;
		}
		//-----
		protected ParseVar getVar(string s) {
			ParseVar v = new ParseVar (s);
			if( v.isLegal())
				return v;
			else
				return null;
		}
	}
}
