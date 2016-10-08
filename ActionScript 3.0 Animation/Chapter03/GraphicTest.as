package
{
	import com.friendsofed.isometric.GraphicTile;
	import com.friendsofed.isometric.IsoUtils;
	import com.friendsofed.isometric.IsoWorld;
	import com.friendsofed.isometric.Point3D;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.MouseEvent;
	import flash.geom.Point;

	[SWF(backgroundColor=0xffffff)]
	public class GraphicTest extends Sprite
	{
		private var world:IsoWorld;
		
		[Embed(source="tile_01.png")]
		private var Tile01:Class;
		
		[Embed(source="tile_02.png")]
		private var Tile02:Class;
		
		public function GraphicTest()
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
					var tile:GraphicTile = new GraphicTile(20, Tile01, 20, 10);
					tile.position = new Point3D(i * 20, 0, j * 20);
					world.addChildToFloor(tile);
				}
			}
			stage.addEventListener(MouseEvent.CLICK, onWorldClick);
		}
		
		private function onWorldClick(event:MouseEvent):void
		{
			var box:GraphicTile = new GraphicTile(20, Tile02, 20, 30);
			var pos:Point3D = IsoUtils.screenToIso(new Point(world.mouseX, world.mouseY));
			pos.x = Math.round(pos.x / 20) * 20;
			pos.y = Math.round(pos.y / 20) * 20;
			pos.z = Math.round(pos.z / 20) * 20;
			box.position = pos;
			world.addChildToWorld(box);
		}
	}
}