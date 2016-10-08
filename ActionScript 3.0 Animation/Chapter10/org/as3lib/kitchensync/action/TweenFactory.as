package org.as3lib.kitchensync.action
{
	import org.as3lib.kitchensync.KitchenSyncDefaults;
	
	/**
	 * Provides a convenient interface for creating all types of tweens.
	 * 
	 * @author Mims Wright
	 * @since 1.6
	 */
	final public class TweenFactory
	{
		public static var objectParser:ITweenObjectParser = KitchenSyncDefaults.tweenObjectParser;
		
		/** 
		 * Uses a generic object to define parameters for a new tween. See ITweenParser and KitchenSyncObjectParser for more details.
		 *
		 * @see ITweenObjectParser
		 * @see KitchenSyncObjectParser
		 * 
		 * @param parameters An object that contains properties that define the new tween
		 * @return A new tween.
		 */ 
		public static function newTween(parameters:Object):ITween {
			return objectParser.parseObject(parameters);
		}
		
		/**
		 * Creates a tween with a TargetProperty as it's tween target. 
		 * 
		 * @see #newWithTweenTarget()
		 * 
		 * @param target - the object whose property will be changed
		 * @param property - the name of the property to change. The property must be a Number, int or uint such as a Sprite object's "alpha"
		 * @param startValue - the value to tween the property to. After the tween is done, this will be the value of the property.
		 * @param endValue - the starting value of the tween. By default, this is the value of the property before the tween begins.
		 * @param duration - the time in milliseconds that this tween will take to execute. String values are acceptable too.
		 * @param delay - the time to wait in milliseconds before starting the tween. String values are acceptable too.
		 * @param easingFunction - the function to use to interpolate the values between fromValue and toValue.
		 */
		public static function newWithTargetProperty(target:Object = null, property:String = "", startValue:Number = 0, endValue:Number = 0, duration:* = 0, delay:* = 0, easingFunction:Function = null):KSTween {
			return new KSTween(target, property, startValue, endValue, duration, delay, easingFunction);
		}


		/**
		 * Creates a new KSTween using an ITweenTarget that you pass into it.
		 * 
		 * @param tweenTarget An explicitly defined tweenTarget object (or an array of tweentargets) that contains the values you want to tween.
		 * @param duration - the time in frames that this tween will take to execute.
		 * @param delay - the time to wait before starting the tween.
		 * @param easingFunction - the function to use to interpolate the values between fromValue and toValue.
		 * @return A new KSTween object.
		 */
		public static function newWithTweenTarget(tweenTarget:*, duration:* = 0, delay:* = 0, easingFunction:Function = null):KSTween {
			return KSTween.newWithTweenTarget(tweenTarget, duration, delay, easingFunction);
		} 

	}
}