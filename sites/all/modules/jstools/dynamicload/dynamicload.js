// $Id: dynamicload.js,v 1.10.2.8 2007/10/08 21:12:26 sun Exp $

Drupal.behaviors.dynamicload = function (context) {
  // Set an initial value for pending script loads.
  Drupal.settings.dynamicload.pending = 0;
  // Add classes to region wrappers.
  $('div.dynamicload-region').each(function () {
    var regionId = $(this).attr('id');
    $(this)
      .parent()
      .addClass(regionId)
      .end()
      .remove();
  });
  // Automatically detect main content wrapper if the default setting is not available.
  if (!$(Drupal.settings.dynamicload.settings).size()) {
    Drupal.settings.dynamicload.content = $('#dynamicload-content-marker')
      .parent()
      .parent()
      .addClass('dynamicload-content').get(0).nodeName + '.dynamicload-content';
  }
  for (var key in Drupal.settings.dynamicload.settings) {
    var selector = Drupal.settings.dynamicload.settings[key].selector;
    var target = Drupal.settings.dynamicload.settings[key].target;
    // If the target is the main content area, set its selector.
    if (target == 'content') {
      target = Drupal.settings.dynamicload.content;
    }
    // If the target is a region, set its selector.
    else if (target.indexOf('region-') == 0) {
      var region = target.substring(7);
      target = '.dynamicload-region-' + region;
    }
    // Only proceed if both source and target elements exist.
    if (Drupal.settings.dynamicload.settings[key].apply && $(selector).size() && ((target == '') || $(target).size())) {
      // Apply to elements that are themselves 'a' elements or to their descendent 'a' elements.
      $(selector).filter('a[@href]').add(selector + ' a[@href]').each(function () {
        Drupal.dynamicload(this, {'target': target == '' ? false : $(target).get(0), 'selector': selector, method: parseInt(Drupal.settings.dynamicload.settings[key].method)});
      });
    }
    if (Drupal.settings.dynamicload.settings[key].refresh && $(selector).length) {
      $(selector).each(function () {
        var timeoutId = window.setTimeout('Drupal.dynamicloadBlockTimeout("' + Drupal.settings.dynamicload.settings[key].module + '", ' + Drupal.settings.dynamicload.settings[key].delta + ', "' + Drupal.settings.dynamicload.settings[key].selector + '")', Drupal.settings.dynamicload.settings[key].refresh_interval);
        $(window).unload(function () {clearTimeout(timeoutId);});
      });
    }
  }
  if (Drupal.settings.dynamicload.all) {
    // Setup tasks to be run only on initial page load.
    if (!Drupal.settings.dynamicload.init) {
      // If there is a hash, load it. This enables bookmarking.
      if (window.location.hash.length > 1) {
        var a = document.createElement('a');
        var url = location.hash.replace('#', '').split('?');
        a.setAttribute('href', Drupal.url(url[0], url[1] ? url[1] : null));
        Drupal.dynamicload(a, {useClick: false});
      }
      // Otherwise, set initial path.
      else {
        var path = Drupal.getPath(window.location.href);
        var match = Drupal.pathMatch(path, Drupal.settings.dynamicload.paths, Drupal.settings.dynamicload.paths_type);
        if (path != '/' && path != '' && !match) {
          window.location.href = window.location.protocol + '//' + window.location.hostname + Drupal.settings.jstools.basePath + '#' + path;
        }
        // If we're on an excluded page, stop processing.
        else if (match) {
          return;
        }
      }
    }
    // Process all links.
    $('a[@href]:not(.dynamicload-processed)').each(function () {
      Drupal.dynamicload(this);
    });
  }
  else {
    $('a[@href].dynamicload:not(.dynamicload-processed), .dynamicload a[@href]:not(.dynamicload-processed)').each(function () {
      Drupal.dynamicload(this);
    });
  }
  if (!Drupal.settings.dynamicload.init) {
    $.ajaxHistory.initialize();
    Drupal.settings.dynamicload.init = true;
  }
};

