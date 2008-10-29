<?php
// $Id: rounded_corners.inc,v 1.1.2.4 2008/01/08 18:03:08 merlinofchaos Exp $
/**
 * @file styles/rounded_corners.inc
 * Definition of the 'rounded_corners' panel style.
 */


// ---------------------------------------------------------------------------
// Panels hooks.

/**
 * Implementation of hook_panels_style_info().
 */
function panels_rounded_corners_panels_styles() {
  return array(
    'rounded_corners' => array(
      'title' => t('Rounded corners'),
      'description' => t('Presents the panes or panels with a rounded corner box around them'),
      'render panel' => 'panels_rounded_corners_style_render_panel',
      'settings form' => 'panels_rounded_corners_style_settings_form',
    ),
  );
}


// ---------------------------------------------------------------------------
// Panels style plugin callbacks.

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_panels_rounded_corners_style_render_panel($display, $panel_id, $panes, $settings) {
  $output = '';

  // Determine where to put the box. If empty or 'pane' around each pane. If
  // 'panel' then just around the whole panel.
  $where = empty($settings['corner_location']) ? 'pane' : $settings['corner_location'];

  $print_separator = FALSE;
  foreach ($panes as $pane_id => $pane) {
    // Add the separator if we've already displayed a pane.
    if ($print_separator) {
      $output .= '<div class="panel-separator"></div>';
    }
    $text = theme('panels_pane', $pane, $display->content[$pane_id], $display);
    if ($text) {
      $output .= ($where == 'pane') ? theme('rounded_corners_box', $text) : $text;
    }

    // If we displayed a pane, this will become true; if not, it will become
    // false.
    $print_separator = (bool) $text;
  }

  if ($where == 'panel') {
    $output = theme('rounded_corners_box', $output);
  }

  static $displays_used = array();
  if (empty($displays_used[$display->name])) {
    theme('rounded_corners_css', $display, $where);
    $displays_used[$display->name] = TRUE;
  }

  return $output;
}

/**
 * Settings form callback.
 */
function panels_rounded_corners_style_settings_form($style_settings) {
  $form['corner_location'] = array(
    '#type' => 'select',
    '#title' => t('Box around'),
    '#options' => array(
      'pane' => t('Each pane'),
      'panel' => t('Each panel')
    ),
    '#default_value' => (isset($style_settings['corner_location'])) ? $style_settings['corner_location'] : 'ul',
    '#description' => t('Choose whether to include the box around each pane (piece of content) or panel (each column or region)'),
  );

  return $form;
}

function theme_rounded_corners_box($content) {
  return $content;
}

/**
 * @} End of "ingroup themeable".
 */
