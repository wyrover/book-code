package org.as3lib.kitchensync.action
{
	/**
	 * Represents a reserved word used in a tween object parser.
	 */
	public class ObjectParserReservedWord
	{
		public function get term():String { return _term; }
		protected var _term:String;
		
		public function get type():String { return _type; }
		protected var _type:String;
		
		public function get description():String { return _description; }
		protected var _description:String;
		
		public function get aliases():Array { return _aliases }
		protected var _aliases:Array;
		
		public function get hasAliases():Boolean { return _aliases.length > 0 }
		
		public function get allReservedWords():Array { return _aliases.concat(_term); }
		
		public function ObjectParserReservedWord(term:String, type:String, description:String, ... aliases)
		{
			_term = term;
			_type = type;
			_description = description;
			_aliases = new Array();
			
			for each (var alias:String in aliases) {
				_aliases.push(alias);
			}
		}
		
		public function toString():String {
			var str:String = _term + ":" + _type + " - " + _description;
			if (hasAliases) {
				str += " Aliases: " + _aliases.join(",");
			}
			return str;
		}

	}
}