Drupal.dynamicloadPager = function (elt) {
  elt = elt ? elt : document;
  $(elt).find('div.pager a').each(function () {
    var target = $(Drupal.settings.dynamicload.content).length ? $(Drupal.settings.dynamicload.content).get(0) : $(this).prev();
    Drupal.dynamicload(this, {'target': target, 'success': Drupal.dynamicloadPager});
  });
};

Drupal.dynamicload = function (elt, settings) {
  settings = jQuery.extend({
     useClick: true,
     target: $(Drupal.settings.dynamicload.content)[0],
     selector: Drupal.settings.dynamicload.content,
     method: 1,
     success: null,
     show: true
  }, settings);

  var href = $(elt).addClass('dynamicload-processed').attr('href');
  var path = Drupal.getPath(href);
  // Only process internal site links. Skip links that already have hashes.
  if ((path == '') || (href.indexOf(Drupal.settings.jstools.basePath) == -1) || (href.indexOf('#') > -1) || Drupal.pathMatch(path, Drupal.settings.dynamicload.paths, Drupal.settings.dynamicload.paths_type)) {
    return;
  }
  // Prepend to the path.
  settings.url = Drupal.prependPath(href, 'dynamicload/js');

  var query = Drupal.getQueryString(href);
  var hash = '#' + Drupal.getPath(href.replace(/\s/g, '+')) + (query ? '?' + query : '');

  if (settings.useClick) {
    $(elt).attr('href', hash).click(function(e) {
      // Add to history only if true click occured, not a triggered click.
      if (e.clientX) {
        $.ajaxHistory.update(hash, function () {
          Drupal.dynamicloadLoad(settings);
        });
        Drupal.dynamicloadLoad(settings);
      }
    });
  }
  else {
    $.ajaxHistory.update(hash, function () {
      Drupal.dynamicloadLoad(settings);
    });
    Drupal.dynamicloadLoad(settings);
  }
};

Drupal.dynamicloadLoad = function (settings) {

  if (settings.method == 1) {
    var cachedContent = $(settings.target).html();
    // Insert progressbar.
    var progress = new Drupal.progressBar('dynamicloadprogress');
    progress.setProgress(-1, 'Fetching page');
    var progressElt = progress.element;
    $(progressElt).css({
      width: '250px',
      height: '15px',
      paddingTop: '10px'
    });
    $(settings.target)
       .html('')
       .append(progressElt);
  }
  $.ajax({
    type: 'POST',
    data: 'selector=' + settings.selector,
    url: settings.url,
    success: function(response){
      $(progressElt).remove();
      progress = null;
      response = Drupal.parseJson(response);
      if (response.result) {
        if (settings.show) {
          $(settings.target).hide();
        }
        // Special handling for loading into the main content area.
        if (settings.selector == Drupal.settings.dynamicload.content) {
          // Prepend content with the 'extra' (breadcrumb, title, help, tabs...).
          response.content = response.extra + response.content;
          // Set document title
          document.title = response.title;
          // If region data set, refresh all regions.
          if (response.regions) {
            for (var region in response.regions) {
              $('.dynamicload-region-' + region).html(response.regions[region]);
            }
          }
        }

        switch (settings.method) {
          case 1:
            $(settings.target).html(response.content);
            break;
          case 2:
            $(settings.target).append(response.content);
            break;
          case 3:
            $(settings.target).prepend(response.content);
            break;
          case 4:
            $(settings.target).after(response.content);
            break;
          case 5:
            $(settings.target).before(response.content);
            break;
        }
        if (settings.show) {
          $(settings.target).animate({height: 'show', opacity: 'show'}, 700);
        }

        // Update Drupal.settings data.
        if (response.scripts.setting) {
          Drupal.settings = $.extend(Drupal.settings, response.scripts.setting);
        }
        // Attach behaviours at once. We will call this again when new JS files are loaded.
        Drupal.attachBehaviors(settings.target);
        // Load JS and CSS files.
        Drupal.dynamicloadFiles(response, settings.target);
      }
      else {
        var message = response.message ? response.message : 'Unable to load page.';
        alert(message);
        $(settings.target)
          .html(cachedContent);
      }
      if (settings.success) {
        settings.success(settings);
      }
    }
  });

};

