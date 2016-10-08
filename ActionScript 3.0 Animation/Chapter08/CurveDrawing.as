package
{
	import __AS3__.vec.Vector;
	
	import flash.display.GraphicsPathCommand;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	[SWF(backgroundColor=0xffffff)]
	public class CurveDrawing extends Sprite
	{
		private var commands:Vector.<int> = new Vector.<int>();
		private var data:Vector.<Number> = new Vector.<Number>();
		
		public function CurveDrawing()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			commands.push(GraphicsPathCommand.MOVE_TO);
			data.push(200, 200);
			
			data.push(250, 100);
			data.push(300, 200);
			
			data.push(400, 250);
			data.push(300, 300);
			
			data.push(250, 400);
			data.push(200, 300);
			
			data.push(100, 250);
			data.push(200, 200);
			
			commands.push(GraphicsPathCommand.CURVE_TO);
			commands.push(GraphicsPathCommand.CURVE_TO);
			commands.push(GraphicsPathCommand.CURVE_TO);
			commands.push(GraphicsPathCommand.CURVE_TO);
			
			graphics.lineStyle(0);
			graphics.drawPath(commands, data);
		}
	}
}