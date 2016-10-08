package org.as3lib.kitchensync.easing
{
	/**
	 * <p>Back produces values that go beyond the usual bounds of 0.0 to 1.0 by a specified amount (overshoot).
	 * Visually, the subject would appear to move behind the starting point or past the ending point before returning
	 * to the target position.</p>
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
	public class Back
	{
		/** The default amount of overshoot if the user doesn't specify. Rougly 10% */
		public static const DEFAULT_OVERSHOOT:Number = 1.70158;
		/** Constant used to calculate easeInOut() */
		private static const OVERSHOOT_GROWTH:Number = 1.525;
		
	    /**
	    * easeIn starts out slow and increases in speed towards the end of the duration.
	    * The values dip below the starting point (0.0) before increasing towards the end point (1.0)
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @param overshoot The ammount to go over past the target value. The higher the number, the farther it will go.
	    * @return percentage complete - between 0.0 and 1.0 but with the overshoot, it may extend below 0.0 or above 1.0
	    */  
		public static function easeIn (timeElapsed:Number, duration:Number, overshoot:Number = DEFAULT_OVERSHOOT):Number 
		{
			return (timeElapsed/=duration)*timeElapsed*((overshoot+1)*timeElapsed - overshoot);
		}	
	
	    /**
	    * easeOut starts out fast and decreases in speed towards the end of the duration.
	    * The values overshoot the end point (1.0) before returning back to it.
	    * 
	    * @param timeElapsed Specifies the time since the tween began in milliseconds or frames.
	    * @param duration Specifies the duration of the tween, in milliseconds or frames.
	    * @param overshoot The ammount to go over past the target value. The higher the number, the farther it will go.
	    * @return percentage complete - between 0.0 and 1.0 but with the overshoot, it may extend below 0.0 or above 1.0
	    */  
		public static function easeOut (timeElapsed:Number, duration:Number, overshoot:Number = DEFAULT_OVERSHOOT):Number 
		{
			return ((timeElapsed=timeElapsed/duration-1)*timeElapsed*((overshoot+1)*timeElapsed + overshoot) + 1);
		}
	
	    /**
	    * easeInOut combines the motion of easeIn and easeOut by starting out slow and increasing in speed 
	    * towards the middle of the duration, then slowing down again towards the end.
	    * The values dip below the starting point (0.0) then overshoot the end point (1.0) before returning back to it.
	    * 
	    * @param timeElapsed Specifies the time since the tween began in milliseconds or frames.
	    * @param duration Specifies the duration of the tween, in milliseconds or frames.
	    * @param overshoot The ammount to go over past the target value. The higher the number, the farther it will go.
	    * @return percentage complete - between 0.0 and 1.0 but with the overshoot, it may extend below 0.0 or above 1.0
	    */  
		public static function easeInOut (timeElapsed:Number, duration:Number, overshoot:Number = DEFAULT_OVERSHOOT):Number 
		{
			if ((timeElapsed/=duration/2) < 1) {
				return 0.5*(timeElapsed*timeElapsed*(((overshoot*=OVERSHOOT_GROWTH)+1)*timeElapsed - overshoot));
			}
			return 0.5*((timeElapsed-=2)*timeElapsed*(((overshoot*=OVERSHOOT_GROWTH)+1)*timeElapsed + overshoot) + 2);
		}	
		
		/**
		* easeOutIn combines the motion of easeIn and easeOut by starting out decelerating 
		* towards the middle of the duration, then accelerating end.
		*  
	    * @param timeElapsed Specifies the time since the tween began in milliseconds or frames.
	    * @param duration Specifies the duration of the tween, in milliseconds or frames.
	    * @param overshoot The ammount to go over past the target value. The higher the number, the farther it will go.
	    * @return percentage complete - between 0.0 and 1.0 but with the overshoot, it may extend below 0.0 or above 1.0
		*/ 
		public static function easeOutIn (timeElapsed:Number, duration:Number, overshoot:Number = DEFAULT_OVERSHOOT):Number {
			if (timeElapsed < duration/2) return easeOut(timeElapsed*2, duration, overshoot)/2;
			return easeIn((timeElapsed*2)-duration, duration, overshoot)/2 + 0.5;
		}
	}
}