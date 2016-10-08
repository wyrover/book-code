package
{
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.Event;

	[SWF(width=800, height=800, backgroundColor=0xffffff)]
	public class Position3D extends Sprite
	{
		private var _shape:Shape;
		private var _n:Number = 0;
		
		public function Position3D()
		{
			_shape = new Shape();
			_shape.graphics.beginFill(0x00ff00);
			_shape.graphics.drawRect(-100, -100, 200, 200);
			_shape.graphics.endFill();
			addChild(_shape);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_shape.x = mouseX;
			_shape.y = mouseY;
			_shape.z = 10000 + Math.sin(_n += .1) * 10000;
		}
	}
}