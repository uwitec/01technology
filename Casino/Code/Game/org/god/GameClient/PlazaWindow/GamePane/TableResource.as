package 
{
	import flash.display.Sprite;

	import org.aswing.geom.IntRectangle;

	//游戏桌资源
	public class TableResource
	{
		public function TableResource()
		{
		}
		public static function getTableViewRect(wProcessType:uint,
		 wChairCount:uint):IntRectangle
		{
			if (IsVideoGameProcessType(wProcessType))
			{
				return new IntRectangle(0,0,0,0);
			}
			else
			{
				return new IntRectangle(0,0,162,172);
			}
		}
	
		public static function getTableViewMC(wProcessType:uint,
		 wChairCount:uint):Sprite
		{
			if (IsVideoGameProcessType(wProcessType))
			{
				return null;
			}
			else
			{
				switch (wChairCount)
				{
					case 1 :
						{
							return new Table01  ;

						};
					case 2 :
						{
							return new Table02  ;

						};
					case 3 :
						{
							return new Table03  ;

						};
					case 4 :
						{
							return new Table04  ;

						};
					case 5 :
						{
							return new Table05  ;

						};
					case 6 :
						{
							return new Table06  ;

					}

				};
				return null;
			}
		}//getTableViewMC
		public static function getTable01_PeopleRect(wChairID:uint):IntRectangle
		{
			return new IntRectangle(44,54,60,80);
		}
		public static function getTable02_PeopleRect(wChairID:uint):IntRectangle
		{
			switch(wChairID)
			{
				case 0:
				{
					return new IntRectangle(5,25,60,80);
				}
				case 1:
				{
					return new IntRectangle(96,25,60,80);
				}
			}
			return new IntRectangle(0,0,0,0);
		}
		public static function getTable03_PeopleRect(wChairID:uint):IntRectangle
		{
			switch(wChairID)
			{
				case 0:
				{
					return new IntRectangle(14,32,60,80);
				}
				case 1:
				{
					return new IntRectangle(82,14,60,80);
				}
				case 2:
				{
					return new IntRectangle(81,67,60,80);
				}			
			}
			return new IntRectangle(0,0,0,0);
		}		
		public static function getTable04_PeopleRect(wChairID:uint):IntRectangle
		{
			switch(wChairID)
			{
				case 0:
				{
					return new IntRectangle(6,26,60,80);
				}
				case 1:
				{
					return new IntRectangle(48,6,60,80);
				}
				case 2:
				{
					return new IntRectangle(98,27,60,80);
				}	
				case 3:
				{
					return new IntRectangle(50,58,60,80);
				}	
			}
			return new IntRectangle(0,0,0,0);
		}				
		public static function getTable05to06_PeopleRect(wChairID:uint):IntRectangle
		{
			switch(wChairID)
			{
				case 0:
				{
					return new IntRectangle(13,29,60,80);
				}
				case 1:
				{
					return new IntRectangle(41,13,60,80);
				}
				case 2:
				{
					return new IntRectangle(85,15,60,80);
				}	
				case 3:
				{
					return new IntRectangle(115,30,60,80);
				}	
				case 4:
				{
					return new IntRectangle(82,66,60,80);
				}	
				case 5:
				{
					return new IntRectangle(30,67,60,80);
				}	
	
			}
			return new IntRectangle(0,0,0,0);
		}						
		public static function getTableViewPeopleRect(wProcessType:uint,
		 wChairCount:uint,
		 wChairID:uint):IntRectangle
		{
			if (IsVideoGameProcessType(wProcessType))
			{
				return new IntRectangle(0,0,0,0);
			}
			else
			{
				switch (wChairCount)
				{
					case 1 :
						{
							return getTable01_PeopleRect(wChairID);

						};
					case 2 :
						{
							return getTable02_PeopleRect(wChairID);

						};
					case 3 :
						{
							return getTable03_PeopleRect(wChairID);

						};
					case 4 :
						{
							return getTable04_PeopleRect(wChairID);

						};
					case 5 :
					case 6 :
						{
							return getTable05to06_PeopleRect(wChairID);

						};
					
				}
	
			}
			return new IntRectangle(0,0,0,0);
		}//getTableViewPeopleRect
		
		public static function getTableViewPeopleMC(wProcessType:uint,
		 wChairCount:uint,
		 wChairID:uint,
		 cbFace:uint):Sprite
		{
			if (IsVideoGameProcessType(wProcessType))
			{
				return null;
			}
			else
			{
				var isBoy:Boolean =  (cbFace / 2) ==  0 ? true:false;
				if(wChairCount == 1)
				{
					if (isBoy)
					{
						return new SB10;
					}
					else
					{
						return new SG10;
					}
				}
				else if (wChairCount == 2)
				{
					switch (wChairID)
					{
						case 0 :
						{
							if (isBoy)
							{
								return new SB01  ;
							}
							else
							{
								return new SG01  ;
							}

						};
						case 1 :
						{
							if (isBoy)
							{
								return new SB00  ;
							}
							else
							{
								return new SG00  ;
							}

						}
					}
				}
				else if (wChairCount == 3)
				{
					switch (wChairID)
					{
						case 0 :
						{
							if (isBoy)
							{
								return new SB04  ;
							}
							else
							{
								return new SG04  ;
							}

						}
						case 1 :
						{
							if (isBoy)
							{
								return new SB05  ;
							}
							else
							{
								return new SG05  ;
							}

						}
						case 2:
						{
							if (isBoy)
							{
								return new SB06  ;
							}
							else
							{
								return new SG06  ;
							}

						}
					}
				}
				else if (wChairCount == 4)
				{
					switch (wChairID)
					{
						case 0 :
						{
							if (isBoy)
							{
								return new SB01  ;
							}
							else
							{
								return new SG01  ;
							}

						}
						case 1 :
						{
							if (isBoy)
							{
								return new SB02  ;
							}
							else
							{
								return new SG02  ;
							}

						}
						case 2:
						{
							if (isBoy)
							{
								return new SB00  ;
							}
							else
							{
								return new SG00  ;
							}

						}
						case 3:
						{
							if (isBoy)
							{
								return new SB03  ;
							}
							else
							{
								return new SG03  ;
							}

						}
					}
				}				
				else if (wChairCount == 5)
				{
					switch (wChairID)
					{
						case 0 :
						{
							if (isBoy)
							{
								return new SB04  ;
							}
							else
							{
								return new SG04  ;
							}

						}
						case 1 :
						{
							if (isBoy)
							{
								return new SB07  ;
							}
							else
							{
								return new SG07  ;
							}

						}
						case 2:
						{
							if (isBoy)
							{
								return new SB05  ;
							}
							else
							{
								return new SG05  ;
							}

						}
						case 3:
						{
							if (isBoy)
							{
								return new SB08  ;
							}
							else
							{
								return new SG08  ;
							}

						}
						case 4:
						{
							if (isBoy)
							{
								return new SB06  ;
							}
							else
							{
								return new SG06  ;
							}

						}						
					}
				}	
				else if (wChairCount == 5 ||
						 wChairCount == 6)
				{
					switch (wChairID)
					{
						case 0 :
						{
							if (isBoy)
							{
								return new SB04  ;
							}
							else
							{
								return new SG04  ;
							}

						}
						case 1 :
						{
							if (isBoy)
							{
								return new SB07  ;
							}
							else
							{
								return new SG07  ;
							}

						}
						case 2:
						{
							if (isBoy)
							{
								return new SB05  ;
							}
							else
							{
								return new SG05  ;
							}

						}
						case 3:
						{
							if (isBoy)
							{
								return new SB08  ;
							}
							else
							{
								return new SG08  ;
							}

						}
						case 4:
						{
							if (isBoy)
							{
								return new SB06  ;
							}
							else
							{
								return new SG06  ;
							}

						}						
						case 5:
						{
							if (isBoy)
							{
								return new SB09  ;
							}
							else
							{
								return new SG09  ;
							}

						}						
					}
				}							

				
			}
			return null;
		}//getTableViewPeopleMC
	}
}
include "../../../Net/GLOBALDEF.as"
