package com.friendsofed.isometric
{
	public class BasicIsoTile extends DrawnIsoTile
	{
		public function BasicIsoTile(size:Number, color:uint = 0xffffff, height:Number = 0)
		{
			super(size, color, height);
		}
		
		override protected function draw():void
		{
			graphics.clear();
			graphics.beginFill(_color, .3);
			graphics.lineStyle(0, 0, .5);
			graphics.moveTo(-size, 0);
			graphics.lineTo(0, -size * .5);
			graphics.lineTo(size, 0);
			graphics.lineTo(0, size * .5);
			graphics.lineTo(-size, 0);
		}
		
	}
}