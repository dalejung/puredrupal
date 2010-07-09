$(document).ready(function() {

	$('.content-in').attr('id','sidebar-right');

	$('.panel-display .panel-pane').each(function() {
	
		var string0 = $(this).attr('class');
		
		var string1 = string0.replace("panel-pane ","");
		var string2 = string1.replace("pane-block ","");
		var string3 = string2.replace("pane-ads ","");
		var string4 = string3.replace("pane-views ","");
		var string5 = string4.replace("pane-custom ","");
		var string6 = string5.replace("pane-panels-mini ","");
		
		var panelClass = string6;
		
		$(this).prepend('<h2 class="panel-class">'+panelClass+'</h2>');
	
	});

});