package
{
	import flash.display.Sprite;

	public class Snow extends Sprite
	{
		public var vx:Number;
		public var vy:Number;
		
		public function Snow()
		{
			graphics.beginFill(0xffffff, .7);
			graphics.drawCircle(0, 0, 2);
			graphics.endFill();
			
			vx = 0;
			vy = 1;
		}
		
		public function update():void
		{
			vx += Math.random() * .2 - .1;
			vx *= .95;
			x += vx;
			y += vy;
		}
	}
}