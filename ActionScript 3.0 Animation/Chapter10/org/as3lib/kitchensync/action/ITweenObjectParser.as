package org.as3lib.kitchensync.action
{
	/**
	 * An algorithm that parses data from an object into tween data.
	 * 
	 * @author Mims Wright
	 * @since 1.6
	 */
	public interface ITweenObjectParser
	{
		/** returns an action based on an object you provide. */
		function parseObject(object:Object):ITween;
		
		/** Returns a list of the reserved words for this parser */
		function get keywords():Array;
	}
}