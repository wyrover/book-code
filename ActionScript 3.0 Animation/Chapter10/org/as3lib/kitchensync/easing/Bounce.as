package org.as3lib.kitchensync.easing
{
	/**
	 * <p>Bounce produces values that would cause a moving subject to bounce against the starting or ending 
	 * points like a rubber ball.</p>
	 * <p>The easing classes provide static methods for interpolating the change between two values over time.
	 * Each class handles the interpolation, or easing, differently. Each class typically contains three methods
	 *  - <code>easeIn()</code>, <code>easeOut()</code> and <code>easeInOut()</code> - which vary the rate of change
	 *  of the values. Most of the easing functions produce values as a percentage - a number between 0.0 and 1.0</p>
	 * 
 	 * @author modified by Mims H. Wright, 2007
	 * @author (c) 2003 Robert Penner, all rights reserved. - This work is subject to the terms in http://www.robertpenner.com/easing_terms_of_use.html
	 * @see http://www.robertpenner.com/easing_terms_of_use.html
	 * @see http://www.robertpenner.com/easing/
	 * @see EasingUtil
	 */
	public class Bounce
	{
		/**
		 * The constants defined in this class are used to control the bouncing algorithm
		 */
		private static const BOUNCE_CONST:Number   = 7.5625;
		private static const THRESHOLDx1:Number    = 0.363636;
		private static const THRESHOLDx15:Number   = 0.545454;
		private static const THRESHOLDx2:Number    = 0.727272;
		private static const THRESHOLDx225:Number  = 0.818181;
		private static const THRESHOLDx25:Number   = 0.909090;
		private static const THRESHOLDx2625:Number = 0.954545;
		
		private static const BOUNCE1:Number = 0.75;
		private static const BOUNCE2:Number = 0.9375;
		private static const BOUNCE3:Number = 0.984375;
		
	    /**
	    * easeIn starts out slow and increases in speed towards the end of the duration.
	    * The bouncing will appear to increase in height.
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @return percentage complete - between 0.0 and 1.0
	    */ 
		public static function easeIn(timeElapsed:Number, duration:Number):Number
		{
			return 1 - easeOut(duration-timeElapsed, duration);
		}
	
	
	   /**
	    * easeOut starts out fast and decreases in speed towards the end of the duration.
	    * The bouncing will appear to decrease in height.
	    * 
	    * @param timeElapsed Specifies the time since the tween began in milliseconds or frames.
	    * @param duration Specifies the duration of the tween, in milliseconds or frames.
	    * @return percentage complete - between 0.0 and 1.0
	    */  
		public static function easeOut(timeElapsed:Number, duration:Number):Number
		{
			if ((timeElapsed/=duration) < THRESHOLDx1) {
				return (BOUNCE_CONST*timeElapsed*timeElapsed);
			} else if (timeElapsed < THRESHOLDx2) {
				return (BOUNCE_CONST*(timeElapsed-=THRESHOLDx15)*timeElapsed + BOUNCE1);
			} else if (timeElapsed < THRESHOLDx25) {
				return (BOUNCE_CONST*(timeElapsed-=THRESHOLDx225)*timeElapsed + BOUNCE2);
			}
			return (BOUNCE_CONST*(timeElapsed-=THRESHOLDx2625)*timeElapsed + BOUNCE3);
		}
	
	    /**
	    * easeInOut combines the motion of easeIn and easeOut by starting out slow and increasing in speed 
	    * towards the middle of the duration, then slowing down again towards the end.
		* The bouncing will appear to increase then decrease in height.
		*  
	    * @param timeElapsed Specifies the time since the tween began in milliseconds or frames.
	    * @param duration Specifies the duration of the tween, in milliseconds or frames.
	    * @param overshoot The ammount to go over past the target value. The higher the number, the farther it will go.
	    * @return percentage complete - between 0.0 and 1.0
	    */  
		public static function easeInOut(timeElapsed:Number, duration:Number):Number
		{
			if (timeElapsed < duration/2) {
				return easeIn (timeElapsed*2, duration)/2;
			}
			return easeOut (timeElapsed*2-duration, duration)/2 + 0.5;
		}
		
		/**
		* easeOutIn combines the motion of easeIn and easeOut by starting out decelerating 
		* towards the middle of the duration, then accelerating end.
		*  
		* @param timeElapsed Specifies the time since the tween began in milliseconds or frames.
		* @param duration Specifies the duration of the tween, in milliseconds or frames.
		* @param overshoot The ammount to go over past the target value. The higher the number, the farther it will go.
		* @return percentage complete - between 0.0 and 1.0
		*/ 
		public static function easeOutIn (timeElapsed:Number, duration:Number):Number {
			if (timeElapsed < duration/2) {
				return easeOut(timeElapsed*2, duration)/2;
			}
			return easeIn((timeElapsed*2)-duration, duration)/2 + 0.5;
		}
	
	}
}