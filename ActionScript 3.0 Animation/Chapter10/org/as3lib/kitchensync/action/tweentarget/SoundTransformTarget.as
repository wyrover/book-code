package org.as3lib.kitchensync.action.tweentarget
{
	import flash.media.SoundChannel;
	import flash.media.SoundTransform;
	
	
	/** 
	 * A tween target for setting sound transform (volume and panning).
	 * 
	 * known issue: high speed transitions, such as oscicators with high frequencies,
	 * can cause clicks and distortion in the sound.
	 */
	// TODO add properties for left to right, right to right, etc.
	public class SoundTransformTarget implements ITweenTarget
	{
	
		/** Use PAN as the property to control panning. */
		public static const PAN:String = "pan";
		/** Use VOLUME as the property to control volume. */
		public static const VOLUME:String = "volume";
		/** Used when a bad property is passed to the constructor. */
		public static const BAD_PROPERTY_ERROR:String = "Property must be either 'pan' or 'volume'. Use SoundTransformTarget.PAN or SoundTransformTarget.VOLUME";
		
		/**
		 * The object containing the sound channel to apply the transform. 
		 */
		public function get channel():SoundChannel { return _channel; }
		public function set channel(channel:SoundChannel):void { _channel = channel; }
		protected var _channel:SoundChannel;
		
		/**
		 * The property to tween. Either 'pan' or 'volume'
		 */
		public function get property():String { return _property; }
		public function set property(property:String):void { 
			_property = property;
			if (_property != PAN && _property != VOLUME) {
				throw new Error(BAD_PROPERTY_ERROR);
			}
		}
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
		 * @param channel The SoundChannel to apply the transform to. 
		 * @param property The property of the sound channel. either pan or volume.
		 * @param startValue the value to start from when tweening
		 * @param endValue the value to end on when tweening 
		 */
		public function SoundTransformTarget (channel:SoundChannel, property:String, startValue:Number = NaN, endValue:Number = NaN) {
			_channel = channel;
			this.property = property.toLowerCase();
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
		
		/**
		 * The current value of the property you specified. 
		 */
		public function get currentValue():Number { 
			if (_property == PAN) { return _channel.soundTransform.pan; }
			if (_property == VOLUME) { return _channel.soundTransform.volume; }
			throw new Error(BAD_PROPERTY_ERROR);
		}
		public function set currentValue(currentValue:Number):void { 
			if (_channel.soundTransform == null) {
				_channel.soundTransform = new SoundTransform();
			}
			var transform:SoundTransform = _channel.soundTransform;
			
			if (_property == PAN) {
				transform.pan = currentValue;
			}
			if (_property == VOLUME) {
				transform.volume = currentValue; 
			}
			
			_channel.soundTransform = transform;
		}
		
		/**
		 * Returns the current value to the start value. 
		 */
		public function reset():void
		{
			currentValue = startValue;
		}
		
		public function clone():ITweenTarget
		{ 
			return new SoundTransformTarget(_channel, _property, _startValue, _endValue);
			
		}
	}
}