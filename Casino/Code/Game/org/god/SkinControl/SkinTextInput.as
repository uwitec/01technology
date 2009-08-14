package org.god.SkinControl
{
	import fl.controls.TextInput;
	
	public class SkinTextInput extends TextInput
	{
		public function SkinTextInput(xp:int,yp:int,
		   bPasswd:Boolean = false,
		   nMaxChars:int = 31)
		{
			x = xp;
			y = yp;
			displayAsPassword = bPasswd;
			maxChars = nMaxChars;
		}
	}
}