<?php
// $Id$

/**
 * Base Grains
 */
function window_cache_window_cache_grains() {
  $grains = array();
  $grains['node'] = array(
    'title' => t('Node'),
    'function' => 'wc_grain_node',
  );
  $grains['user'] = array(
    'title' => t('User'),
    'function' => 'wc_grain_user',
  );
  $grains['logged_in'] = array(
    'title' => t('Logged In (on/off)'),
    'function' => 'wc_grain_logged_in',
  );
  $grains['page'] = array(
    'title' => t("\$_GET['page']"),
    'function' => 'wc_grain_page',
  );
  $grains['get'] = array(
    'title' => t("Query String (\$_GET)"),
    'function' => 'wc_grain_get',
  );
  return $grains;
}

function wc_grain_node() {
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $bit = '|nid:' . arg(1);
  }
  return $bit;
}

function wc_grain_user() {
  global $user;
  $bit = '|user:' . $user->uid;
  return $bit;
}

function wc_grain_logged_in() {
  global $user;
  if ($user->uid > 0) {
    $bit = '|logged_in:1';
  }
  else {
    $bit = '|logged_in:0';
  }
  return $bit;
}

function wc_grain_page() {
  if ($_GET['page']) {
    $bit = '|page:' . $_GET['page'];
  }
  return $bit;
}
function wc_grain_get() {
  $bits = '';
  foreach($_GET as $key => $val) {
    if ($key != 'q') {
      $bits .= $key . '=' . $val . '&';
    }
  }
  if ($bits) {
    $bits = '?' . $bits;
  }
  return $bits;
}

