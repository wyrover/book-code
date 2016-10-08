package
{
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.events.MouseEvent;

	public class Game extends Sprite
	{
		private var _cellSize:int = 20;
		private var _grid:Grid;
		private var _player:Sprite;
		private var _index:int;
		private var _path:Array;
		
		public function Game()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			makePlayer();
			makeGrid();
			stage.addEventListener(MouseEvent.CLICK, onGridClick);
		}
		
		/**
		 * Creates the player sprite. Just a circle here.
		 */
		private function makePlayer():void
		{
			_player = new Sprite();
			_player.graphics.beginFill(0xff0000);
			_player.graphics.drawCircle(0, 0, 5);
			_player.graphics.endFill();
			_player.x = Math.random() * 600;
			_player.y = Math.random() * 600;
			addChild(_player);
		}
		
		/**
		 * Creates a grid with a bunch of random unwalkable nodes.
		 */
		private function makeGrid():void
		{
			_grid = new Grid(30, 30);
			for(var i:int = 0; i < 200; i++)
			{
				_grid.setWalkable(Math.floor(Math.random() * 30),
								  Math.floor(Math.random() * 30),
								  false);
			}
			drawGrid();
		}
		
		/**
		 * Draws the given grid, coloring each cell according to its state.
		 */
		private function drawGrid():void
		{
			graphics.clear();
			for(var i:int = 0; i < _grid.numCols; i++)
			{
				for(var j:int = 0; j < _grid.numRows; j++)
				{
					var node:Node = _grid.getNode(i, j);
					graphics.lineStyle(0);
					graphics.beginFill(getColor(node));
					graphics.drawRect(i * _cellSize, j * _cellSize, _cellSize, _cellSize);
				}
			}
		}
		
		/**
		 * Determines the color of a given node based on its state.
		 */
		private function getColor(node:Node):uint
		{
			if(!node.walkable) return 0;
			if(node == _grid.startNode) return 0xcccccc;
			if(node == _grid.endNode) return 0xcccccc;
			return 0xffffff;
		}

		/**
		 * Handles the click event on the GridView. Finds the clicked on cell and toggles its walkable state.
		 */
		private function onGridClick(event:MouseEvent):void
		{
			var xpos:int = Math.floor(mouseX / _cellSize);
			var ypos:int = Math.floor(mouseY / _cellSize);
			_grid.setEndNode(xpos, ypos);
			
			xpos = Math.floor(_player.x / _cellSize);
			ypos = Math.floor(_player.y / _cellSize);
			_grid.setStartNode(xpos, ypos);
			
			drawGrid();
			findPath();
		}
		
		/**
		 * Creates an instance of AStar and uses it to find a path.
		 */
		private function findPath():void
		{
			var astar:AStar = new AStar();
			if(astar.findPath(_grid))
			{
				_path = astar.path;
				_index = 0;
				addEventListener(Event.ENTER_FRAME, onEnterFrame);
			}
		}
		
		/**
		 * Finds the next node on the path and eases to it.
		 */
		private function onEnterFrame(event:Event):void
		{
			var targetX:Number = _path[_index].x * _cellSize + _cellSize / 2;
			var targetY:Number = _path[_index].y * _cellSize + _cellSize / 2;
			var dx:Number = targetX - _player.x;
			var dy:Number = targetY - _player.y;
			var dist:Number = Math.sqrt(dx * dx + dy * dy);
			if(dist < 1)
			{
				_index++;
				if(_index >= _path.length)
				{
					removeEventListener(Event.ENTER_FRAME, onEnterFrame);
				}
			}
			else
			{
				_player.x += dx * .5;
				_player.y += dy * .5;
			}
		}
	}
}