// $Id: tabs.js,v 1.2.2.8 2007/08/08 21:26:10 nedjo Exp $

Drupal.behaviors.tabs = function (context) {

  // Process custom tabs.
  $('.drupal-tabs:not(.tabs-processed)', context)
    .addClass('tabs-processed')
    .addClass('tabs')
    .tabs({
      fxFade: Drupal.settings.tabs.fade,
      fxSlide: Drupal.settings.tabs.slide,
      fxSpeed: Drupal.settings.tabs.speed,
      fxAutoHeight: Drupal.settings.tabs.auto_height,
      onShow: Drupal.tabsAddClassesCallback()
    })
    .show()
    .find('ul.anchors')
    .addClass('tabs')
    .each(function () {
      var newClass = $(this).parents('.drupal-tabs').size() ? 'secondary' : 'primary';
      $(this).addClass(newClass);
    });
  Drupal.tabsAddClasses();
};

Drupal.tabsAddClassesCallback = function() {
  return function() {
    Drupal.tabsAddClasses();
  }
};

Drupal.tabsAddClasses = function() {
  $('ul.anchors.tabs.primary')
    .find('.active')
    .removeClass('active')
    .end()
    .find('.tabs-selected')
    .addClass('active');
};

Drupal.tabsLocalTasks = function(elt) {
  var elt = elt ? elt : document;
  // Process standard Drupal local task tabs.
  // Only proceed if we have dynamicload available.
  if (Drupal.settings && Drupal.settings.tabs && Drupal.dynamicload && typeof(Drupal.dynamicload == 'function')) {

    $(elt).find('ul.tabs.primary')
      .each(function() {
        var index = 1;
        var activeIndex;
        $(this).addClass('anchors')
        .addClass('tabs-processed')
        .find('li > a')
        .each(function () {
          var div = document.createElement('div');
          $(div)
            .attr('id', 'section-' + index)
            .addClass('fragment');
          var parentDiv = $(this).parents('div').get(0);
          parentDiv.appendChild(div);
          // The active tab already has content showing.
          if ($(this).is('.active')) {
            activeIndex = parseInt(index);
          }
          // Other tabs need to load their content.
          else {
            Drupal.dynamicload(this, {
              target: document.getElementById('section-' + index),
              useClick: false,
              show: false
            });
          }
          $(this).attr('href', '#section-' + index);
          index++;
        })
        .end()
        .parent('div')
        .each(function() {
          while (this.nextSibling) {
            var oldDiv = this.parentNode.removeChild(this.nextSibling);
            document.getElementById('section-' + activeIndex).appendChild(oldDiv);
          }
        })
        .tabs({
          onShow: Drupal.tabsAddClassesCallback()
        });
    });
  }
};
