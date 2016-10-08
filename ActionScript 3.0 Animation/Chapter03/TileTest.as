package
{
	import com.friendsofed.isometric.DrawnIsoTile;
	import com.friendsofed.isometric.Point3D;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;

	[SWF(backgroundColor=0xffffff)]
	public class TileTest extends Sprite
	{
		public function TileTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var world:Sprite = new Sprite();
			world.x = stage.stageWidth / 2;
			world.y = 100;
			addChild(world);
			
			for(var i:int = 0; i < 20; i++)
			{
				for(var j:int = 0; j < 20; j++)
				{
					var tile:DrawnIsoTile = new DrawnIsoTile(20, 0xcccccc);
					tile.position = new Point3D(i * 20, 0, j * 20);
					world.addChild(tile);
				}
			}
		}
		
	}
}