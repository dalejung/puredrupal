<?php
// $Id$


/**
 * Page callback for the very short admin page.
 */
$plugin = array(
  'title' => t("Window Cache"),
  'description' => t('Window Cache'),
  'cache get' => 'window_cache_panels_cache_get_cache',
  'cache set' => 'window_cache_panels_cache_set_cache',
  'cache clear' => 'window_cache_panels_cache_clear',
  'settings form' => 'window_cache_panels_cache_settings_form',
  'settings form submit' => 'window_cache_panels_cache_settings_form_submit',
);

/**
 * Get cached content.
 */
function window_cache_panels_cache_get_cache($conf, $display, $args, $contexts, $pane = NULL) {
  $pane_key = window_cache_pane_key($pane);
  $settings = $pane->cache['settings'];
  $window_key = wc_window_key($pane_key, $settings);
  if (!$cache) {
    return FALSE;
  }

  $cache->data->content->content = '<div class="signal-cached">' . $cache_info . $cache->data->content->content . '</div>';

  return $cache->data;
}

/**
 * Set cached content.
 */
function window_cache_panels_cache_set_cache($conf, $content, $display, $args, $contexts, $pane = NULL) {
}

/**
 * Clear cached content.
 * Cache clears are always for an entire display, regardless of arguments.
 */
function window_cache_panels_cache_clear_cache($display) {
  $cid = 'window_cache_cache';

  // This is used in case this is an in-code display, which means did will be something like 'new-1'.
  if (isset($display->owner) && isset($display->owner->id)) {
    $cid .= ':' . $display->owner->id;
  }
  $cid .= ':' . $display->did;

  cache_clear_all($cid, 'cache', TRUE);
}

function window_cache_panels_cache_settings_form($conf, $display, $pid) {
}
