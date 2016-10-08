package
{
	import __AS3__.vec.Vector;
	
	import flash.display.GraphicsPathCommand;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.ui.Keyboard;
	
	[SWF(backgroundColor=0xffffff)]
	public class PathSketch extends Sprite
	{
		private var commands:Vector.<int> = new Vector.<int>();
		private var data:Vector.<Number> = new Vector.<Number>();
		
		private var lineWidth:Number = 0;
		private var lineColor:uint = 0;
		
		public function PathSketch()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
	
			stage.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			stage.addEventListener(KeyboardEvent.KEY_UP, onKeyUp);
		}
		
		
		private function onMouseDown(event:MouseEvent):void
		{
			commands.push(GraphicsPathCommand.MOVE_TO);
			data.push(mouseX, mouseY);
			stage.addEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			stage.addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			draw();
		}
		
		private function onMouseUp(event:MouseEvent):void
		{
			stage.removeEventListener(MouseEvent.MOUSE_MOVE, onMouseMove);
			stage.removeEventListener(MouseEvent.MOUSE_UP, onMouseUp);
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			commands.push(GraphicsPathCommand.LINE_TO);
			data.push(mouseX, mouseY);
			draw();
		}
		
		private function onKeyUp(event:KeyboardEvent):void
		{
			if(event.keyCode == Keyboard.DOWN)
			{
				lineWidth = Math.max(0, lineWidth -1);
			}
			else if(event.keyCode == Keyboard.UP)
			{
				lineWidth++;
			}
			else if(event.keyCode == Keyboard.SPACE)
			{
				lineColor = Math.random() * 0xffffff;
			}
			draw();
		}
		
		private function draw():void
		{
			graphics.clear();
			graphics.lineStyle(lineWidth, lineColor);
			graphics.drawPath(commands, data);
		}
	}
}