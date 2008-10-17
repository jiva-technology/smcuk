INSTALLATION

Copy the readpath module to your sites/all/modules directory and enable it at
?q=admin/build/modules

Add this to your theme, changing "garland" to your theme's name:

function garland_box($edit, $title = NULL) {
  if (module_exists('readpath') && $edit == t('Post new comment') && readpath_allowed_type(arg(1)) && !readpath_on_last_page()) {
    return;
  }
  else {
    return phptemplate_box($edit, $title);
  }
}