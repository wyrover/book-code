package org.as3lib.kitchensync.action.tweentarget
{
	import org.as3lib.kitchensync.KitchenSyncDefaults;
	
	/**
	 * A TweenTarget used to tween numeric properties of an object.
	 * 
	 * @use Generally used internally by KSTween but can be explicitly created thusly...
	 * 		<code>
	 * 		var target:Sprite = new Sprite();
	 * 		var property:String = "x";
	 * 		var startValue:Number = 0;
	 * 		var endValue:Number = 250;
	 *		var tweenTargetProperty:ITweenTarget = new TargetProperty(target, property, startValue, endValue);
	 * 
	 * 		var duration:int = 2000;
	 *		var delay:int = 500;
	 *		var tweeen:KSTween = KSTween.newWithTweenTarget(tweenTargetProperty, duration, delay);
	 * 		</code>
	 * 
	 * @since 1.5
	 * @author Mims H. Wright
	 */
	public class TargetProperty implements ITweenTarget
	{
		protected const NON_NUMERIC_PROPERTY_ERROR:String = "The 'property' of the object 'target' must be a Number, int, or uint.";
		
		/**
		 * The object containing the property you want to tween.
		 */
		public function get target():Object { return _target; }
		protected var _target:Object;
		
		/**
		 * The string name of the property of the target object that you want to tween.
		 * Note: the property must be a numeric value.
		 */
		public function get property():String { return _property; }
		protected var _property:String;
		
		/**
		 * Sets the target object and property name that will be tweened.  
		 * 
		 * @param target an object that contains the numeric property to tween.
		 * @param property the name of the numeric property to tween.
		 */
		public function setTargetPropterty(target:Object, property:String):void {
			if (isPropertyValid(target, property)) {
				_target = target;
				_property = property;
			} else {
				throw new Error (NON_NUMERIC_PROPERTY_ERROR);
			}
		}
		
		protected function isPropertyValid(target:Object, property:String):Boolean {
			return (target[property] is Number);
		}
		
		/**
		 * The current value of the property. Using the setter directly sets the value. 
		 * 
		 * @throws Error if the target or property aren't set.
		 */
		public function get currentValue():Number { 
			if (_target) {
				return _target[_property];
			} else { 
				throw new Error ("Target doesn't exist.");
				return NaN; 
			}
		}
		public function set currentValue(currentValue:Number):void{
			if (_target && _property) {
				_target[_property] = currentValue;
			} else {
				throw new Error ("Target and Property must both be defined before setting the value.");
			}
		}
		
		/**
		 * The value to start from when tweening.
		 */ 
		public function get startValue():Number	{ return _startValue; }
		public function set startValue(startValue:Number):void { _startValue = startValue; }
		protected var _startValue:Number;
		
		/**
		 * The value to end on when tweening.
		 */		
		public function get endValue():Number {	return _endValue; }
		public function set endValue(endValue:Number):void	{ _endValue = endValue; }
		protected var _endValue:Number;
		
		/**
		 * The total amount of change between the start and end values. (used internally)
		 */
		public function get differenceInValues():Number { return _endValue - _startValue; }
		
		/**
		 * Indicates whether tweened values should snap to whole value numbers or use decimals.
		 * If set to true, the results of the easing functions on the target property will be 
		 * rounded to the nearest integer.
		 * 
		 * @see org.as3lib.kitchensync.ActionDefaults
		 */
		 // todo rename to snapToInteger 
		public function get snapToInteger():Boolean { return _snapToInteger; }
		public function set snapToInteger(snapToInteger:Boolean):void { _snapToInteger = snapToInteger; }
		protected var _snapToInteger:Boolean;
		
		/**
		 * Constructor.
		 * 
		 * @param target the object whose property you want to tween
		 * @param property the name of the numeric property to tween
		 * @param startValue the value to start from when tweening
		 * @param endValue the value to end on when tweening 
		 */
		public function TargetProperty (target:Object, property:String, startValue:Number = NaN, endValue:Number = NaN) {
			setTargetPropterty(target, property);
			_startValue = (isNaN(startValue)) ? currentValue : startValue;
			_endValue   = (isNaN(endValue))   ? currentValue : endValue;
			_snapToInteger = KitchenSyncDefaults.snapToInteger;
		}
		
		/**
		 * The main function that the Tween uses to update the TweenTarget. 
		 * Sets the percentage complete.
		 * 
		 * @param percentComplete a number between 0 and 1 (but sometimes more or less) that represents
		 * 		  the percentage of the tween that has been completed. This should update
		 * @return Number the new current value of the tween.
		 */
		public function updateTween(percentComplete:Number):Number {
			currentValue = percentComplete * differenceInValues + startValue;
			if (_snapToInteger) { currentValue = Math.round(currentValue); }
			return Number(currentValue);
		}
		
		/** Returns the tweenTarget to its pre-tweened state */
		public function reset():void {
			currentValue = startValue;
		}
		
		/** Create a copy of the tweenTarget object */
		public function clone():ITweenTarget {
			var clone:TargetProperty = new TargetProperty(_target, _property, _startValue, _endValue);
			clone.snapToInteger = _snapToInteger;
			return clone;
		}
		
		public function toString():String {
			return target + "." + property + " from " + startValue + " to " + endValue;
		}
	}
}