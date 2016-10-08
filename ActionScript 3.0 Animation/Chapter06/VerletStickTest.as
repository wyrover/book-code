package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.Rectangle;

	public class VerletStickTest extends Sprite
	{
		private var _pointA:VerletPoint;
		private var _pointB:VerletPoint;
		private var _stick:VerletStick;
		private var _stageRect:Rectangle;
		
		public function VerletStickTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			_stageRect = new Rectangle(0, 0, stage.stageWidth, stage.stageHeight);
			
			_pointA = new VerletPoint(100, 100);
			_pointB = new VerletPoint(105, 200);
			
			_stick = new VerletStick(_pointA, _pointB);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_pointA.y += .5;
			_pointA.update();
			
			_pointB.y += .5;
			_pointB.update();
			
			for(var i:int = 0; i < 5; i++)
			{
				_pointA.constrain(_stageRect);
				_pointB.constrain(_stageRect);
				_stick.update();
			}
			
			graphics.clear();
			_pointA.render(graphics);
			_pointB.render(graphics);
			_stick.render(graphics);
		}
	}
}
