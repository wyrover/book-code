package
{
	import com.foed.Vector2D;
	import com.foed.SteeredVehicle;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;

	public class Steering2 extends Sprite
	{
		private var _small:SteeredVehicle;
		private var _medium:SteeredVehicle;
		private var _large:SteeredVehicle;
		
		
		public function Steering2()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_small = new SteeredVehicle();
			_small.scaleX = _small.scaleY = .5;
//			_small.mass = .5;
			_small.position = new Vector2D(300, 400);
			addChild(_small);
			
			_medium = new SteeredVehicle();
			_medium.position = new Vector2D(100, 100);
			addChild(_medium);
			
//			_large = new SteeredVehicle();
//			_large.scaleX = _large.scaleY = 1.5;
//			_large.mass = 2;
//			_large.position = new Vector2D(600, 140);
//			addChild(_large);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_small.wander();
			_medium.seek(_small.position);
			_small.update();
			_medium.update();
//			_large.update();
		}
		
	}
}