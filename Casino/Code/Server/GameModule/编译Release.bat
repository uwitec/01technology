del /s B_*.dll V_*.dll L_*.dll P_*.dll CommonModule*.dll GameFrameModule*.dll GameItemListModule*.dll SocketModule*.dll KernelModule*.dll PlazaServer.exe GameServer.exe CenterServer.exe DataBaseTool.exe ServerMonitor.exe  *.obj *.idb *.aps *.res *.plg *.opt *.ncb *.bak *.tmp *.idb *.exp *.ilk *.map *.pdb *.tli *.tlh disasm.asm *.trg *.pch *.~* *.ddp *.lib *.sbr

@set path=C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE;%path%

@cls

devenv "B_Hoe\GameProject\GameServer.vcproj" /build release
devenv "B_Land\GameProject\GameServer.vcproj" /build release
devenv "B_UncoverPig\GameProject\GameServer.vcproj" /build release
devenv "B_BlackJack\GameProject\GameServer.vcproj" /build release
devenv "B_Upgrade\GameProject\GameServer.vcproj" /build release
devenv "B_Sparrow\GameProject\GameServer.vcproj" /build release
devenv "B_ShowHand\GameProject\GameServer.vcproj" /build release
devenv "B_GoldFlower\GameProject\GameServer.vcproj" /build release
devenv "B_SiChuan_Land\GameProject\GameServer.vcproj" /build release

devenv "P_BigGameMachine_Fruit\GameProject\GameServer.vcproj" /build release
devenv "P_BigGameMachine_GuessTrain\GameProject\GameServer.vcproj" /build release
devenv "P_BigGameMachine_HorseRace\GameProject\GameServer.vcproj" /build release
devenv "P_BigGameMachine_LuckyNumber\GameProject\GameServer.vcproj" /build release
devenv "P_BigGameMachine_MonsterLegend\GameProject\GameServer.vcproj" /build release
devenv "P_Slot_CoolFruit\GameProject\GameServer.vcproj" /build release
devenv "P_Slot_MahjongBall\GameProject\GameServer.vcproj" /build release
devenv "P_Slot_Poker\GameProject\GameServer.vcproj" /build release
devenv "P_Slot_Soccer\GameProject\GameServer.vcproj" /build release
devenv "P_Slot_Star97\GameProject\GameServer.vcproj" /build release
devenv "P_TableGame_Baccarat\GameProject\GameServer.vcproj" /build release
devenv "P_TableGame_BlackJack\GameProject\GameServer.vcproj" /build release
devenv "P_TableGame_DragonTiger\GameProject\GameServer.vcproj" /build release
devenv "P_TableGame_RedDog\GameProject\GameServer.vcproj" /build release
devenv "P_TableGame_Roulette\GameProject\GameServer.vcproj" /build release
devenv "P_TableGame_Sicbo\GameProject\GameServer.vcproj" /build release
devenv "P_TableGame_War\GameProject\GameServer.vcproj" /build release
devenv "P_VideoPoker_5PK\GameProject\GameServer.vcproj" /build release
devenv "P_VideoPoker_5PK1\GameProject\GameServer.vcproj" /build release
devenv "P_VideoPoker_5PK2\GameProject\GameServer.vcproj" /build release
devenv "P_VideoPoker_7PK\GameProject\GameServer.vcproj" /build release

pause;

