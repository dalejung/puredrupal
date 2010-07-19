<?php
// $Id$

function wc_log($window_key=NULL, $message=NULL) {
  static $log;

  if($window_key) {
    $log[] = array($window_key, $message); 
  } else {
    return $log;
  }
}

function _wc_cache_get($window_key, $table='cache_windows') {
  $cache = cache_get($window_key, $table);
  if($cache) {
    wc_log($window_key, $cache);
    return $cache;
  }
}

/**
 *
 */
function _wc_cache_set($window_key, $window, $table='cache_windows') {
  wc_log($window_key, $window);
  $window->timestamp = time();
  cache_set($window_key, $window, CACHE_PERMANENT);
}

/**
 *
 */
function wc_cache($mode, $base_key, $settings, $window=NULL) {
  $window_key = wc_window_key($base_key, $settings);

  switch($mode) {
    case 'get':
      return wc_cache_get($window_key, $settings);
      break;
    case 'set':
      wc_cache_set($window_key, $window);
      break;
  }
}

function wc_cache_get($window_key, $settings) {
  return _wc_cache_get($window_key, $settings);
}

function wc_cache_set($window_key, $window) {
  _wc_cache_set($window_key, $window);
}

/*
 * This will let module provide programmitic grains 
 **/
function window_cache_get_all_grains() {
  static $grains = array();

  if ($grains) {
    return $grains;
  }

  $grains = module_invoke_all('window_cache_grains');
  drupal_alter('window_cache_grains', $grains);
  return $grains;
}

/**
 * Generate the window key
 */
function wc_window_key($base_key, $settings) {
  $window_key = $base_key;

  $granularity = $settings['granularity'];
  if($granularity) {
    $window_key .= wc_granularity_key($granularity);
  }
  
  return $window_key;
}

/**
 *
 */
function wc_granularity_key($granularity) {
  $grains = window_cache_get_all_grains();

  $g_key = '';
  foreach ($granularity as $key) {
    $bit = '';

    // if system wide grain use the function 
    if ($grain = $grains[$key]) {

      $func = $grain['function'];
      if (function_exists($func)) {
        $bit = $func();
      } 
    }
    else { 
      // Use the key. This is for other module to provide their own granularity
      // Such as panels which uses the contexts and arguments of the panels
      $bit = $key;
    }
    $g_key .= $bit;
  }
  return $g_key;
}
