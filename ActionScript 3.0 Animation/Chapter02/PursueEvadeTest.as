package
{
	import com.foed.SteeredVehicle;
	import com.foed.Vector2D;
	import com.foed.Vehicle;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;

	public class PursueEvadeTest extends Sprite
	{
		private var _pursuer:SteeredVehicle;
		private var _evader:SteeredVehicle;
		
		public function PursueEvadeTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_pursuer = new SteeredVehicle();
			_pursuer.position = new Vector2D(200, 200);
			_pursuer.edgeBehavior = Vehicle.BOUNCE;
			addChild(_pursuer);
			
			_evader = new SteeredVehicle();
			_evader.position = new Vector2D(400, 300);
			_evader.edgeBehavior = Vehicle.BOUNCE;
			addChild(_evader);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_pursuer.pursue(_evader);
			_evader.evade(_pursuer);
			_pursuer.update();
			_evader.update();
		}
	}
}