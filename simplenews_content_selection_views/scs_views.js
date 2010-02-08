$(document).ready(function(){
	$('input[@type=checkbox]').each(
		function () {
			if (this.id) {
				$(this).bind('click', function() {
					var nodes = '';
					$('input[@type=checkbox]').each(
						function () {
							if (this.id && this.checked) {
								id = this.id;
								id = id.split('-');
								id = id[2];
								nodes += id + '-';
							}
						}
					);
					
					var scs_ajax_return = function(data) {
						console.log(data);
					}
					
					//DO THE AJAX
					$.get('scs_views/nodeselected', 'nid=' + nodes, scs_ajax_return);
				});
			}
		}
	);
});