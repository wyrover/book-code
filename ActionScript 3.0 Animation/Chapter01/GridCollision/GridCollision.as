package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.utils.getTimer;

	public class GridCollision extends Sprite
	{
		private const GRID_SIZE:Number = 120;
		private const RADIUS:Number = 25;
		
		private var _balls:Array;
		private var _grid:Array;
		private var _numBalls:int = 100;
		private var _numChecks:int = 0;
		
		public function GridCollision()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			makeBalls();
			drawGrid();
			
			var startTime:int;
			var elapsed:int;
			var i:int;
			
			startTime = getTimer();
			for(i = 0; i < 10; i++)
			{
				makeGrid();
				assignBallsToGrid();
				checkGrid();
			}
			elapsed = getTimer() - startTime;
			trace("Grid-based:", elapsed);
			
			startTime = getTimer();
			for(i = 0; i < 10; i++)
			{
				basicCheck();
			}
			elapsed = getTimer() - startTime;
			trace("Basic check", elapsed);
		}
		
		
		private function makeBalls():void
		{
			_balls = new Array();
			for(var i:int = 0; i < _numBalls; i++)
			{
				// create a Ball and add it to the displa list and the _balls array.
				var ball:Ball = new Ball(RADIUS);
				ball.x = Math.random() * stage.stageWidth;
				ball.y = Math.random() * stage.stageHeight;
				addChild(ball);
				_balls.push(ball);
			}
		}
		
		private function makeGrid():void
		{
			_grid = new Array();
			// stage width / grid size = number of columns
   			for(var i:int = 0; i < stage.stageWidth / GRID_SIZE; i++)
			{
				_grid[i] = new Array();
				// stage width / grid size = number of columns
				for(var j:int = 0; j < stage.stageHeight / GRID_SIZE; j++)
				{
					_grid[i][j] = new Array();
				}
			}
		}
		
		private function drawGrid():void
		{
			// draw lines to indicate rows and columns
			graphics.lineStyle(0, .5);
			for(var i:int = 0; i <= stage.stageWidth; i += GRID_SIZE)
			{
				graphics.moveTo(i, 0);
				graphics.lineTo(i, stage.stageHeight);
			}
			for(i = 0; i <= stage.stageHeight; i += GRID_SIZE)
			{
				graphics.moveTo(0, i);
				graphics.lineTo(stage.stageWidth, i);
			}			
		}
		
		private function assignBallsToGrid():void
		{
			for(var i:int = 0; i < _numBalls; i++)
			{
				// dividing position by grid size 
				// tells us which row and column each ball is in
				var ball:Ball = _balls[i] as Ball;
				var xpos:int = Math.floor(ball.x / GRID_SIZE);
				var ypos:int = Math.floor(ball.y / GRID_SIZE);
				_grid[xpos][ypos].push(ball);
			}
		}
		
		private function checkGrid():void
		{
		    // loop through each row and column of grid
		    for(var i:int = 0; i < _grid.length; i++)
		    {
		        for(var j:int = 0; j < _grid[i].length; j++)
		        {
		            // examine all the objects in the first cell against each other
		            checkOneCell(i, j);
		
		            checkTwoCells(i, j, i + 1, j);     // cell to the right
		            checkTwoCells(i, j, i - 1, j + 1); // cell below to the left
		            checkTwoCells(i, j, i, j + 1);     // cell directly below
		            checkTwoCells(i, j, i + 1, j + 1); // cell below to the right
		        }
		    }
		}
		private function checkOneCell(x:int, y:int):void
		{
			// check all the balls in a single cell against each other			
			var cell:Array = _grid[x][y] as Array;
			
			for(var i:int = 0; i < cell.length - 1; i++)
			{
				var ballA:Ball = cell[i] as Ball;
				for(var j:int = i + 1; j < cell.length; j++)
				{
					var ballB:Ball = cell[j] as Ball;
					checkCollision(ballA, ballB);
				}
			}
		}
		
		private function checkTwoCells(x1:int, y1:int, x2:int, y2:int):void
		{
			// make sure the second cell really exists
			if(x2 < 0) return;
			if(x2 >= _grid.length) return;
			if(y2 >= _grid[x2].length) return;
			
			var cell0:Array = _grid[x1][y1] as Array;
			var cell1:Array = _grid[x2][y2] as Array;
			
			// check all the balls in one cell against all in the other
			for(var i:int = 0; i < cell0.length; i++)
			{
				var ballA:Ball = cell0[i] as Ball;
				for(var j:int = 0; j < cell1.length; j++)
				{
					var ballB:Ball = cell1[j] as Ball;
					checkCollision(ballA, ballB);
				}
			}
		}
		
		private function checkCollision(ballA:Ball, ballB:Ball):void
		{
			// if distance is less than sum of radii, collision
//			_numChecks++;
			var dx:Number = ballB.x - ballA.x;
			var dy:Number = ballB.y - ballA.y;
			var dist:Number = Math.sqrt(dx * dx + dy * dy);
			if(dist < ballA.radius + ballB.radius)
			{
				ballA.color = 0xff0000;
				ballB.color = 0xff0000;
			}
		}
		
		private function basicCheck():void
		{
			for(var i:int = 0; i < _balls.length - 1; i++)
			{
				var ballA:Ball = _balls[i] as Ball;
				for(var j:int = i + 1; j < _balls.length; j++)
				{
					var ballB:Ball = _balls[j] as Ball;
					checkCollision(ballA, ballB);
				}
			}
		}
	}
}
