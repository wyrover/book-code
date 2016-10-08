package
{
	import com.foed.SteeredVehicle;
	import com.foed.Vector2D;
	import com.foed.Vehicle;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;

	public class SeekFleeTest1 extends Sprite
	{
		private var _seeker:SteeredVehicle;
		private var _fleer:SteeredVehicle;
		
		public function SeekFleeTest1()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_seeker = new SteeredVehicle();
			_seeker.position = new Vector2D(200, 200);
			_seeker.edgeBehavior = Vehicle.BOUNCE;
			addChild(_seeker);
			
			_fleer = new SteeredVehicle();
			_fleer.position = new Vector2D(400, 300);
			_fleer.edgeBehavior = Vehicle.BOUNCE;
			addChild(_fleer);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_seeker.seek(_fleer.position);
			_fleer.flee(_seeker.position);
			_seeker.update();
			_fleer.update();
		}
	}
}