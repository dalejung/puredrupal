<?php
// $Id: panany_gradient.inc,v 1.1 2010/01/11 05:35:13 jwolf Exp $

/**
 * @file styles/panany_gradient/panany_gradient.inc
 * Definition of the style.
 * 
 * Based  on rounded_corner.inc from panels.module
 * 
 */

function panany_gradient_get_path($file, $base_path = FALSE, $theme = 'panany') {
  $output = $base_path ? base_path() : '';
  return $output . drupal_get_path('theme', $theme) . '/' . $file;
}

// ---------------------------------------------------------------------------
// Panels hooks.

/**
 * Implementation of THEME_STYLENAME_panels_style().
 * 
 * Where THEME is the theme name, and STYLENAME matches this inc filename.
 */
function panany_panany_gradient_panels_styles() {
  return array(
    'panany_gradient' => array(
      'title' => t('Panany gradient background'),
      'description' => t('Presents the panes or panels with a box around them'),
      'render panel' => 'panany_panany_gradient_style_render_panel',
      'render pane' => 'panany_panany_gradient_style_render_pane',
      'settings form' => 'panany_panany_gradient_style_settings_form',
      'hook theme' => array(
        'panany_panany_gradient_box' => array(
          'arguments' => array('content' => NULL),
          'path' => panany_gradient_get_path('plugins/styles/panany_gradient'),
          'template' => 'panany-gradient-box',
        ),
      ),
    ),
  );
}

// ---------------------------------------------------------------------------
// Panels style plugin callbacks.

/**
 * Render callback.
 * 
 * Declared by name in THEME_STYLENAME_panels_style() above.
 *
 * @ingroup themeable
 */
function theme_panany_panany_gradient_style_render_panel($display, $panel_id, $panes, $settings) {
  $output = '';

  // Determine where to put the box. If empty or 'pane' around each pane. If
  // 'panel' then just around the whole panel.
  $where = empty($settings['panany_gradient_location']) ? 'pane' : $settings['panany_gradient_location'];

  $print_separator = FALSE;
  foreach ($panes as $pane_id => $pane) {
    $text = panels_render_pane($pane, $display->content[$pane_id], $display);
    if ($text) {
      // Add the separator if we've already displayed a pane.
      if ($print_separator) {
        $output .= '<div class="panel-separator"></div>';
      }

      if ($where == 'pane') {
        $output .= theme('panany_panany_gradient_box', $text);
      }
      else {
        $output .= $text;
        $print_separator = TRUE;
      }
    }
  }

  if ($where == 'panel') {
    $output = theme('panany_panany_gradient_box', $output);
  }

  panany_add_panany_gradient_css();

  return $output;
}

/**
 * Render callback for a single pane.
 */
function theme_panany_panany_gradient_style_render_pane($content, $pane, $display) {
  $output = theme('panels_pane', $content, $pane, $display);

  if (!$output) {
    return;
  }

  // Just stick a box around the standard theme_panels_pane.
  $output = theme('panany_panany_gradient_box', $output);
  panany_add_panany_gradient_css();
  return $output;
}

function panany_add_panany_gradient_css() {
  static $css_loaded;
  if (!$css_loaded == TRUE) {
    $filename = drupal_get_path('theme', 'panany') . '/plugins/styles/panany_gradient/panany_gradient.css';
    drupal_add_css($filename, 'module', 'all', FALSE);
    $css_loaded = TRUE;
  }
}

/**
 * Settings form callback. 
 */
function panany_panany_gradient_style_settings_form($style_settings) {
  $form['panany_gradient_location'] = array(
    '#type' => 'select',
    '#title' => t('Box around'),
    '#options' => array(
      'pane' => t('Each pane'),
      'panel' => t('Each panel'),
    ),
    '#default_value' => (isset($style_settings['panany_gradient_location'])) ? $style_settings['panany_gradient_location'] : 'pane',
    '#description' => t('Choose whether to include the box around each pane (piece of content) or panel (each column or region)'),
  );

  return $form;
}
