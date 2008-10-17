Drupal.checkPlain = function (text) {
  return text.replace('&', '&amp;')
             .replace('<', '&lt;')
             .replace('"', '&quot;')
             .replace(/^\s+/g, '')
             .replace(/\s+$/g, '')
             .replace('\n', '<br />');
}

Drupal.serialize = function (data, prefix) {
  prefix = prefix || '';
  var out = '';
  for (i in data) {
    var name = prefix.length ? (prefix +'[' + i +']') : i;
    if (out.length) out += '&';
    if (typeof data[i] == 'object') {
      out += Drupal.serialize(data[i], name);
    }
    else {
      out += name +'=';
      out += Drupal.encodeURIComponent(data[i]);
    }
  }
  return out;
}

if (Drupal.jsEnabled) {
  $(document).ready(function () {
    // Note: all tag fields are autocompleted, and have already been initialized at this point.
    $('input.form-tags').each(function () {
      // Hide submit buttons.
      $('input[@type=submit]', this.form).hide();

      // Fetch settings.
      var o = Drupal.settings.communityTags;
      var sequence = 0;

      // Patch for drupal_to_js() bug in Drupal 5.1
      if (typeof o.tags.push == 'undefined') {
        o.tags = [];
      }

      // Show the textfield and empty its value.
      var textfield = $(this).val('').css('display', 'inline');

      // Prepare the add Ajax handler and add the button.
      var addHandler = function () {
        // Send existing tags and new tag string.
        $.post(o.url, Drupal.serialize({ sequence: ++sequence, tags: o.tags, add: textfield[0].value }), function (data) {
          data = Drupal.parseJson(data);
          if (data.status && sequence == data.sequence) {
            o.tags = data.tags;
            updateList();
          }
        });

        // Add tag to local list
        o.tags.push(textfield[0].value);
        o.tags.sort(function (a,b) { a = a.toLowerCase(); b = b.toLowerCase(); return (a>b) ? 1 : (a<b) ? -1 : 0; });
        updateList();
        
        // Clear field and focus it.
        textfield.val('').focus();
      };
      var button = $('<input type="button" class="form-button" value="'+ Drupal.checkPlain(o.add) +'" />').click(addHandler);
      $(this.form).submit(function () { addHandler(); return false; });

      // Prepare the delete Ajax handler.
      var deleteHandler = function () {
        // Remove tag from local list.
        var i = $(this).attr('key');
        o.tags.splice(i, 1);
        updateList();

        // Send new tag list.
        $.post(o.url, Drupal.serialize({ sequence: ++sequence, tags: o.tags, add: '' }), function (data) {
          data = Drupal.parseJson(data);
          if (data.status && sequence == data.sequence) {
            o.tags = data.tags;
            updateList();
          }
        });

        // Clear textfield and focus it.
        textfield.val('').focus();
      };

      // Callback to update the tag list.
      function updateList() {
        list.empty();
        for (i in o.tags) {
          list.append('<li key="'+ Drupal.checkPlain(i) +'">'+ Drupal.checkPlain(o.tags[i]) +'</li>');
        }
        $('li', list).click(deleteHandler);
      }

      // Create widget markup.
      var widget = $('<div class="tag-widget"><ul class="inline-tags clear-block"></ul></div>');
      textfield.before(widget);
      widget.append(textfield).append(button);
      var list = $('ul', widget);

      updateList();
    });
  });
}