package
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.Point;
	import flash.geom.Vector3D;

	[SWF(width=800, height=800)]
	public class GlobalLocal extends Sprite
	{
		private var _sprite:Sprite;
		private var _tracker:Sprite;
		private var _angle:Number = 0;
		
		
		public function GlobalLocal()
		{
			_sprite = new Sprite();
			_sprite.graphics.lineStyle(5);
			_sprite.graphics.drawRect(-200, -200, 400, 400);
			_sprite.x = 400;
			_sprite.y = 400;
			addChild(_sprite);
			
			_tracker = new Sprite();
			_tracker.graphics.lineStyle(2, 0xff0000);
			_tracker.graphics.drawCircle(0, 0, 20);
			_sprite.addChild(_tracker);
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_sprite.rotationX += 1;
			_sprite.rotationY+= 1.2;
			_sprite.rotationZ += .5;
			_sprite.x = 400 + Math.cos(_angle) * 100;
			_sprite.y = 400 + Math.sin(_angle) * 100;
			_sprite.z = 200 + Math.cos(_angle * .8) * 400;
			_angle += .05;
//			var p:Vector3D = _sprite.globalToLocal3D(new Point(mouseX, mouseY));
//			_tracker.x = p.x;
//			_tracker.y = p.y;
			_tracker.x = _sprite.mouseX;
			_tracker.y = _sprite.mouseY;
		}
		
	}
}