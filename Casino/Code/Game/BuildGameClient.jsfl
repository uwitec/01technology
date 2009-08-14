var str;
puplishGameFLA();

function puplishGameFLA()
{
	fl.trace("Begin puplishGameFLA...");
	
	str = "file:///./org/god/GameClient/Main.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameClient/LogonWindow/LogonWindowModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameClient/RuleWindow/RuleWindowModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameClient/PlazaWindow/PlazaWindowModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameClient/PlazaWindow/Dialog_Common/PlazaWindow_CommonModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameClient/PlazaWindow/BetHistoryPanel_Common/BetHistoryPanel_CommonModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameClient/PlazaWindow/GamePane/GamePaneModule.fla";
	publishSingleFLA();

	fl.trace("End puplishGameFLA...");
	
	fl.quit();
}
function publishSingleFLA()
{
	var doc = fl.openDocument(str);
	fl.trace(doc.name + "...");
	doc.publish();
	doc.close(false); 
}


