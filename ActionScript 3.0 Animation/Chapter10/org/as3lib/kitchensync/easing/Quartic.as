package org.as3lib.kitchensync.easing
{
	/**
	 * <p>Quartic produces values that would cause a moving subject to move with moderately fast accelleration.</p>
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
	 * @see Quadratic
	 * @see Cubic
	 * @see Quintic
	 * @see Sextic
	 */
	public class Quartic
	{	
	    /**
	    * easeIn starts out slow and increases in speed towards the end of the duration.
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @return percentage complete - between 0.0 and 1.0
	    */ 
		public static function easeIn(timeElapsed:Number, duration:Number):Number
		{
			return (timeElapsed/=duration)*timeElapsed*timeElapsed*timeElapsed;
		}
				
	    /**
	    * easeOut starts out fast and decreases in speed towards the end of the duration.
	    * 
	    * @param timeElapsed Specifies the time since the tween began in milliseconds or frames.
	    * @param duration Specifies the duration of the tween, in milliseconds or frames.
	    * @return percentage complete - between 0.0 and 1.0
	    */  
		public static function easeOut(timeElapsed:Number, duration:Number):Number
		{
			return  -1 * ((timeElapsed=timeElapsed/duration-1)*timeElapsed*timeElapsed*timeElapsed - 1);
		}
	
	    /**
		* easeInOut combines the motion of easeIn and easeOut by starting out slow and increasing in speed 
		* towards the middle of the duration, then slowing down again towards the end.
		*  
		* @param timeElapsed Specifies the time since the tween began in milliseconds or frames.
		* @param duration Specifies the duration of the tween, in milliseconds or frames.
		* @param overshoot The ammount to go over past the target value. The higher the number, the farther it will go.
		* @return percentage complete - between 0.0 and 1.0
		*/  
		public static function easeInOut(timeElapsed:Number, duration:Number):Number
		{
			if ((timeElapsed/=duration/2) < 1) return 0.5*timeElapsed*timeElapsed*timeElapsed*timeElapsed;
			return -0.5*((timeElapsed-=2)*timeElapsed*timeElapsed*timeElapsed - 2);
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
			if (timeElapsed < duration/2) return easeOut(timeElapsed*2, duration)/2;
			return easeIn((timeElapsed*2)-duration, duration)/2 + 0.5;
		}	
	}
}