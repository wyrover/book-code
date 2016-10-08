package {
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.PerspectiveProjection;
	import flash.geom.Point;
	
	public class Test3D extends Sprite
	{
		private var _shape:Shape;
		
		public function Test3D()
		{
			stage.addEventListener(Event.RESIZE, onResize)
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_shape = new Shape();
			_shape.graphics.beginFill(0xff0000);
			_shape.graphics.drawRect(-100, -100, 200, 200);
			_shape.x = stage.stageWidth / 2;
			_shape.y = stage.stageHeight / 2;
			addChild(_shape);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onResize(event:Event):void
		{
			root.transform.perspectiveProjection.projectionCenter = new Point(stage.stageWidth / 2, stage.stageHeight / 2);
			if(_shape != null)
			{
				_shape.x = stage.stageWidth / 2;
				_shape.y = stage.stageHeight / 2;
			}
		}
		
		private function onEnterFrame(event:Event):void
		{
			_shape.rotationY += 2;
			_shape.x = stage.stageWidth / 2;
			_shape.y = stage.stageHeight / 2;
		}
	}
}
