package org.as3lib.kitchensync.easing
{
	/**
	 * <p>Oscillate is slightly different from the other easing classes. The functions in this class cause 
	 * motion that oscillates at a specified frequency based on the timeElapsed. The values will continue to oscilate
	 * between 0.0 and 1.0 without reaching an end point. However, if <code>snapping</code> is set to true, the
	 * functions will return a 1.0 when the duration has elapsed.</p>
	 * <p>The easing classes provide static methods for interpolating the change between two values over time.
	 * Each class handles the interpolation, or easing, differently. Each class typically contains three methods
	 *  - <code>easeIn()</code>, <code>easeOut()</code> and <code>easeInOut()</code> - which vary the rate of change
	 *  of the values. Most of the easing functions produce values as a percentage - a number between 0.0 and 1.0</p>
	 *
	 * 
 	 * @author Mims H. Wright
	 * @see EasingUtil
	 */
	public class Oscillate
	{
		private static const TWICE_PI:Number = Math.PI*2;
		public static var defaultFrequency:Number = 1; // in hertz, that's 1 cycle every 1 second
		public static var defaultPulseWidth:Number = 0.5; // Square Wave
		
		public static var snapping:Boolean = false;
		
		/**
	    * Oscillates between 0 and 1.0 (starting with 0 and ending with 1.0) for as long as the duration lasts 
	    * in a sinusoidal motion. The position will be based on the timeElapsed and frequency.
	    *
	    * @param timeElapsed - Specifies the time since the tween began in milliseconds or frames.
	    * @param duration - Specifies the duration of the tween, in milliseconds or frames.
  	    * @param frequency - The frequency of the sine wave in hertz. Higher is faster. 1/frequency = wavelength
	    * @return Number - between 0.0 and 1.0 based on the time and frequency. Returns 1.0 if duration is elapsed and snapping is turned on.
	    */  
		public static function sine(timeElapsed:Number, duration:Number, frequency:Number = NaN):Number {
			if (isNaN(frequency)) { frequency = defaultFrequency; }
			if (timeElapsed >= duration && snapping == true) { return 1.0 }
			frequency /= 1000;
			return 0.5 * (1 - Math.cos( TWICE_PI * timeElapsed * frequency));
		}	

		/**
	    * Oscillates between 0 and 1.0 (starting with 0 and ending with 1.0) for as long as the duration lasts 
	    * in an absolute sinusoidal (half-wave) motion. The subject will appear to be bouncing.
	    * The position will be based on the timeElapsed and frequency.
	    *
	    * @param timeElapsed - Specifies the time since the tween began in milliseconds or frames.
	    * @param duration - Specifies the duration of the tween, in milliseconds or frames.
  	    * @param frequency - The frequency of the sine wave in hertz. Higher is faster. 1/frequency = wavelength
	    * @return Number - between 0.0 and 1.0 based on the time and frequency. Returns 1.0 if duration is elapsed and snapping is turned on.
	    */  
		public static function absoluteSine(timeElapsed:Number, duration:Number, frequency:Number=NaN):Number {
			if (isNaN(frequency)) { frequency = defaultFrequency; }
			if (timeElapsed >= duration && snapping == true) { return 1.0 }
			frequency /= 1000;
			return Math.abs(Math.cos( TWICE_PI * timeElapsed * frequency));
		}	
	
		/**
	    * Oscillates between 0 and 1.0 (starting with 0 and ending with 1.0) for as long as the duration lasts 
	    * in linear motion.
	    * The position will be based on the timeElapsed and frequency.
	    *
	    * @param timeElapsed - Specifies the time since the tween began in milliseconds or frames.
	    * @param duration - Specifies the duration of the tween, in milliseconds or frames.
  	    * @param frequency - The frequency of the sine wave in hertz. Higher is faster. 1/frequency = wavelength
	    * @return Number - between 0.0 and 1.0 based on the time and frequency. Returns 1.0 if duration is elapsed and snapping is turned on.
	    */  
		public static function sawtooth(timeElapsed:Number, duration:Number, frequency:Number=NaN):Number {
			if (isNaN(frequency)) { frequency = defaultFrequency; }
			if (timeElapsed >= duration && snapping == true) { return 1.0 }
			frequency /= 1000;
			return timeElapsed * frequency % 1.0;
		}	
		
		/**
	    * Oscillates between 0 and 1.0 (starting with 0 and ending with 1.0) for as long as the duration lasts 
	    * in linear motion.
	    * The position will be based on the timeElapsed and frequency.
	    *
	    * @param timeElapsed - Specifies the time since the tween began in milliseconds or frames.
	    * @param duration - Specifies the duration of the tween, in milliseconds or frames.
  	    * @param frequency - The frequency of the sine wave in hertz. Higher is faster. 1/frequency = wavelength
	    * @return Number - between 0.0 and 1.0 based on the time and frequency. Returns 1.0 if duration is elapsed and snapping is turned on.
	    */  
		public static function triangle(timeElapsed:Number, duration:Number, frequency:Number = NaN):Number {
			if (isNaN(frequency)) { frequency = defaultFrequency; }
			if (timeElapsed >= duration && snapping == true) { return 1.0 }
			frequency /= 1000;
			var position:Number = timeElapsed * frequency;
			if (position % 2.0 >= 1.0) { return 1.0 - (position % 1.0); }
			return position % 1.0;
		}	

		/**
	    * Oscillates between 0 and 1.0 (starting with 0 and ending with 1.0) for as long as the duration lasts 
	    * in square or pulse wave motion. The subject will appear to alternate between the start and end points.
	    * The position will be based on the timeElapsed and frequency.
	    *
	    * @param timeElapsed - Specifies the time since the tween began in milliseconds or frames.
	    * @param duration - Specifies the duration of the tween, in milliseconds or frames.
  	    * @param frequency - The frequency of the sine wave in hertz. Higher is faster. 1/frequency = wavelength
  	    * @param pulseWidth - The width of the upside of the square wave as a percentage. 0.5 is half up, half down. 0.0 is all down, 1.0 is all up.
	    * @return Number - between 0.0 and 1.0 based on the time and frequency. Returns 1.0 if duration is elapsed and snapping is turned on.
	    */  
		public static function pulse(timeElapsed:Number, duration:Number, frequency:Number = NaN, pulseWidth:Number= NaN):Number {
			if (isNaN(frequency)) { frequency = defaultFrequency; }
			if (isNaN(pulseWidth)) { pulseWidth = defaultPulseWidth; }
			frequency /= 1000;
			if (timeElapsed >= duration && snapping == true) { return 1.0 }
			if ((timeElapsed * frequency % 1.0) >= pulseWidth) { return 1.0 }
			return 0;
		}	
	}
}