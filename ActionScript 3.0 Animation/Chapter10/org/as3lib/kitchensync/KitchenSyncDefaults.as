package org.as3lib.kitchensync
{
	import org.as3lib.kitchensync.action.ITweenObjectParser;
	import org.as3lib.kitchensync.action.KitchenSyncObjectParser;
	import org.as3lib.kitchensync.easing.Linear;
	import org.as3lib.kitchensync.utils.*;
	
	public class KitchenSyncDefaults
	{
		public static var syncrhonizerUsesWeakReferences:Boolean = false;
		
		// ALL ACTIONS
		public static var autoDelete:Boolean = false;
		public static var sync:Boolean = true;
		public static var timeStringParser:ITimeStringParser = new TimeStringParser_en();
		public static var easingFunction:Function = Linear.ease;
		
		// TWEENS
		public static var snapToValueOnComplete:Boolean = true;
		public static var snapToInteger:Boolean = false;
		public static var tweenObjectParser:ITweenObjectParser = new KitchenSyncObjectParser();
		
		// GROUPS
		public static var resetChildrenAtStart:Boolean = true;
	}
}