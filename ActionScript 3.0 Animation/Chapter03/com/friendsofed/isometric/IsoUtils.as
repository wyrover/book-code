package com.friendsofed.isometric
{
	import flash.geom.Point;
	
	public class IsoUtils
	{
		// a more accurate version of 1.2247...
		public static const Y_CORRECT:Number = Math.cos(-Math.PI / 6) * Math.SQRT2;
		
		/**
		 * Converts a 3D point in isometric space to a 2D screen position.
		 * @arg pos the 3D point.
		 */
		public static function isoToScreen(pos:Point3D):Point
		{
			var screenX:Number = pos.x - pos.z;
			var screenY:Number = pos.y * Y_CORRECT + (pos.x + pos.z) * .5;
			return new Point(screenX, screenY);
		}
		
		/**
		 * Converts a 2D screen position to a 3D point in isometric space, assuming y = 0.
		 * @arg point the 2D point.
		 */
		public static function screenToIso(point:Point):Point3D
		{
			var xpos:Number = point.y + point.x * .5;
			var ypos:Number = 0;
			var zpos:Number = point.y - point.x * .5;
			return new Point3D(xpos, ypos, zpos);
		}
		
	}
}