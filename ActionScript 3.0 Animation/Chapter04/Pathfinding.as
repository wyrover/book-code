package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.MouseEvent;

	[SWF(backgroundColor=0xffffff)]
	public class Pathfinding extends Sprite
	{
		private var _grid:Grid;
		private var _gridView:GridView;
		
		
		public function Pathfinding()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_grid = new Grid(50, 30);
			_grid.setStartNode(0, 2);
			_grid.setEndNode(48, 27);
			
			_gridView = new GridView(_grid);
			_gridView.x = 20;
			_gridView.y = 20;
			addChild(_gridView);
		}
		
	}
}
