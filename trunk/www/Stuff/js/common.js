 jQuery(function($) {
	$("#menu>li").hover(
	  function () {
	    $(this).addClass("menu_sel");
	  },
	  function () {
	    $(this).removeClass("menu_sel");
	  }
	); 
 }); 
 function showPic(disp, hide){
   document.getElementById(disp).className = "show_list";
   document.getElementById(hide).className = "hide_list";
 }
