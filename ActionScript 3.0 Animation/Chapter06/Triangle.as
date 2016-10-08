package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.Rectangle;

	public class Triangle extends Sprite
	{
		private var _pointA:VerletPoint;
		private var _pointB:VerletPoint;
		private var _pointC:VerletPoint;
		private var _stickA:VerletStick;
		private var _stickB:VerletStick;
		private var _stickC:VerletStick;
		private var _stageRect:Rectangle;
		
		public function Triangle()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			_stageRect = new Rectangle(0, 0, stage.stageWidth, stage.stageHeight);
			
			_pointA = new VerletPoint(100, 100);
			_pointB = new VerletPoint(200, 100);
			_pointC = new VerletPoint(150, 200);
			
			_stickA = new VerletStick(_pointA, _pointB);
			_stickB = new VerletStick(_pointB, _pointC);
			_stickC = new VerletStick(_pointC, _pointA);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_pointA.y += .5;
			_pointA.update();
			
			_pointB.y += .5;
			_pointB.update();
			
			_pointC.y += .5;
			_pointC.update();
			
			for(var i:int = 0; i < 1; i++)
			{
				_pointA.constrain(_stageRect);
				_pointB.constrain(_stageRect);
				_pointC.constrain(_stageRect);
				_stickA.update();
				_stickB.update();
				_stickC.update();
			}
			
			graphics.clear();
			_pointA.render(graphics);
			_pointB.render(graphics);
			_pointC.render(graphics);
			_stickA.render(graphics);
			_stickB.render(graphics);
			_stickC.render(graphics);
		}
	}
}
