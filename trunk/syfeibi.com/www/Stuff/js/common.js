// 显示菜单导航
 $(document).ready(function(){
    var gsjj = "<li><a href='/index.php/About'>关于菲比</a></li><li><a href='/index.php/MakeupIntroduction'>化妆师简介</a></li><li><a href='/index.php/Scenery'>公司内景</a></li>";
    var xxxc = "<li><a href='/index.php/Albums/Hz'>化妆作品展示</a></li><li><a href='/index.php/Albums/Hs'>婚纱作品展示</a></li>";

 	$("#menu_1").hover( 
 						function(){getChildMenu(gsjj);
 						           $("#child_menu").css("display","block");}, 
 					    function(){$("#child_menu").css("display","none");}
 					   );
 	$("#menu_2").hover( 
 						function(){getChildMenu(xxxc);
 						           $("#child_menu").css("display","block");}, 
 					    function(){$("#child_menu").css("display","none");}
 					   );
 	$("#child_menu").hover( 
 						   function(){$(this).css("display","block");
 						              	$("#child_menu li a").mouseover(function(){$(this).fadeOut(100);$(this).fadeIn(500);});
 						             }, 
 					       function(){$(this).css("display","none");}
 					   );		   
 	$("#menu ul li").hover(function(){$(this).fadeOut(100);$(this).fadeIn(500);},
 							 function(){$(this).css("color","");}
 	);
	$('.lightbox a').lightBox(); 	
 	$(document).pngFix(); 				 
 });

function getChildMenu(str) {
 $("#child_menu").empty();
 $("#child_menu").append(str);
}


function showPicAll() {
 $("#show_all").html("<img src='../Stuff/images/album_button_all_sel.png' />");
 $("#show_hz").html("<img src='../Stuff/images/album_button_hz_no.png' />");
 $("#show_hs").html("<img src='../Stuff/images/album_button_hs_no.png' />"); 
 $("#pic_all").attr("style","display:block");
 $("#pic_hz").attr("style","display:none");
 $("#pic_hs").attr("style","display:none"); 
}

function showPicHz() {
 $("#show_all").html("<img src='../Stuff/images/album_button_all_no.png' />");
 $("#show_hz").html("<img src='../Stuff/images/album_button_hz_sel.png' />");
 $("#show_hs").html("<img src='../Stuff/images/album_button_hs_no.png' />"); 
 $("#pic_all").attr("style","display:none");
 $("#pic_hz").attr("style","display:block");
 $("#pic_hs").attr("style","display:none"); 
}

function showPicHs() {
 $("#show_all").html("<img src='../Stuff/images/album_button_all_no.png' />");
 $("#show_hz").html("<img src='../Stuff/images/album_button_hz_no.png' />");
 $("#show_hs").html("<img src='../Stuff/images/album_button_hs_sel.png' />");
 $("#pic_all").attr("style","display:none");
 $("#pic_hz").attr("style","display:none");
 $("#pic_hs").attr("style","display:block"); 
}

function disp_main_pic(obj){
 	$("#show_main_img").html("<img src='"+$("#"+obj).attr("rel")+"' alt='"+$("#"+obj).attr("alt")+"'>");
}
