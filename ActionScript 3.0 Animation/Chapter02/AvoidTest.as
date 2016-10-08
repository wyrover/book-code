package
{
	import com.foed.Circle;
	import com.foed.SteeredVehicle;
	import com.foed.Vector2D;
	import com.foed.Vehicle;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;

	public class AvoidTest extends Sprite
	{
		private var _vehicle:SteeredVehicle;
		private var _circles:Array;
		private var _numCircles:int = 10;
		
		public function AvoidTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_vehicle = new SteeredVehicle();
			_vehicle.edgeBehavior = Vehicle.BOUNCE;
			addChild(_vehicle);
			
			_circles = new Array();
			for(var i:int = 0; i < _numCircles; i++)
			{
				var circle:Circle = new Circle(Math.random() * 50 + 50);
				circle.x = Math.random() * stage.stageWidth;
				circle.y = Math.random() * stage.stageHeight;
				addChild(circle);
				_circles.push(circle);
			}
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_vehicle.wander();
			_vehicle.avoid(_circles);
			_vehicle.update();
		}
	}
}