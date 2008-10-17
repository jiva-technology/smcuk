<?php

function phptemplate_settings($saved_settings) {

  $settings = theme_get_settings('litejazz');

  $defaults = array(
    'litejazz_style' => 0,
    'litejazz_width' => 0,
	'litejazz_fixedwidth' => '850',
    'litejazz_breadcrumb' => 0,
	'litejazz_iepngfix' => 0,
    'litejazz_themelogo' => 0,
  );

  $settings = array_merge($defaults, $settings);

  $form['litejazz_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#default_value' => $settings['litejazz_style'],
    '#options' => array(
      'blue' => t('Blue'),
      'red' => t('Red'),
    ),
  );

  $form['litejazz_themelogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Themed Logo'),
    '#default_value' => $settings['litejazz_themelogo'],
  );

  $form['litejazz_width'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Fixed Width'),
    '#default_value' => $settings['litejazz_width'],
  );

  $form['litejazz_fixedwidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Fixed Width Size'),
    '#default_value' => $settings['litejazz_fixedwidth'],
    '#size' => 5,
    '#maxlength' => 5,
  );

  $form['litejazz_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Breadcrumbs'),
    '#default_value' => $settings['litejazz_breadcrumb'],
  );

  $form['litejazz_iepngfix'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use IE PNG Fix'),
    '#default_value' => $settings['litejazz_iepngfix'],
  );

  return $form;
}


