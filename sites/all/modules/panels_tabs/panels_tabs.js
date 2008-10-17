// $Id: panels_tabs.js,v 1.1.2.3 2008/01/02 16:51:31 wimleers Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function(){ 
    for (var id in Drupal.settings.panelsTabs) {
      var $tabs = $('#'+ id +' ul li');

      switch (Drupal.settings.panelsTabs[id].fillingTabs) {
        case 'equal':
          $tabs.each(function() {
            $(this)
            .css('width', (99 / $tabs.length) +'%')
            .css('overflow', 'hidden');
          });
          break;

        case 'smart':
          var tabTextLengths = new Array();
          var totalTabTextLength = 0;

          $tabs
          // Collect the length of the text on each tab and the total length.
          .each(function(i) {
            totalTabTextLength += tabTextLengths[i] = $(this).text().length;
          })
          // Use the collected text lenghts to do smart tab width scaling.
          .each(function(i) {
            $(this)
            .css('width', (tabTextLengths[i] / totalTabTextLength * 100) +'%')
            .css('overflow', 'hidden');
          });
          break;
        
        case 'disabled':
        default:
          // Nothing should happen if we 
          break;
      }
    }
  });
}
