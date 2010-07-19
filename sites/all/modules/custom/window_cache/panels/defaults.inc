<?php
// $Id$

function window_cache_panels_default_settings(&$display, &$content, $pane_key) {
  $conf = array(); 
  $conf['signal'] = array();
  $conf['granularity'] = array();

  foreach((array)$content->configuration['context'] as $context_name) {
    $context = $display->context[$context_name];

    // Context based caching
    if($context->type == 'node') {
    }
  }

  return $conf;
}
