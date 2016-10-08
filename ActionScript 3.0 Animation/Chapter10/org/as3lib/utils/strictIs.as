package org.as3lib.utils
{
	import flash.utils.*;
	
	/**
	 * Checks the class of <code>instance</code> against the <code>compareClass</code> for strict
	 * equality. If the classes are exactly the same, returns true. If
	 * the classes are different even if the <code>instance</code>'s class is a subclass
	 * of <code>compareClass</code>, it returns false.
	 * Does not work with interfaces. The compareClass must be a class.
	 * 
	 * @author Mims Wright
	 * 
	 * @example <listing version="3.0">
	 * 			var myBase:BaseClass = new BaseClass();
	 * 			var mySub:SubClass = new SubClass();
	 * 			trace(strictIs(myBase, BaseClass)); // true
	 * 			trace(strictIs(mySub, SubClass)); 	// true
	 * 			trace(strictIs(mySub, BaseClass)); 	// false
	 * 			</listing>
	 * 
	 * @param instance - the object whos class you want to check.
	 * @param compareClass - the class to compare your object against.
	 * @return true if instance is strictly of type compareClass.
	 */
	public function strictIs(instance:Object, compareClass:Class):Boolean {
		var instanceClass:Class = Class(getDefinitionByName(getQualifiedClassName(instance)));
		return instanceClass == compareClass;
	}
}