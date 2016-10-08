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
	import flash.events.MouseEvent;
	import flash.filters.BlurFilter;

	[SWF(backgroundColor=0xffffff)]
	public class MotionTest2 extends Sprite
	{
		private var world:IsoWorld;
		private var box:DrawnIsoBox;
		private var shadow:DrawnIsoTile;
		private var gravity:Number = 2;
		private var friction:Number = 0.95;
		private var bounce:Number = -0.9;
		private var filter:BlurFilter;
		
		public function MotionTest2()
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
			
			shadow = new DrawnIsoTile(20, 0);
			shadow.alpha = .5;
			world.addChildToFloor(shadow);
			
			filter = new BlurFilter();
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
			stage.addEventListener(MouseEvent.CLICK, onClick);
		}
		
		private function onClick(event:MouseEvent):void
		{
			box.vx = Math.random() * 20 - 10;
			box.vy = -Math.random() * 40;
			box.vz = Math.random() * 20 - 10;
		}
		private function onEnterFrame(event:Event):void
		{
			box.vy += 2;
			box.x += box.vx;
			box.y += box.vy;
			box.z += box.vz;
			if(box.x > 380)
			{
				box.x = 380;
				box.vx *= -.8;
			}
			else if(box.x < 0)
			{
				box.x = 0;
				box.vx *= bounce;
			}
			if(box.z > 380)
			{
				box.z = 380;
				box.vz *= bounce;
			}
			else if(box.z < 0)
			{
				box.z = 0;
				box.vz *= bounce;
			}
			if(box.y > 0)
			{
				box.y = 0;
				box.vy *= bounce;
			}
			box.vx *= friction;
			box.vy *= friction;
			box.vz *= friction;
			
			shadow.x = box.x;
			shadow.z = box.z;
			filter.blurX = filter.blurY = -box.y * .25;
			shadow.filters = [filter];
		}
	}
}