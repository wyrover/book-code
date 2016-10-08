package
{
	import flash.display.Graphics;
	import flash.geom.Rectangle;
	
	public class VerletPoint
	{
		public var x:Number;
		public var y:Number;
		
		private var _oldX:Number;
		private var _oldY:Number;
		
		
		public function VerletPoint(x:Number, y:Number)
		{
			setPosition(x, y);
		}
		
		public function update():void
		{
			var tempX:Number = x;
			var tempY:Number = y;
			x += vx;
			y += vy;
			_oldX = tempX;
			_oldY = tempY;
		}
		
		public function setPosition(x:Number, y:Number):void
		{
			this.x = _oldX = x;
			this.y = _oldY = y;
		}
		
		public function constrain(rect:Rectangle):void
		{
			x = Math.max(rect.left, Math.min(rect.right, x));
			y = Math.max(rect.top, Math.min(rect.bottom, y));
		}
		
		public function set vx(value:Number):void
		{
			_oldX = x - value;
		}
		public function get vx():Number
		{
			return x - _oldX;
		}
		
		public function set vy(value:Number):void
		{
			_oldY = y - value;
		}
		public function get vy():Number
		{
			return y - _oldY;
		}
		
		public function render(g:Graphics):void
		{
			g.beginFill(0);
			g.drawCircle(x, y, 4);
			g.endFill();
		}

	}
}