package
{
	import com.foed.SteeredVehicle;
	import com.foed.Vector2D;
	import com.foed.Vehicle;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;

	public class FlockTest extends Sprite
	{
		private var _vehicles:Array;
		private var _numVehicles:int = 30;
		
		public function FlockTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_vehicles = new Array();
			for(var i:int = 0; i < _numVehicles; i++)
			{
				var vehicle:SteeredVehicle = new SteeredVehicle();
				vehicle.position = new Vector2D(Math.random() * stage.stageWidth, Math.random() * stage.stageHeight);
				vehicle.velocity = new Vector2D(Math.random() * 20 - 10, Math.random() * 20 - 10);
				vehicle.edgeBehavior = Vehicle.BOUNCE;
				_vehicles.push(vehicle);
				addChild(vehicle);
			}
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			for(var i:int = 0; i < _numVehicles; i++)
			{
				_vehicles[i].flock(_vehicles);
				_vehicles[i].update();
			}
		}
	}
}