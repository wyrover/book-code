package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.Point;
	import flash.utils.getTimer;

	public class RK4 extends Sprite
	{
		private var _ball:Sprite;
		private var _position:Point;
		private var _velocity:Point;
		private var _gravity:Number = 32;
		private var _bounce:Number = -0.6;
		private var _oldTime:int;
		private var _pixelsPerFoot:Number = 10;
		
		
		public function RK4()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_ball = new Sprite();
			_ball.graphics.beginFill(0xff0000);
			_ball.graphics.drawCircle(0, 0, 20);
			_ball.graphics.endFill();
			_ball.x = 50;
			_ball.y = 50;
			addChild(_ball);
			
			_velocity = new Point(10, 0);
			_position = new Point(_ball.x / _pixelsPerFoot, _ball.y / _pixelsPerFoot);
			
			_oldTime = getTimer();
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			var time:int = getTimer();
			var elapsed:Number = (time - _oldTime) / 1000;
			_oldTime = time;
			
			var accel1:Point = acceleration(_position, _velocity);
			
			var position2:Point = new Point();
			position2.x = _position.x + _velocity.x / 2 * elapsed;
			position2.y = _position.y + _velocity.y / 2 * elapsed;

			var velocity2:Point = new Point();
			velocity2.x = _velocity.x + accel1.x / 2 * elapsed;
			velocity2.y = _velocity.y + accel1.x / 2 * elapsed;
			
			var accel2:Point = acceleration(position2, velocity2);
			
			var position3:Point = new Point();
			position3.x = _position.x + velocity2.x / 2 * elapsed;
			position3.y = _position.y + velocity2.y / 2 * elapsed;
			
			var velocity3:Point = new Point();
			velocity3.x = _velocity.x + accel2.x / 2 * elapsed;
			velocity3.y = _velocity.y + accel2.y / 2 * elapsed;
			
			var accel3:Point = acceleration(position3, velocity3);
			
			var position4:Point = new Point();
			position4.x = _position.x + velocity3.x * elapsed;
			position4.y = _position.y + velocity3.y * elapsed;
			
			var velocity4:Point = new Point();
			velocity4.x = _velocity.x + accel3.x * elapsed;
			velocity4.y = _velocity.y + accel3.y * elapsed;
			
			var accel4:Point = acceleration(position4, velocity4);
			
			_position.x += (_velocity.x + 2 * velocity2.x + 2 * velocity3.x + velocity4.x) / 6 * elapsed;
			_position.y += (_velocity.y + 2 * velocity2.y + 2 * velocity3.y + velocity4.y) / 6 * elapsed;
			
			_velocity.x += (accel1.x + 2 * accel2.x + 2 * accel3.x + accel4.x) / 6 * elapsed;
			_velocity.y += (accel1.y + 2 * accel2.y + 2 * accel3.y + accel4.y) / 6 * elapsed;
			
			if(_position.y > (stage.stageHeight - 20) / _pixelsPerFoot)
			{
				_position.y = (stage.stageHeight - 20) / _pixelsPerFoot;
				_velocity.y *= _bounce;
			}
			if(_position.x > (stage.stageWidth - 20) / _pixelsPerFoot)
			{
				_position.x = (stage.stageWidth - 20) / _pixelsPerFoot;
				_velocity.x *= _bounce
			}
			else if(_position.x < 20 / _pixelsPerFoot)
			{
				_position.x = 20 / _pixelsPerFoot;
				_velocity.x *= _bounce;
			}
			
			_ball.x = _position.x * _pixelsPerFoot;
			_ball.y = _position.y * _pixelsPerFoot;
		}
		
		private function acceleration(p:Point, v:Point):Point
		{
			return new Point(0, _gravity);
		}
	}
}
