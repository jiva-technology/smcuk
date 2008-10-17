$(function () {

  // add trigger to the button
  $('span.ignore a').click(function () {
    $(this).parent().hide();
    $('span.unignore').show();
    // use ajax to run a menu_path callback
    $.get(Drupal.settings.basePath + 'readpath/ignore/'+Drupal.settings.nid);

    // it will abort button submit
    return false;
  });

  // add trigger to the button
  $('span.unignore a').click(function () {
    $(this).parent().hide();
    $('span.ignore').show();
    // use ajax to run a menu_path callback
    $.get(Drupal.settings.basePath + 'readpath/unignore/'+Drupal.settings.nid);

    // it will abort button submit
    return false;
  });
});