package
{
	import com.foed.SteeredVehicle;
	import com.foed.Vector2D;
	import com.foed.Vehicle;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;

	public class PursueTest extends Sprite
	{
		private var _seeker:SteeredVehicle;
		private var _pursuer:SteeredVehicle;
		private var _target:Vehicle;
		
		public function PursueTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_seeker = new SteeredVehicle();
			_seeker.x = 400;
			addChild(_seeker);
			
			_pursuer = new SteeredVehicle();
			_pursuer.x = 400;
			addChild(_pursuer);
			
			_target = new Vehicle();
			_target.position = new Vector2D(200, 300);
			_target.velocity.length = 15;
			addChild(_target);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_seeker.seek(_target.position);
			_seeker.update();
			
			_pursuer.pursue(_target);
			_pursuer.update();
			
			_target.update();
		}
	}
}