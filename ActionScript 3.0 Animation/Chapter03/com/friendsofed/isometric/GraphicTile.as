package com.friendsofed.isometric
{
	import flash.display.DisplayObject;
	
	public class GraphicTile extends IsoObject
	{
		public function GraphicTile(size:Number, classRef:Class, xoffset:Number, yoffset:Number):void
		{
			super(size);
			
			var gfx:DisplayObject = new classRef() as DisplayObject;
			gfx.x = -xoffset;
			gfx.y = -yoffset;
			addChild(gfx);
		}
	}
}