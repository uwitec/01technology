del /s B_*.dll V_*.dll L_*.dll P_*.dll CommonModule*.dll GameFrameModule*.dll GameItemListModule*.dll SocketModule*.dll KernelModule*.dll PlazaServer.exe GameServer.exe CenterServer.exe DataBaseTool.exe ServerMonitor.exe  *.obj *.idb *.aps *.res *.plg *.opt *.ncb *.bak *.tmp *.idb *.exp *.ilk *.map *.pdb *.tli *.tlh disasm.asm *.trg *.pch *.~* *.ddp *.lib *.sbr

@set path=C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE;%path%

@cls

devenv "ShareModule\GridCtrl\GridCtrl.vcproj" /build debug
devenv "ShareModule\CommonModule\CommonModule.vcproj" /build debug
devenv "ShareModule\SocketModule\SocketModule.vcproj" /build debug
           
devenv "ServerModule\GameItemListModule\GameItemListModule.vcproj" /build debug
devenv "ServerModule\KernelModule\KernelModule.vcproj" /build debug
devenv "ServerModule\GameFrameModule\GameFrameModule.vcproj" /build debug
devenv "ServerModule\CenterServer\CenterServer.vcproj" /build debug
devenv "ServerModule\PlazaServer\PlazaServer.vcproj" /build debug
devenv "ServerModule\GameServer\GameServer.vcproj" /build debug

devenv "Tool\DataBaseTool\DataBaseTool.vcproj" /build debug
devenv "Tool\ServerMonitor\ServerMonitor.vcproj" /build debug

devenv "GameModule\B_Hoe\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\B_Land\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\B_UncoverPig\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\B_BlackJack\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\B_Upgrade\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\B_Sparrow\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\B_ShowHand\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\B_GoldFlower\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\B_SiChuan_Land\GameProject\GameServer.vcproj" /build debug

devenv "GameModule\P_BigGameMachine_Fruit\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_BigGameMachine_GuessTrain\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_BigGameMachine_HorseRace\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_BigGameMachine_LuckyNumber\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_BigGameMachine_MonsterLegend\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_Slot_CoolFruit\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_Slot_MahjongBall\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_Slot_Poker\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_Slot_Soccer\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_Slot_Star97\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_TableGame_Baccarat\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_TableGame_BlackJack\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_TableGame_DragonTiger\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_TableGame_RedDog\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_TableGame_Roulette\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_TableGame_Sicbo\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_TableGame_War\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_VideoPoker_5PK\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_VideoPoker_5PK1\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_VideoPoker_5PK2\GameProject\GameServer.vcproj" /build debug
devenv "GameModule\P_VideoPoker_7PK\GameProject\GameServer.vcproj" /build debug

pause;

