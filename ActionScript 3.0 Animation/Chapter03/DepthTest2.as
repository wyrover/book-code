package
{
	import com.friendsofed.isometric.DrawnIsoBox;
	import com.friendsofed.isometric.DrawnIsoTile;
	import com.friendsofed.isometric.IsoUtils;
	import com.friendsofed.isometric.Point3D;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.MouseEvent;
	import flash.geom.Point;

	[SWF(backgroundColor=0xffffff)]
	public class DepthTest2 extends Sprite
	{
		private var floor:Sprite;
		private var world:Sprite;
		private var objectList:Array;
		
		public function DepthTest2()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			floor = new Sprite();
			floor.x = stage.stageWidth / 2;
			floor.y = 100;
			addChild(floor);
			
			world = new Sprite();
			world.x = stage.stageWidth / 2;
			world.y = 100;
			addChild(world);
			
			objectList = new Array();
			
			for(var i:int = 0; i < 20; i++)
			{
				for(var j:int = 0; j < 20; j++)
				{
					var tile:DrawnIsoTile = new DrawnIsoTile(20, 0xcccccc);
					tile.position = new Point3D(i * 20, 0, j * 20);
					floor.addChild(tile);
				}
			}
			stage.addEventListener(MouseEvent.CLICK, onWorldClick);
		}
		
		private function onWorldClick(event:MouseEvent):void
		{
			var box:DrawnIsoBox = new DrawnIsoBox(20, Math.random() * 0xffffff, 20);
			var pos:Point3D = IsoUtils.screenToIso(new Point(world.mouseX, world.mouseY));
			pos.x = Math.round(pos.x / 20) * 20;
			pos.y = Math.round(pos.y / 20) * 20;
			pos.z = Math.round(pos.z / 20) * 20;
			box.position = pos;
			world.addChild(box);
			objectList.push(box);
			sortList();
		}
		
		private function sortList():void
		{
			objectList.sortOn("depth", Array.NUMERIC);
			for(var i:int = 0; i < objectList.length; i++)
			{
				world.setChildIndex(objectList[i], i);
			}
		}
	}
}