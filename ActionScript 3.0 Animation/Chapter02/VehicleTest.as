package
{
	import com.foed.Vector2D;
	import com.foed.Vehicle;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	
	public class VehicleTest extends Sprite
	{
		private var _vehicle:Vehicle;
		
		public function VehicleTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_vehicle = new Vehicle();
			addChild(_vehicle);
			
			_vehicle.position = new Vector2D(100, 100);
			
			_vehicle.velocity.length = 5;
			_vehicle.velocity.angle = Math.PI / 4;
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_vehicle.update();
		}
	}
}