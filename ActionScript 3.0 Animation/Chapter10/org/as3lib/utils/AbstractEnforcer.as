package org.as3lib.utils
{
	import org.as3lib.utils.strictIs;
	import org.as3lib.errors.AbstractError;
	
	public class AbstractEnforcer
	{
		public static function enforceConstructor(instance:Object, className:Class):void {
			if (strictIs(instance, className)) {
				throw (new AbstractError(AbstractError.CONSTRUCTOR_ERROR));
			}
		}
		
		public static function enforceMethod ():void {
			throw (new AbstractError(AbstractError.METHOD_ERROR));
		}
	}
}