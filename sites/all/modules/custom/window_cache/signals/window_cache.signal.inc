<?php

require 'node.inc';
require 'taxonomy.inc';
require 'user.inc';
require 'comment.inc';
require 'votingapi.inc';
require 'nodequeue.inc';

/**
 * Simple method to invalidate
 */
function wc_signal_simple($type,$id, $prefix='') {
  $signal = $type .':'. $id;
  if($prefix) {
    $signal = $prefix .'-'.$signal;
  }
  wc_signal('invalidate',$signal);
}

/**
 * 
 */
function wc_signal($op, $signal) {
  switch($op) {
    case 'invalidate':
      wc_signal_invalidate($signal);
      break;
    case 'get':
      wc_signal_get($signal);
      break;
  }
}

/**
 * 
 */
function wc_signal_invalidate($signal) {
  $val = 0;

  if ($cache = cache_get($signal, 'cache_signals')) {
    $val = $cache->data;
  }
  $val++; 
  cache_set($signal, $val,'cache_signals' );

  // notify other modules of invalidation so they can create their own
  module_invoke_all('window_cache_signal', $signal);
}


/**
 * Get the current signal value. You will notice that if this is a new entry
 * we do not store a new signal value. This means that nothing gets stored in
 * cache until the signal is invalidated
 */
function wc_signal_get($signal) {
  $val = 0;
  if ($cache = cache_get($signal, 'cache_signals')) {
    $val = $cache->data;;
  }
  return $val;
}
