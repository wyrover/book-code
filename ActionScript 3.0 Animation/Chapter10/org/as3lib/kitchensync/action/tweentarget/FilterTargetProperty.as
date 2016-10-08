package org.as3lib.kitchensync.action.tweentarget
{
	import flash.display.DisplayObject;
	import flash.filters.BitmapFilter;
	
	// todo add docs
	public class FilterTargetProperty implements IFilterTweenTarget
	{
		/**
		 * The object containing the filter property you want to tween. 
		 */
		public function get target():DisplayObject { return _target; }
		protected var _target:DisplayObject;
		
		public function get filterType():Class { return _filterType; }
		protected var _filterType:Class;
		
		/**
		 * The string name of the property of the target object that you want to tween.
		 */
		public function get property():String { return _property; }
		protected var _property:String;
		
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
		 * Constructor.
		 * 
		 * @param target the object whose property you want to tween
		 * @param property the name of the numeric property to tween
		 * @param startValue the value to start from when tweening
		 * @param endValue the value to end on when tweening 
		 */
		public function FilterTargetProperty (target:DisplayObject, filterType:Class, property:String, startValue:Number = NaN, endValue:Number = NaN) {
			_target = target;
			_filterType = filterType;
			_property = property;
			_startValue = (isNaN(startValue)) ? currentValue : startValue;
			_endValue   = (isNaN(endValue))   ? currentValue : endValue;
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
			return currentValue = percentComplete * differenceInValues + startValue;
		}
				
		public function get currentValue():Number { return Number(_previousFilter[property]); }
		public function set currentValue(currentValue:Number):void { 
			var newFilters:Array = [];
			
			// if the previous filter array contains any filters
			if (filters.length > 0) {
				// pull in all the old filters except the one added previously.
				for each (var filter:BitmapFilter in filters) {
					if (filter is _filterType) {
						_previousFilter = filter;
					} else {
						newFilters.push(filter);
					}
				}
			}
			
			var newFilter:BitmapFilter = _previousFilter == null ? getDefaultFilter(_filterType) : _previousFilter;
			newFilter[_property] = currentValue;
			
			// add the newFilter
			newFilters.push(newFilter);

			// apply the filters
			filters = newFilters;
			// save the previous filter.
			_previousFilter = newFilter;
		}
		protected var _previousFilter:BitmapFilter = null;
		
		protected function get filters ():Array {
			if (_target != null) { return _target.filters; }
			return null;
		}
		protected function set filters (filters:Array):void { 
			if (_target != null) { _target.filters = filters; }
			else { throw new Error("The target must be defined before setting filters"); }
		}
		
		/**
		 */		
		protected function getDefaultFilter(filterType:Class):BitmapFilter { 
			var emptyFilter:BitmapFilter = new filterType();
			return emptyFilter;
		}
		
		public function reset():void
		{
			currentValue = startValue;
		}
		
		public function clone():ITweenTarget
		{
			return new FilterTargetProperty(_target, _filterType, _property, _startValue, _endValue);
			
		}
		
	}
}