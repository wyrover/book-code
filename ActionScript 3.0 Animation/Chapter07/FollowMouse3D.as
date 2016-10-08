package
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Vector3D;

	[SWF(width=800, height=800, backgroundColor = 0xffffff)]
	public class FollowMouse3D extends Sprite
	{
		private var _sprite:Sprite;
		private var _angleX:Number = 0;
		private var _angleY:Number = 0;
		private var _angleZ:Number = 0;
		
		public function FollowMouse3D()
		{
			_sprite = new Sprite();
			_sprite.x = 400;
			_sprite.y = 400;
			_sprite.z = 200;
			_sprite.graphics.beginFill(0xff0000);
			_sprite.graphics.moveTo(0, 50);
			_sprite.graphics.lineTo(-25, 25);
			_sprite.graphics.lineTo(-10, 25);
			_sprite.graphics.lineTo(-10, -50);
			_sprite.graphics.lineTo(10, -50);
			_sprite.graphics.lineTo(10, 25);
			_sprite.graphics.lineTo(25, 25);
			_sprite.graphics.lineTo(0, 50);
			_sprite.graphics.endFill();
			addChild(_sprite);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_sprite.x = 400 + Math.sin(_angleX += .11) * 200;
			_sprite.y = 400 + Math.sin(_angleY += .07) * 200;
			_sprite.z = Math.sin(_angleZ += .09) * 200;
			_sprite.transform.matrix3D.pointAt(new Vector3D(mouseX, mouseY, 0));
		}
	}
}