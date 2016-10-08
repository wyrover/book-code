package
{
	import __AS3__.vec.Vector;
	
	import flash.display.GraphicsPathCommand;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	[SWF(backgroundColor=0xffffff)]
	public class SingleLine extends Sprite
	{
		public function SingleLine()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var commands:Vector.<int> = new Vector.<int>();
			commands[0] = GraphicsPathCommand.MOVE_TO;
			commands[1] = GraphicsPathCommand.LINE_TO;
			
			var data:Vector.<Number> = new Vector.<Number>();
			data[0] = 100;
			data[1] = 100;
			data[2] = 250;
			data[3] = 200;
			
			graphics.lineStyle(0);
			graphics.drawPath(commands, data);
			
		}
	}
}