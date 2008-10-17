if (Drupal.jsEnabled) {
  $(document).ready( function () {
    $('a.toc-toggle').click(function() {
      $('.toc-list').slideToggle();
	  var text = $('a.toc-toggle').text();
	  if (text == 'hide') {
		  $('a.toc-toggle').text('show');
      } else {
		  $('a.toc-toggle').text('hide');
      }
	  return false;
	});
  });
}