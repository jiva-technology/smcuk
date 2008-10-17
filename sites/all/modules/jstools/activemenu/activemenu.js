// $Id: activemenu.js,v 1.19.2.8 2007/08/08 21:26:08 nedjo Exp $

Drupal.behaviors.activeMenu = function (context) {
  // The elements supported. Each can designate a different uri.
  var menus = Drupal.settings.activemenu;

  for (var menu in menus) {
    $(menu + ' li.expanded:not(.activemenu-processed)', context).each(function () {
      Drupal.preventSelect(this);
      $(this)
        .click(function (e) {
          Drupal.activemenuToggle(this, e);
        })
        .addClass('activemenu-processed')
      });
    $(menu + ' li.collapsed:not(.activemenu-processed)', context).each(function() {
      if ($(this).children('ul').length > 0) {
        return;
      }
      var path = Drupal.getPath($('a:first', this).attr('href'));
      var url = Drupal.url(menus[menu]);
      var elt = this;
      Drupal.preventSelect(this);
      $(this)
        .click(function (e) {
          var offset = Drupal.mousePosition(e).x - Drupal.absolutePosition(this).x;
          var padding = $(this).css('padding-left');
          // Determine if we are in the selection area.
          if (offset < (padding.slice(-2) == "px" ? parseInt(padding) : 18)) {
            $(elt).addClass('loading');
            // Ajax GET request for autocompletion
            $.ajax({
              type: 'POST',
              url: url,
              data: 'path=' + path,
              success: function (data) {
                $(elt).removeClass('loading');
                if ($(elt).children('ul').length > 0) {
                  return;
                }
                data = Drupal.parseJson(data);
                $(elt)
                  .append(data.content)
                  .removeClass('collapsed')
                  .addClass('expanded')
                  .unclick()
                  .click(function (e) {
                    Drupal.activemenuToggle(this, e);
                  })
                  .find('ul:first')
                  .slideDown(200);
                Drupal.attachBehaviors(elt);
              },
              error: function (xmlhttp) {
                alert('An HTTP error '+ xmlhttp.status +' occured.\n' + url);
              }
            });
            return false;
          }
        })
        .addClass('activemenu-processed');
    });
  }
};

Drupal.activemenuToggle = function (menu, e) {
  // Only toggle if this is the element that was clicked.
  // Otherwise, a parent li element might be toggled too.
  // Don't animate multiple times.
  if (menu == Drupal.getTarget(e) && !menu.animating) {
    if ($(menu).is(".collapsed")) {
      $(menu)
        .removeClass('collapsed')
        .addClass('expanded')
        .find('ul:first')
        .slideDown(200, function(){
          this.parentNode.animating = false;
        });
    } 
    else {
      $(menu)
        .removeClass('expanded')
        .addClass('collapsed')
        .find('ul:first')
        .slideUp(200, function(){
          this.parentNode.animating = false;
        });
    }
    menu.animating = true;
  }
};

