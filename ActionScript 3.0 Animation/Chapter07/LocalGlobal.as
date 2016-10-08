package
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.Point;
	import flash.geom.Vector3D;

	[SWF(width=800, height=800)]
	public class LocalGlobal extends Sprite
	{
		private var _sprite:Sprite;
		private var _tracker:Sprite;
		private var _angle:Number = 0;
		
		
		public function LocalGlobal()
		{
			_sprite = new Sprite();
			_sprite.graphics.lineStyle(10);
			_sprite.graphics.lineTo(200, 0);
			_sprite.graphics.drawCircle(200, 0, 10);
			_sprite.x = 400;
			_sprite.y = 400;
			addChild(_sprite);
			
			_tracker = new Sprite();
			_tracker.graphics.lineStyle(2, 0xff0000);
			_tracker.graphics.drawCircle(0, 0, 20);
			addChild(_tracker);
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
			var p:Point = _sprite.local3DToGlobal(new Vector3D(200, 0, 0));
			_tracker.x = p.x;
			_tracker.y = p.y;
		}
		
	}
}