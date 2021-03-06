<?php
// $Id$

require('defaults.inc');

/*
 * Generate a key for each pane
 */
function window_cache_pane_key(&$content) {
  $pane_key = $content->type .'__'. $content->subtype;
  // Special case for nodes
  if ($pane_key == 'node__node') {
    $pane_key .= '__'. $content->configuration['nid'];
  }
  return $pane_key;
}

/*
 * We take over the cache settings in the pre_render
 */
function window_cache_panels_pre_render(&$display) {
  $settings = array();

  // Gather Settings
  foreach ($display->content as &$content) {
    $pane_key = window_cache_pane_key($content);

    $hook = 'window_cache_pane_settings';

    $conf = window_cache_panels_default_settings($display, $content, $pane_key); 

    foreach (module_implements($hook) as $module) {
      $func = $module . '_' . $hook;
      $func($conf, $display, $content, $pane_key);
      if ($conf['final']) {
        break;
      }
    }

    $settings[$pane_key] = $conf;
  }

  wc_panels_apply_all_settings($display, $settings); 
}

function wc_panels_apply_all_settings(&$display, $settings) {
  // Gather Settings
  foreach ($display->content as &$pane) {
    $pane_key = window_cache_pane_key($pane);

    $cache_conf = $settings[$pane_key];
    wc_panels_apply_single_settings($pane, $cache_conf);
  }
}

function wc_panels_apply_single_settings(&$pane, $settings) {
  $pane->cache['method'] = 'window_panel_cache';
  $pane->cache['settings'] = $settings;
}
