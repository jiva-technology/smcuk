<?php
// $Id: template.php,v 1.2 2007/08/23 22:55:46 roopletheme Exp $

if (is_null(theme_get_setting('litejazz_style'))) {
  global $theme_key;
  // Save default theme settings
  $defaults = array(
    'litejazz_style' => 0,
    'litejazz_width' => 0,
	'litejazz_fixedwidth' => '850',
    'litejazz_breadcrumb' => 0,
	'litejazz_iepngfix' => 0,
    'litejazz_themelogo' => 0,
  );

  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge(theme_get_settings($theme_key), $defaults)
  );
  // Force refresh of Drupal internals
  theme_get_setting('', TRUE);
}

function litejazz_regions() {
  return array(
       'sidebar_left' => t('left sidebar'),
       'sidebar_right' => t('right sidebar'),
       'content_top' => t('content top'),
       'content_bottom' => t('content bottom'),
       'header' => t('header'),
	   'user1' => t('user1'),
	   'user2' => t('user2'),
	   'user3' => t('user3'),
	   'user4' => t('user4'),
	   'user5' => t('user5'),
	   'user6' => t('user6'),
       'footer' => t('footer')
  );
} 
 
$style = theme_get_setting('litejazz_style');
if (!$style)
{
   $style = 'blue';
}

drupal_add_css(drupal_get_path('theme', 'litejazz') . '/css/' . $style . '.css', 'theme');
drupal_add_css(drupal_get_path('theme', 'litejazz') . '/content.css', 'theme');

if (theme_get_setting('litejazz_iepngfix')) {
   drupal_add_js(drupal_get_path('theme', 'litejazz') . '/js/jquery.pngFix.js', 'theme');
}

if (theme_get_setting('litejazz_themelogo'))
{
   function _phptemplate_variables($hook, $variables = array()) {
     $variables['logo'] = base_path() . path_to_theme() . '/images/' . theme_get_setting('litejazz_style') . '/logo.png';

     return $variables;
   }
}
