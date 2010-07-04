<?php
// $Id: panels_simple_cache.module,v 1.1.2.7 2008/06/04 12:55:15 pancho Exp $


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

/**
 * Figure out an id for our cache based upon input and settings.
 */
function window_cache_panels_cache_get_id($display, $pane) {
  // By default use the pane type & subtype as the identifier.
	if(is_object($pane) && !$pane->pid) {
  	$base_key = "panel:{$pane->type}:{$pane->subtype}";
	} else {
		$base_key = 'panel_';
  }

  // If available, include the display owner.
  if (isset($display->owner) && isset($display->owner->id)) {
    $base_key .= ':' . $display->owner->id;
  }
  
  // Include the display did if it's available.
    $base_key .= ':' . $display->did;

  // Tack on the pane pid if it's available.
  if (!empty($pane->pid)) {
    $base_key .= ':' . $pane->pid;
  }

  return $base_key;
}

function window_cache_panels_cache_settings_form($conf, $display, $pid) {
}
