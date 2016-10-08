package org.as3lib.kitchensync.easing
{
	public class Elastic
	{	
		private static const TWICE_PI:Number = Math.PI * 2;
		private static const DEFAULT_PERIOD:Number = 0.3;
	    
	    /**
	    * easeIn starts out slow and increases in speed towards the end of the duration.
	    * The values wobble around the starting point before going towards the end point.
	    * 
	    * @author modified by Mims H. Wright, 2007
	 	* @author (c) 2003 Robert Penner, all rights reserved. - This work is subject to the terms in http://www.robertpenner.com/easing_terms_of_use.html
	 	* @see http://www.robertpenner.com/easing_terms_of_use.html
	 	* @see http://www.robertpenner.com/easing/
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @param amplitude The aplitude of the sine wave. Low numbers are less extreme than high numbers.
	    * @param period The period of the sine wave. Low numbers are wobbly, high numbers are smooth.
	    * @return percentage complete - between 0.0 and 1.0 but it may extend below 0.0 or above 1.0
	    */  
		public static function easeIn(timeElapsed:Number, duration:Number, amplitude:Number = 0, period:Number = 0):Number
		{
			if (timeElapsed <= 0) {
				return 0;
			} 
			if ((timeElapsed /= duration) >= 1) {
				return 1;
			}
			if (period == 0) {
				period = duration * DEFAULT_PERIOD;
			} 
			var decay:Number;
			if (amplitude < 1) { 
				amplitude = 1; 
				decay = period / 4; 
			} else {
				decay= period / TWICE_PI * Math.asin (1/amplitude);
			}
			return -(amplitude*Math.pow(2,10*(timeElapsed-=1)) * Math.sin( (timeElapsed*duration-decay)*TWICE_PI/period )); 
		}
	
	
	    /**
	    * easeOut starts out fast and decreases in speed towards the end of the duration.
	    * The values wobble around the endpoint after reaching it.
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @param amplitude The aplitude of the sine wave. Low numbers are less extreme than high numbers.
	    * @param period The period of the sine wave. Low numbers are wobbly, high numbers are smooth.
	    * @return percentage complete - between 0.0 and 1.0 but it may extend below 0.0 or above 1.0
	    */  
		public static function easeOut(timeElapsed:Number, duration:Number, amplitude:Number = 0, period:Number = 0):Number
		{
			if (timeElapsed <= 0) {
				return 0;  
			}
			if ((timeElapsed /= duration) >= 1) {
				return 1;
			}
			if (!period) {
				period = duration * DEFAULT_PERIOD;
			}
			var decay:Number;
			if (amplitude < 1) { 
				amplitude = 1; 
				decay = period / 4; 
			} else {
			   decay= period / TWICE_PI * Math.asin (1/amplitude);
			} 
			return (amplitude*Math.pow(2,-10*timeElapsed) * Math.sin( (timeElapsed*duration-decay)*TWICE_PI/period ) + 1);
		}
	
	    /**
	  	* easeInOut combines the motion of easeIn and easeOut by starting out slow and increasing in speed 
	    * towards the middle of the duration, then slowing down again towards the end.
	    * The values wobble around the starting point (0.0) then go to the the end point (1.0) and wobble there as well.
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @param amplitude The aplitude of the sine wave. Low numbers are less extreme than high numbers.
	    * @param period The period of the sine wave. Low numbers are wobbly, high numbers are smooth.
	    * @return percentage complete - between 0.0 and 1.0 but it may extend below 0.0 or above 1.0
	    */  
		public static function easeInOut(timeElapsed:Number, duration:Number, amplitude:Number = 0, period:Number = 0):Number
		{
			if (timeElapsed <= 0) {
				return 0;
			}  
			if ((timeElapsed /= duration/2) >= 2) {
				return 1;
			}  
			if (!period) {
				period = duration * (DEFAULT_PERIOD * 1.5);
			}
			var decay:Number;
			if (amplitude < 1) { 
				amplitude = 1; 
				decay = period / 4; 
			} else {
				decay = period / TWICE_PI * Math.asin (1/amplitude);
			}
			if (timeElapsed < 1) {
				return -0.5*(amplitude*Math.pow(2,10*(timeElapsed-=1)) * Math.sin((timeElapsed*duration-decay)*TWICE_PI/period ));
			}
			return 0.5*(amplitude*Math.pow(2,-10*(timeElapsed-=1)) * Math.sin((timeElapsed*duration-decay)*TWICE_PI/period )) + 1;
		}
	
		/**
		* easeOutIn combines the motion of easeIn and easeOut by starting out decelerating 
		* towards the middle of the duration, then accelerating end.
		*
		* @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @param amplitude The aplitude of the sine wave. Low numbers are less extreme than high numbers.
	    * @param period The period of the sine wave. Low numbers are wobbly, high numbers are smooth.
	    * @return percentage complete - between 0.0 and 1.0 but it may extend below 0.0 or above 1.0
	    */  
		public static function easeOutIn (timeElapsed:Number, duration:Number, amplitude:Number = 0, period:Number = 0):Number {
			if (timeElapsed < duration/2) return easeOut(timeElapsed*2, duration, amplitude, period)/2;
			return easeIn((timeElapsed*2)-duration, duration, amplitude, period)/2 + 0.5;
		}
	
	}
}