package org.as3lib.utils
{
	import flash.utils.*;
	
	/**
	 * Checks to see if the class of <code>instance</code> is a primitive which will be referenced by value
	 * rather than by pointer.
	 * The primitive types checked for are :
	 * Number
	 * int
	 * uint
	 * String
	 * Boolean
	 * 
	 * @author Mims Wright
	 * 
	 * @param instance - the object whos class you want to check.
	 * @return true if instance is is a primitive value.
	 */
	 // todo test
	 // todo move to ClassUtils?
	public function isPrimitive(instance:*):Boolean {
		if (!isNaN(instance) return true;
		if (instance is String) return true;
		if (instance is Boolean) return true;
		if (instance is void) return true;
		return false;
	}
}