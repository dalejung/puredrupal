<?php
/*
 * Provides a set of common image, dek, tout functions
 */
function common_view_common_view_registry() {
  $reg['title'] = array(
  ); 
  $reg['link'] = array(
  ); 
  return $reg;
}

/**
 * Returns a common credit link. Either to a node or user
 */
function common_view_common_view_title(&$node) {
  switch ($node->type) {
    default:
      $title = $node->title; 
      break;
  }
  return $title;
}

/**
 * Returns a common node link.
 */
function common_view_common_view_link(&$node) {
  $url = 'node/' . $node->nid;
  return $url;
}
