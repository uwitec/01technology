package org.god.GameModule.Common.SlotWheel
{
	import flash.events.*;
	import flash.display.*;
	import flash.utils.*;
	
	[Event(name="motionChange", type="SlotWheel.ScrollEngineEvent")];
	[Event(name="motionFinish", type="SlotWheel.ScrollEngineEvent")];
	[Event(name="motionLoop", type="SlotWheel.ScrollEngineEvent")];
	[Event(name="motionResume", type="SlotWheel.ScrollEngineEvent")];
	[Event(name="motionStart", type="SlotWheel.ScrollEngineEvent")];
	[Event(name="motionStop", type="SlotWheel.ScrollEngineEvent")];

	public class BaseScrollEngine extends EventDispatcher
	{
		protected static  var _mc:MovieClip = new MovieClip();
		public var isPlaying:Boolean = false;
		public var obj:* = null;
		public var func:Function = function (t:Number, b:Number, c:Number, d:Number):Number { return c*t/d + b; };
		public var begin:Number = NaN;
		public var change:Number = NaN;
		public var useSeconds:Boolean = false;
		public var prevTime:Number = NaN;
		public var prevPos:Number = NaN;
		public var looping:Boolean = false;
		private var _duration:Number = NaN;
		private var _time:Number = NaN;
		private var _fps:Number = NaN;
		private var _position:Number = NaN;
		private var _startTime:Number = NaN;
		private var _intervalID:uint = 0;
		private var _finish:Number = NaN;
		private var _timer:Timer = null;

		public function get time():Number
		{
			return this._time;
		}

		public function set time(t:Number):void
		{
			this.prevTime = this._time;
			if (t > this.duration)
			{
				if (this.looping)
				{
					this.rewind(t - this._duration);
					this.update();
					this.dispatchEvent(new ScrollEngineEvent(ScrollEngineEvent.MOTION_LOOP, this._time, this._position));
				}
				else
				{
					if (this.useSeconds)
					{
						this._time = this._duration;
						this.update();
					}
					this.stop();
					this.dispatchEvent(new ScrollEngineEvent(ScrollEngineEvent.MOTION_FINISH, this._time, this._position));
				}
			}
			else if (t < 0)
			{
				this.rewind();
				this.update();
			}
			else
			{
				this._time = t;
				this.update();
			}
		}

		public function get duration():Number
		{
			return this._duration;
		}
		public function set duration(d:Number):void
		{
			this._duration = (d <= 0) ? Infinity : d;
		}
		public function get FPS():Number
		{
			return this._fps;
		}

		public function set FPS(fps:Number):void
		{
			var oldIsPlaying:Boolean = this.isPlaying;
			this.stopEnterFrame();
			this._fps = fps;
			if (oldIsPlaying)
			{
				this.startEnterFrame();
			}
		}
		public function get position():Number
		{
			return this.getPosition(this._time);
		}
		public function set position(p:Number):void
		{
			this.setPosition(p);
		}
		public function getPosition(t:Number=NaN):Number
		{
			if (isNaN(t))
			{
				t = this._time;
			}
			return this.func(t,this.begin,this.change,this._duration);
		}
		public function setPosition(p:Number):void
		{
			this.prevPos = this._position;
			
			this._position = p;
			
			OnScrollLoop(p);
			//trace("setPosition  : " + p);

			this.dispatchEvent(new ScrollEngineEvent(ScrollEngineEvent.MOTION_CHANGE, this._time, this._position));
		}
		public function get finish():Number
		{
			return this.begin + this.change;
		}

		public function set finish(value:Number):void
		{
			this.change = value - this.begin;
		}
		function BaseScrollEngine(obj:*, 
							  func:Function, 
							  begin:Number, 
							  finish:Number, 
							  duration:Number,
							  useSeconds:Boolean=true)
		{
			if (!arguments.length)
			{
				return;
			}
			this.obj = obj;
			this.begin = begin;
			this.position = begin;
			this.duration = duration;
			this.useSeconds = useSeconds;
			if (func is Function)
			{
				this.func = func;
			}
			this.finish = finish;
			this._timer = new Timer(40);
		}
		public function Destroy():void
		{
			this.stop();
			this.obj = null;
			this.func= null;
			_timer = null;
		}
		public function continueTo(finish:Number, duration:Number):void
		{
			this.begin = this.position;
			this.finish = finish;
			if (!isNaN(duration))
			{
				this.duration = duration;
			}
			this.start();
		}
		public function yoyo():void
		{
			this.continueTo(this.begin, this.time);
		}

		protected function startEnterFrame():void
		{
			if (isNaN(this._fps))
			{
				_mc.addEventListener(Event.ENTER_FRAME, this.onEnterFrame, false, 0, true);
			}
			else
			{
				var milliseconds:Number = 1000 / this._fps;
				this._timer.delay = milliseconds;
				this._timer.addEventListener(TimerEvent.TIMER, this.timerHandler, false, 0, true);
				this._timer.start();
			}
			this.isPlaying = true;
		}

		protected function stopEnterFrame():void
		{
			if (isNaN(this._fps))
			{
				_mc.removeEventListener(Event.ENTER_FRAME, this.onEnterFrame);
			}
			else
			{
				this._timer.stop();
			}
			this.isPlaying = false;
		}

		public function start():void
		{
			this.rewind();
			this.startEnterFrame();
			this.dispatchEvent(new ScrollEngineEvent(ScrollEngineEvent.MOTION_START, this._time, this._position));
		}
		public function stop():void
		{
			this.stopEnterFrame();
			this.dispatchEvent(new ScrollEngineEvent(ScrollEngineEvent.MOTION_STOP, this._time, this._position));
		}

		public function resume():void
		{
			this.fixTime();
			this.startEnterFrame();
			this.dispatchEvent(new ScrollEngineEvent(ScrollEngineEvent.MOTION_RESUME, this._time, this._position));
		}
		public function rewind(t:Number=0):void
		{
			this._time = t;
			this.fixTime();
			this.update();
		}
		public function fforward():void
		{
			this.time = this._duration;
			this.fixTime();
		}
		public function nextFrame():void
		{
			if (this.useSeconds)
			{
				this.time = (getTimer() - this._startTime) / 1000;
			}
			else
			{
				this.time = this._time + 1;
			}
		}

		protected function onEnterFrame(event:Event):void
		{
			this.nextFrame();
		}

		protected function timerHandler(timerEvent:TimerEvent):void
		{
			this.nextFrame();
			timerEvent.updateAfterEvent();
		}

		public function prevFrame():void
		{
			if (!this.useSeconds)
			{
				this.time = this._time - 1;
			}
		}
		private function fixTime():void
		{
			if (this.useSeconds)
			{
				this._startTime = getTimer() - this._time*1000;
			}
		}

		private function update():void
		{
			this.setPosition(this.getPosition(this._time));
		}
		//////////////
		protected function OnScrollLoop(p:Number):void
		{
		}
	}
}