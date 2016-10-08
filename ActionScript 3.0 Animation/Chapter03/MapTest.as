package
{
	import com.friendsofed.isometric.DrawnIsoBox;
	import com.friendsofed.isometric.GraphicTile;
	import com.friendsofed.isometric.IsoWorld;
	import com.friendsofed.isometric.MapLoader;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	
	[SWF(backgroundColor=0xffffff)]
	public class MapTest extends Sprite
	{
		private var _world:IsoWorld;
		private var _floor:IsoWorld;
		private var mapLoader:MapLoader;
		
		[Embed(source="tile_01.png")]
		private var Tile01:Class;
		
		[Embed(source="tile_02.png")]
		private var Tile02:Class;
		
		public function MapTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			mapLoader = new MapLoader();
			mapLoader.addEventListener(Event.COMPLETE, onMapComplete);
			mapLoader.loadMap("map.txt");
		}
		
		private function onMapComplete(event:Event):void
		{
			_world = mapLoader.makeWorld(20);
			_world.x = stage.stageWidth / 2;
			_world.y = 100;
			addChild(_world);
		}
	}
}