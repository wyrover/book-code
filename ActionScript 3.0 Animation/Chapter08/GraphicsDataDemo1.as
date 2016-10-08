package
{
	import __AS3__.vec.Vector;
	
	import flash.display.GraphicsPath;
	import flash.display.GraphicsPathCommand;
	import flash.display.GraphicsSolidFill;
	import flash.display.GraphicsStroke;
	import flash.display.IGraphicsData;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	[SWF(backgroundColor=0xffffff)]
	public class GraphicsDataDemo1 extends Sprite
	{
		public function GraphicsDataDemo1()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var graphicsData:Vector.<IGraphicsData> = new Vector.<IGraphicsData>();
			
			var stroke:GraphicsStroke = new GraphicsStroke(5);
			stroke.fill = new GraphicsSolidFill(0xff0000);
			
			var commands:Vector.<int> = new Vector.<int>();
			commands.push(GraphicsPathCommand.MOVE_TO);
			commands.push(GraphicsPathCommand.LINE_TO);
			
			var data:Vector.<Number> = new Vector.<Number>();
			data.push(100, 100);
			data.push(200, 200);
			
			var path:GraphicsPath = new GraphicsPath(commands, data);
			
			graphicsData.push(stroke);
			graphicsData.push(path);
			
			graphics.drawGraphicsData(graphicsData);
		}
	}
}