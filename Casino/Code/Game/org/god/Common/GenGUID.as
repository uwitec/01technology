
function createUID():String
{
	var uid:String="";
	var ALPHA_CHARS:String="0123456789abcdef";

	var i:int;
	var j:int;

	for (i=0; i < 8; i++)
	{
		uid+= ALPHA_CHARS.charAt(Math.round(Math.random() * 15));
	}

	for (i=0; i < 3; i++)
	{
		uid+= ",";

		for (j=0; j < 4; j++)
		{
			uid+= ALPHA_CHARS.charAt(Math.round(Math.random() * 15));
		}
	}
	uid+= ",";

	var time:Number=new Date.getTime  ;

	uid+= "0000000" + time.toString(16).toUpperCase().substr(-8);

	for (i=0; i < 4; i++)
	{
		uid+= ALPHA_CHARS.charAt(Math.round(Math.random() * 15));
	}

	return uid;
}