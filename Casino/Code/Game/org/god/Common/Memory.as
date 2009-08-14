import flash.utils.ByteArray;
import flash.utils.Endian;
//拷贝内存
function CopyMemory(dst:ByteArray,src:ByteArray,length:uint = 0,
					dst_offset:uint = 0, src_offset:uint = 0):void {
	var old_pos:int = src.position;
	src.position = src_offset;
	src.readBytes(dst,dst_offset,length);
	src.position = old_pos;
}
//新建数组
function NewArray(length:uint,c:Class = null):Array
{
	var result:Array = new Array(length);
	if(c != null)
	{
		for(var i:uint = 0; i < length; i ++)
		{
			result[i] = new c;
		}
	}
	return result;
}
//新建数组且初始化
function NewArrayAndSetValue(length:uint,v:* = 0):Array
{
	var result:Array = new Array(length);

	for(var i:uint = 0; i < length; i ++)
	{
		result[i] = v;
	}
	
	return result;
}
//新建数组通过拷贝
function NewArrayByCopy(src:Array,length:uint = 0, src_offset:uint = 0):Array
{
	if(src == null || length == 0)
		length = src.length;
	var result:Array = new Array(length);
	for(var i:uint = 0; i < length; i ++)
	{
		result[i] = src[src_offset + i];
	}
	
	return result;
}
//拷贝数组
function CopyArray(dst:Array,src:Array,length:uint = 0,
					dst_offset:uint = 0, src_offset:uint = 0):void {
	if(length == 0 && src != null)
		length = src.length;
	for(var i:uint = 0; i < length; i ++)
	{
		dst[dst_offset + i] = src[src_offset + i];
	}
}
//数组元素执行函数
function EachArray(dst:Array,func:Function):void 
{
	if(dst == null || func == null)
		return;
	for(var i:uint = 0; i < dst.length; i ++)
	{
		if(dst[i])
		{
			func(dst[i]);
		}
	}
}
//拷贝二维数组
function CopyTwoDimensionArray(dst:Array,src:Array,length:uint = 0,
					dst_offset:uint = 0, src_offset:uint = 0):void {
	if(src == null)
		return;
	for(var i:uint = 0; i < length; i ++)
	{
		dst[dst_offset + i] = src[src_offset + i];
	}
}
//二维数组元素执行函数
function EachTwoDimensionArray(dst:Array,func:Function):void 
{
	if(dst == null || func == null)
		return;
	for(var i:uint = 0; i < dst.length; i ++)
	{
		if(dst[i])
		{
			for(var k:uint = 0; k < dst[i].length; k ++)
			{
				if(dst[i][k])
				{
					func(dst[i][k]);
				}
			}
		}
	}
}
//设置二维数组元素
function FreeTwoDimensionArray(dst:Array,
							   val:* = null,
							   func:Function = null):void 
{
	if(dst == null)
		return;
	for(var i:uint = 0; i < dst.length; i ++)
	{
		if(dst[i])
		{
			for(var k:uint = 0; k < dst[i].length; k ++)
			{
				if(dst[i][k] && func != null)
				{
					func(dst[i][k]);
				}
				dst[i][k] = val;
			}
			dst[i] = null;
		}
	}
}
//清空数组
function ZeroArray(src:Array,val:* = 0,func:Function=null,length=0):void 
{
	if(src == null)
		return;
	if(length <= 0)
		length = src.length;
	for(var i:uint = 0; i < length; i ++)
	{
		if(func != null && src[i])
		{
			func(src[i]);
		}
		src[i] = val;
	}
}
//清空二维数组
function ZeroTwoDimensionArray(src:Array,val:* = 0,func:Function=null):void 
{
	if(src == null)
		return;
	for(var i:uint = 0; i < src.length; i ++)
	{
		if(src[i])
		{
			var a:Array = src[i];
			if(a)
			{
				for(var j:uint = 0; j < a.length; j ++)
				{
					if(func != null && a[j])
					{
						func(a[j]);
					}
					a[j] = val;
				}
				
			}
		}
		
	}
}
//清空三维数组
function ZeroThreeDimensionArray(src:Array,val:* = 0,func:Function=null):void 
{
	if(src == null)
		return;
	for(var i:uint = 0; i < src.length; i ++)
	{
		if(src[i])
		{
			var a:Array = src[i];
			if(a)
			{
				for(var j:uint = 0; j < a.length; j ++)
				{
					var a0:Array = a[j];
					if(a0)
					{
						for(var k:uint = 0; k < a0.length; k ++)
						{
							if(func != null && a0[k])
							{
								func(a0[k]);
							}
							a0[k] = val;
						}
					}
				}
				
			}
		}
		
	}
}
//数组长度
function CountArray(src:Array):uint
{
	if(src == null)
	{
		return 0;
	}
	return src.length;
}
//拷贝数组
function CloneArray(src:Array, beginIndex:uint):Array
{
	if(src == null)
		return null;
	var result:Array = new Array;
	var n:uint = 0;
	for(var i:uint = beginIndex; i < src.length; i ++)
	{
		result[n] = src[i];
		n ++;
	}
	return result;
}
//移动数组单元
function MoveArray(dst:Array,src:Array,length:uint = 0,
					dst_offset:uint = 0, src_offset:uint = 0):void 
{
	for(var i:uint = 0; i < length;i++)
	{
		dst[dst_offset + i] = src[src_offset + i];
	}
}
//移动内存字节流
function MoveMemory(dst:ByteArray,src:ByteArray,length:uint = 0,
					dst_offset:uint = 0, src_offset:uint = 0):void {
	var old_pos:int = src.position;
	src.position = src_offset;
	src.readBytes(dst,dst_offset,length);
	src.position = old_pos;
}
//设置内存字节流
function memset(dst:ByteArray,val:int,size:int,pos:int=0):void {
	var old_pos:int = dst.position;
	dst.position = pos;
	for (var i:int = 0; i < size; i ++) {
		dst.writeByte(val);
	}
	dst.position = old_pos;
}
//设置内存字节流通过内存字节流
function memsetByByteArray(dst:ByteArray,val:ByteArray,length:uint = 0,
						   dst_offset:uint = 0, val_offset:uint = 0):void {
	var old_pos:int = dst.position;
	dst.position = dst_offset;
	var old_pos1:int = val.position;
	val.position = val_offset;
	
	dst.writeBytes(val,val_offset,length);

	val.position = old_pos1;
	dst.position = old_pos;
}
//写入文本到内存字节流
function writeStringToByteArray(dst:ByteArray,val:String,length:uint):void
{
	var nValLen:int = val.length;
	dst.writeUTFBytes(val);
	for (var i:int = 0; i < (length - nValLen); i ++) {
		dst.writeByte(0);
	}
}
//读取文本从内存字节流
function readStringByByteArray(dst:ByteArray,length:uint):String
{
	if(length > (dst.length - dst.position))
		length =  dst.length - dst.position;
	return dst.readUTFBytes(length);
}
//新建内存字节流
function newLitteEndianByteArray():ByteArray
{
	var result:ByteArray = new ByteArray;
	result.endian = Endian.LITTLE_ENDIAN;
	return result;
}
//断言
function ASSERT(b:Boolean):void
{
	
}
//获取高字节
function HIBYTE(w:uint):uint
{
	return (w & 0x0000ff00) >> 8;
}
//获取低字节
function LOBYTE(w:uint):uint
{
	return (w & 0x000000ff);
}
//判断大小
function __min(a:*,b:*):*
{
	return (((a) < (b)) ? (a) : (b));
}
//判断大小
function __max(a:*,b:*):*
{
	return (((a) > (b)) ? (a) : (b));
}