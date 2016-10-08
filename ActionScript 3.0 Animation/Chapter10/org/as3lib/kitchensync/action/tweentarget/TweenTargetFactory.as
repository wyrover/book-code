package org.as3lib.kitchensync.action.tweentarget
{
	import flash.display.DisplayObject;
	
	
	
	/** 
	 * NOT YET IMPLEMENTED.
	 * Manages the instantiation of ITweenTarget objects.
	 * 
	 * @example <listing version="3.0">
	 * 		var tweenTarget:ITweenTarget = TweenTargetFactory.
	 * 		</listing>
	 * 
	 * @see ITweenTarget
	 * @see TweenTargetType
	 * @see Tween
	 */
	public class TweenTargetFactory
	{
		private static var _instance:TweenTargetFactory;
		public static function get instance():TweenTargetFactory { 
			if (!_instance) { _instance = new TweenTargetFactory(new SingletonEnforcer()); }
			return _instance; 
		}
		public function TweenTargetFactory (singletonEnforcer:SingletonEnforcer) {}
		
		public function getTweenTarget(target:Object, property:String):ITweenTarget {
			/*	
			if (target is DisplayObject) {
				switch ( property ) {
					break;
				}
			}
			*/

			if (target[property] is Number) {
				return new TargetProperty(target, property);
			} else { 
				throw new TypeError("The specified target's property must be a number.");
			}
		}
	}
}
class SingletonEnforcer {}