package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.Rectangle;

	public class Square extends Sprite
	{
		private var _points:Array;
		private var _sticks:Array;
		private var _stageRect:Rectangle;
		
		public function Square()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			_stageRect = new Rectangle(0, 0, stage.stageWidth, stage.stageHeight);
			 
			_points = new Array();
			_sticks = new Array();
			 
			var pointA:VerletPoint = makePoint(100, 100);
			pointA.vx = 10;
			var pointB:VerletPoint = makePoint(200, 100);
			var pointC:VerletPoint = makePoint(200, 200);
			var pointD:VerletPoint = makePoint(100, 200);
			
			makeStick(pointA, pointB);
			makeStick(pointB, pointC);
			makeStick(pointC, pointD);
			makeStick(pointD, pointA);
			makeStick(pointA, pointC);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			updatePoints();
			
			for(var i:int = 0; i < 1; i++)
			{
				constrainPoints();
				updateSticks();
			}
			
			graphics.clear();
			renderPoints();
			renderSticks();
		}
		
		private function makePoint(xpos:Number, ypos:Number):VerletPoint
		{
			var point:VerletPoint = new VerletPoint(xpos, ypos);
			_points.push(point);
			return point;
		}
		
		private function makeStick(pointA:VerletPoint, pointB:VerletPoint, length:Number = -1):VerletStick
		{
			var stick:VerletStick = new VerletStick(pointA, pointB, length);
			_sticks.push(stick);
			return stick;
		}
		
		private function updatePoints():void
		{
			for(var i:int = 0; i < _points.length; i++)
			{
				var point:VerletPoint = _points[i] as VerletPoint;
				point.y += .5;
				point.update();
			}
		}
		
		private function constrainPoints():void
		{
			for(var i:int = 0; i < _points.length; i++)
			{
				var point:VerletPoint = _points[i] as VerletPoint;
				point.constrain(_stageRect);
			}
		}
		
		private function updateSticks():void
		{
			for(var i:int = 0; i < _sticks.length; i++)
			{
				var stick:VerletStick = _sticks[i] as VerletStick;
				stick.update();
			} 
		}
		
		private function renderPoints():void
		{
			for(var i:int = 0; i < _points.length; i++)
			{
				var point:VerletPoint = _points[i] as VerletPoint;
				point.render(graphics);
			}
		}
		
		private function renderSticks():void
		{
			for(var i:int = 0; i < _sticks.length; i++)
			{
				var stick:VerletStick = _sticks[i] as VerletStick;
				stick.render(graphics);
			}
		}
	}
}
