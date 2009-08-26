$(document).ready(function(){
	var Idx = 1;
	var IntervalKey;
	var ChangeImage = function(){
		//If the image still animating, stop it and start the new one
		$("#MainImage").ImageStop(true,true);
		$("#MainImage").ImageSwitch({NewImage: $(".TnImage").eq(Idx).attr("src").replace("tn-","med-")});
		//alert($(".TnImage").eq(Idx).attr("src"));		
		//Mark which thumbnail is displaying
		$(".TnImage").css("opacity","0.5");
		$(".TnImage").eq(Idx).css("opacity","1");
		//Set the next image will be display
		Idx++;
		if(Idx>1){
			Idx = 0;
		}
		//Start preload the next image
		$.ImagePreload($(".TnImage").eq(Idx).attr("src").replace("tn-","med-"));			
	};
	//When a thumbnail's clicked
	$(".TnImage").click(function(){
		Idx = $(".TnImage").index(this);
		ChangeImage();
	});		
	
	var StartSlideShow = function(){
		IntervalKey = setInterval(ChangeImage,10000);
		$("#SlideShow").text("Stop Slide show");
		$("#SlideShow").unbind("click");
		$("#SlideShow").bind("click",StopSlideShow);					
	};
	
	var StopSlideShow = function(){
		clearInterval(IntervalKey);
		$("#SlideShow").text("Start Slide show");
		$("#SlideShow").unbind("click");
		$("#SlideShow").bind("click",StartSlideShow);					
	};
	
	$("#SlideShow").bind("click",StopSlideShow);
	IntervalKey = setInterval(ChangeImage,10000);
	$("#DisplayDiv").bind("mouseover", function(){clearInterval(IntervalKey);});
	$("#DisplayDiv").bind("mouseout", function(){IntervalKey = setInterval(ChangeImage,10000);});		
});
