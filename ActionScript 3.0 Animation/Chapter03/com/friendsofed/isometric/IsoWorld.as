package com.friendsofed.isometric
{
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.geom.Rectangle;

	public class IsoWorld extends Sprite
	{
		private var _floor:Sprite;
		private var _objects:Array;
		private var _world:Sprite;
		
		public function IsoWorld()
		{
			_floor = new Sprite();
			addChild(_floor);
			
			_world = new Sprite();
			addChild(_world);
			
			_objects = new Array();
		}
		
		public function addChildToWorld(child:IsoObject):void
		{
			_world.addChild(child);
			_objects.push(child);
			sort();
		}
		
		public function addChildToFloor(child:IsoObject):void
		{
			_floor.addChild(child);
		}
		
		public function sort():void
		{
			_objects.sortOn("depth", Array.NUMERIC);
			for(var i:int = 0; i < _objects.length; i++)
			{
				_world.setChildIndex(_objects[i], i);
			}
		}
		
		public function canMove(obj:IsoObject):Boolean
		{
			var rect:Rectangle = obj.rect;
			rect.offset(obj.vx, obj.vz);
			for(var i:int = 0; i < _objects.length; i++)
			{
				var objB:IsoObject = _objects[i] as IsoObject;
				if(obj != objB && !objB.walkable && rect.intersects(objB.rect))
				{
					return false;
				}
			}
			return true;
		}
	}
}