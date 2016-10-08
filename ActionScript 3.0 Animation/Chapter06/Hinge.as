package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.Rectangle;

	public class Hinge extends Sprite
	{
		private var _points:Array;
		private var _sticks:Array;
		private var _stageRect:Rectangle;
		
		public function Hinge()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			_stageRect = new Rectangle(0, 0, stage.stageWidth, stage.stageHeight);
			 
			_points = new Array();
			_sticks = new Array();
			 
			// base
			var pointA:VerletPoint = makePoint(stage.stageWidth / 2, stage.stageHeight - 500);
			var pointB:VerletPoint = makePoint(0, stage.stageHeight);
			var pointC:VerletPoint = makePoint(stage.stageWidth, stage.stageHeight);
			
			// arm
			var pointD:VerletPoint = makePoint(stage.stageWidth / 2 + 350, stage.stageHeight - 500);
			
			// weight
			var pointE:VerletPoint = makePoint(stage.stageWidth / 2 + 360, stage.stageHeight - 510);
			var pointF:VerletPoint = makePoint(stage.stageWidth / 2 + 360, stage.stageHeight - 490);
			var pointG:VerletPoint = makePoint(stage.stageWidth / 2 + 370, stage.stageHeight - 500);
			
			// base
			makeStick(pointA, pointB);
			makeStick(pointB, pointC);
			makeStick(pointC, pointA);
			
			// arm
			makeStick(pointA, pointD);
			
			// weight
			makeStick(pointD, pointE);
			makeStick(pointD, pointF);
			makeStick(pointE, pointF);
			makeStick(pointE, pointG);
			makeStick(pointF, pointG);
			
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
