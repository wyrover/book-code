package org.as3lib.kitchensync.easing
{
	/**
	 * <p>Linear produces values that increase in a linear fashion without any accelleration. It simply finds
	 * the percentage complete from the time elapsed to the duration. Since there is no variation in the values
	 * produced, there is no need for multiple easing functions and only <code>ease()</code> is provided.
	 * Visually, this class produces values that cause a subject to move steadily from one point to another.</p>
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
	public class Linear
	{
		/**
	    * Computes the percentage finished between the time elapsed and the duration. No transformations are made.
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @return percentage complete - between 0.0 and 1.0
	    */ 
		public static function ease(timeElapsed:Number, duration:Number):Number{
			return timeElapsed / duration;
		}
	}
}