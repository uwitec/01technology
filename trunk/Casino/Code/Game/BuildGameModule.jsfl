var str;
puplishGameFLA();

function puplishGameFLA()
{
	fl.trace("Begin puplishGameFLA...");
	
	str = "file:///./org/god/GameModule/B_BlackJack/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/B_GoldFlower/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/B_Hoe/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/B_Land/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/B_ShowHand/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/B_Sparrow/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/B_UncoverPig/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/B_Upgrade/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/B_SiChuan_Land/ModuleMain.fla";
	publishSingleFLA();
	
	str = "file:///./org/god/GameModule/P_BigGameMachine_Fruit/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_BigGameMachine_GuessTrain/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_BigGameMachine_HorseRace/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_BigGameMachine_LuckyNumber/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_BigGameMachine_MonsterLegend/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_Slot_CoolFruit/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_Slot_MahjongBall/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_Slot_Poker/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_Slot_Soccer/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_Slot_Star97/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_TableGame_Baccarat/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_TableGame_BlackJack/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_TableGame_DragonTiger/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_TableGame_RedDog/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_TableGame_Roulette/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_TableGame_Sicbo/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_TableGame_War/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_VideoPoker_5PK/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_VideoPoker_5PK1/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_VideoPoker_5PK2/ModuleMain.fla";
	publishSingleFLA();
	str = "file:///./org/god/GameModule/P_VideoPoker_7PK/ModuleMain.fla";
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


