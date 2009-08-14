var str;
puplishManagementFLA();

function puplishManagementFLA()
{
	fl.trace("Begin puplishManagementFLA...");
	
	str = "file:///./org/god/GameManagement/Main.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/LogonWindow/LogonWindowModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/RuleWindow/RuleWindowModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/PlazaWindow/PlazaWindowModule.fla";
	publishSingleFLA();
	
	str = "file:///./org/god/GameManagement/PlazaWindow/AccountPanel/AccountPanelModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/PlazaWindow/AccountPanel/Common/AccountPanel_CommonModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/PlazaWindow/BetHistoryPanel/BetHistoryPanelModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/PlazaWindow/BetHistoryPanel/Common/BetHistoryPanel_CommonModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/PlazaWindow/BetMonitorPanel/BetMonitorPanelModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/PlazaWindow/ChartPanel/ChartPanelModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/PlazaWindow/MySelfDataPanel/MySelfDataPanelModule.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameManagement/PlazaWindow/OperationPanel/OperationPanelModule.fla";
	publishSingleFLA();

	fl.trace("End puplishManagementFLA...");
	
	fl.quit();
}
function publishSingleFLA()
{
	var doc = fl.openDocument(str);
	fl.trace(doc.name + "...");
	doc.publish();
	doc.close(false); 
}


