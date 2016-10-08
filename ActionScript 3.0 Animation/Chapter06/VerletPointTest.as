package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.Rectangle;

	public class VerletPointTest extends Sprite
	{
		private var _point:VerletPoint;
		private var _stageRect:Rectangle;
		
		public function VerletPointTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			_stageRect = new Rectangle(0, 0, stage.stageWidth, stage.stageHeight);
			
			_point = new VerletPoint(100, 100);
			_point.x += 5;
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_point.y += .5;
			_point.constrain(_stageRect);
			_point.update();
			
			graphics.clear();
			_point.render(graphics);
		}
	}
}
