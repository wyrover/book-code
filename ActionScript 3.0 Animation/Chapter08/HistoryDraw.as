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
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.ui.Keyboard;
	
	[SWF(backgroundColor=0xffffff)]
	public class HistoryDraw extends Sprite
	{
		private var graphicsData:Vector.<IGraphicsData>;
		private var graphicsDataBuffer:Vector.<IGraphicsData>;
		private var commands:Vector.<int>;
		private var data:Vector.<Number>;
		private var index:int = 0;
		
		public function HistoryDraw()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			graphicsData = new Vector.<IGraphicsData>();
			graphicsDataBuffer = new Vector.<IGraphicsData>();
			
			stage.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			stage.addEventListener(KeyboardEvent.KEY_UP, onKeyUp);
		}
		
		private function onMouseDown(event:MouseEvent):void
		{
			// create a random stroke, add it to buffer
			var stroke:GraphicsStroke = new GraphicsStroke();
			stroke.thickness = Math.random() * 10;
			stroke.fill = new GraphicsSolidFill(Math.random() * 0xffffff);
			graphicsDataBuffer.push(stroke);
			
			// increment index and make this the last command in the buffer 
			index++;
			graphicsDataBuffer.length = index;
			
			// push a moveTo onto the commands
			commands = new Vector.<int>();
			commands.push(GraphicsPathCommand.MOVE_TO);
			
			// move to this point
			data = new Vector.<Number>();
			data.push(mouseX, mouseY);
			
			// draw a gray line for now
			graphics.lineStyle(0, 0, .5);
			graphics.moveTo(mouseX, mouseY);
			
			stage.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			stage.addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			// push a lineto onto the commands, and current point to data
			commands.push(GraphicsPathCommand.LINE_TO);
			data.push(mouseX, mouseY);
			
			// draw temporary line
			graphics.lineTo(mouseX, mouseY);
		}
		
		private function onMouseUp(event:MouseEvent):void
		{
			// this line is done. push path onto buffer
			graphicsDataBuffer.push(new GraphicsPath(commands, data));
			
			// increment index and make this last command in buffer
			index++;
			graphicsDataBuffer.length = index;
			
			stage.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			stage.removeEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			draw();
		}
		
		private function onKeyUp(event:KeyboardEvent):void
		{
			// go back two commands (path and stroke)
			if(event.keyCode == Keyboard.LEFT)
			{
				index -= 2;
			}
			// go forward two commands (stroke and path)
			else if(event.keyCode == Keyboard.RIGHT)
			{
				index += 2;
			}
			// limit index to sensible range
			index = Math.max(0, index);
			index = Math.min(graphicsDataBuffer.length, index);
			draw();
		}
		
		private function draw():void
		{
			// clear graphics data vector
			graphicsData.length = 0;
			
			// copy over data from buffer up to index
			for(var i:int = 0; i < index; i++)
			{
				graphicsData[i] = graphicsDataBuffer[i];
			}
			// draw graphics data
			graphics.clear();
			graphics.drawGraphicsData(graphicsData);
		}
	}
}