package
{
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	[SWF(width=800, height=800, backgroundColor = 0xccffcc)]
	public class DepthSort extends Sprite
	{
		private var _trees:Array;
		
		public function DepthSort()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_trees = new Array();
			
			for(var i:int = 0; i < 500; i++)
			{
				var tree:Shape = new Shape();
				tree.graphics.beginFill(Math.random() * 255 << 8);
				tree.graphics.lineTo(-10, 0);
				tree.graphics.lineTo(-10, -30);
				tree.graphics.lineTo(-40, -30);
				tree.graphics.lineTo(0, -100);
				tree.graphics.lineTo(40, -30);
				tree.graphics.lineTo(10, -30);
				tree.graphics.lineTo(10, 0);
				tree.graphics.lineTo(0, 0);
				tree.graphics.endFill();
				tree.x = Math.random() * stage.stageWidth;
				tree.y = stage.stageHeight - 100;
				tree.z = Math.random() * 10000;
				_trees.push(tree);
			}
			
			_trees.sortOn("z", Array.NUMERIC | Array.DESCENDING);
			for(i = 0; i < 500; i++)
			{
				addChild(_trees[i] as Shape);
			}
		}
		
	}
}