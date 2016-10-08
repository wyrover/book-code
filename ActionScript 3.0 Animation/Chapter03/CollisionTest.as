package
{
	import com.friendsofed.isometric.DrawnIsoBox;
	import com.friendsofed.isometric.DrawnIsoTile;
	import com.friendsofed.isometric.IsoWorld;
	import com.friendsofed.isometric.Point3D;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;

	[SWF(backgroundColor=0xffffff)]
	public class CollisionTest extends Sprite
	{
		private var world:IsoWorld;
		private var box:DrawnIsoBox;
		private var speed:Number = 4;
		
		public function CollisionTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			world = new IsoWorld();
			world.x = stage.stageWidth / 2;
			world.y = 100;
			addChild(world);
			
			for(var i:int = 0; i < 20; i++)
			{
				for(var j:int = 0; j < 20; j++)
				{
					var tile:DrawnIsoTile = new DrawnIsoTile(20, 0xcccccc);
					tile.position = new Point3D(i * 20, 0, j * 20);
					world.addChildToFloor(tile);
				}
			}
			
			box = new DrawnIsoBox(20, 0xff0000, 20);
			box.x = 200;
			box.z = 200;
			world.addChildToWorld(box);
			
			var newBox:DrawnIsoBox = new DrawnIsoBox(20, 0xcccccc, 20);
			newBox.x = 300;
			newBox.z = 300;
			world.addChildToWorld(newBox);
			
			stage.addEventListener(KeyboardEvent.KEY_DOWN, onKeyDown);
			stage.addEventListener(KeyboardEvent.KEY_UP, onKeyUp);
		}
		
		private function onKeyDown(event:KeyboardEvent):void
		{
			switch(event.keyCode)
			{
				case Keyboard.UP :
				box.vx = -speed;
				break;
				
				case Keyboard.DOWN :
				box.vx = speed;
				break;
				
				case Keyboard.LEFT :
				box.vz = speed;
				break;
				
				case Keyboard.RIGHT :
				box.vz = -speed;
				break;
				
				default :
				break;
				
			}
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onKeyUp(event:KeyboardEvent):void
		{
			box.vx = 0;
			box.vz = 0;
			removeEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			if(world.canMove(box))
			{
				box.x += box.vx;
				box.y += box.vy;
				box.z += box.vz;
			}
			world.sort();
		}
	}
}