Drupal.dynamicloadFiles = function (response, target) {
  // Handle scripts.
  // TODO: handle inline scripts.
  var types = ['core', 'module', 'theme'];
  for (var i in types) {
    for (var src in response.scripts[types[i]]) {
      // Load scripts.
      src = Drupal.settings.jstools.basePath + src;
      // Test if the script already exists.
      if (!$('script[@src=' + src + ']').size()) {
        $.getScript(src, function () {
          Drupal.dynamicloadComplete(target);
        });
        Drupal.settings.dynamicload.pending++;
      }
    }
  }
  // Handle stylesheets.
  var types = ['module', 'theme'];
  for (var media in response.css) {
    for (var i in types) {
      for (var src in response.css[media][types[i]]) {
        src = Drupal.settings.jstools.basePath + src;
        // Test if the stylesheet already exists.
        if (!$('style:contains(' + src + ')').size()) {
          $('<style type="text/css" media="' + media + '">@import "' + src + '";</style>').appendTo('head');
        }
      }
    }
  }
};

Drupal.dynamicloadComplete = function(target) {
  Drupal.settings.dynamicload.pending--;
  if (Drupal.settings.dynamicload.pending == 0) {
    Drupal.attachBehaviors(target);
  }
};

Drupal.dynamicloadBlockTimeout = function (module, delta, selector) {
  var timeoutId = window.setTimeout('Drupal.dynamicloadBlockTimeout("' + module + '", ' + delta + ', "' + selector + '")', Drupal.settings.dynamicload.settings[module + '_' + delta].refresh_interval);
  $(window).unload(function () {clearTimeout(timeoutId);});
  Drupal.dynamicloadLoadBlock(module, delta, selector);
};

Drupal.dynamicloadLoadBlock = function (module, delta, selector) {
  $.ajax({
    type: 'GET',
    url: Drupal.url('dynamicload/block/' + module + '/' + delta),
    success: function(response){
      response = Drupal.parseJson(response);
      if (response.result) {
        // If there are list items and we're set to scroll, fade + slide in the new ones and fade + slide out the old ones.
        var count = $(selector + ' div.item-list li').size();
        if (Drupal.settings.dynamicload.settings[module + '_' + delta].scroll && count) {

          $('<div id="dynamicload-newblock"></div>')
            .appendTo(selector)
            .hide()
            .append(response.content)
            .each(function () {
              var newItems = $(this).find('div.item-list ul:first > li').addClass('is-new').hide();
            })
            .parent()
            .find('div.item-list ul:first')
            .prepend(newItems)
            .find('> li:not(.is-new)')
            .addClass('is-old')
            .each(function () {
              var oldElt = this;
              var oldHref = href = $(this).find('a:first').attr('href');
              $(this).siblings('.is-new').each(function () {
                if ($(this).find('a:first').attr('href') == oldHref) {
                  $(oldElt).removeClass('is-old').html($(this).html());
                  $(this).remove();
                }
              })
            })
            .end()
            .find('li.is-new')
            .animate({height: 'show', opacity: 'show'}, 700)
            .end()
            .find('li.is-old')
            .animate({height: 'hide', opacity: 'hide'}, 700, function() {
              $(this).remove();
            })
            .end()
            .next('#dynamicload-newblock')
            .remove();
        }
        // Otherwise simply replace the existing block.
        else {
          $(selector)
            .after(response.content)
            .remove();
        }
      }
    }
  });
};
