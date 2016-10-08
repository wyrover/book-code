package
{
	/**
	 * Holds a two-dimensional array of Nodes methods to manipulate them, start node and end node for finding a path.
	 */
	public class Grid
	{
		private var _startNode:Node;
		private var _endNode:Node;
		private var _nodes:Array;
		private var _numCols:int;
		private var _numRows:int;
		
		/**
		 * Constructor.
		 */
		public function Grid(numCols:int, numRows:int)
		{
			_numCols = numCols;
			_numRows = numRows;
			_nodes = new Array();
			
			for(var i:int = 0; i < _numCols; i++)
			{
				_nodes[i] = new Array();
				for(var j:int = 0; j < _numRows; j++)
				{
					_nodes[i][j] = new Node(i, j);
				}
			}
		}
		
		
		////////////////////////////////////////
		// public methods
		////////////////////////////////////////
		
		/**
		 * Returns the node at the given coords.
		 * @param x The x coord.
		 * @param y The y coord.
		 */
		public function getNode(x:int, y:int):Node
		{
			return _nodes[x][y] as Node;
		}
		
		/**
		 * Sets the node at the given coords as the end node.
		 * @param x The x coord.
		 * @param y The y coord.
		 */
		public function setEndNode(x:int, y:int):void
		{
			_endNode = _nodes[x][y] as Node;
		}
		
		/**
		 * Sets the node at the given coords as the start node.
		 * @param x The x coord.
		 * @param y The y coord.
		 */
		public function setStartNode(x:int, y:int):void
		{
			_startNode = _nodes[x][y] as Node;
		}
		
		/**
		 * Sets the node at the given coords as walkable or not.
		 * @param x The x coord.
		 * @param y The y coord.
		 */
		public function setWalkable(x:int, y:int, value:Boolean):void
		{
			_nodes[x][y].walkable = value;
		}
		
		
		
		////////////////////////////////////////
		// getters / setters
		////////////////////////////////////////
		
		/**
		 * Returns the end node.
		 */
		public function get endNode():Node
		{
			return _endNode;
		}
		
		/**
		 * Returns the number of columns in the grid.
		 */
		public function get numCols():int
		{
			return _numCols;
		}
		
		/**
		 * Returns the number of rows in the grid.
		 */
		public function get numRows():int
		{
			return _numRows;
		}
		
		/**
		 * Returns the start node.
		 */
		public function get startNode():Node
		{
			return _startNode;
		}
		
	}
}