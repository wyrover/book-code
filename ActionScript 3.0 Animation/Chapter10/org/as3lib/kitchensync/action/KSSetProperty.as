package org.as3lib.kitchensync.action
{	
	/**
	 * SynchronizedAction to set a property for any object to a specified value.
	 */
	public class KSSetProperty extends KSFunction
	{
		/**
		 * @param target - The object whose property you want to modify.
		 * @param key - The name of the property to modify.
		 * @param value - The new value of the property.
		 * @param delay - The delay before the action executes.
		 */ 
		public function KSSetProperty(target:Object, key:String, value:*, delay:* = 0)
		{
			super(delay, setProperty, target, key, value);
		}
		
		/** This is the function that will be called to set the property. */
		protected function setProperty(target:Object, key:String, value:*):void {
			target[key] = value;	
		}
	}
}