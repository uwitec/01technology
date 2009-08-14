import flash.utils.ByteArray;
//转换地址
function inet_ntoa(dwServerAddr:uint):String
{
	var b0:uint = dwServerAddr & 0x000000ff;
	var b1:uint = (dwServerAddr >> 8) & 0x000000ff;
	var b2:uint = (dwServerAddr >> 16)& 0x000000ff;
	var b3:uint = (dwServerAddr >> 24)& 0x000000ff;
	var result:String = b0 + "." + b1 + "." + b2 + "." + b3;
	return result;
}