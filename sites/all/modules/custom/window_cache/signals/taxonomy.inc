<?php
// $Id$

function wc_signal_node_taxonomy_signal(&$node, $prefix=null) {
  if (!isset($node->taxonomy)) {
    return;
  }
	foreach ($node->taxonomy as $vocab) {
    if (!is_array($vocab)) { continue; }
    foreach ($vocab as $tid) {
      if (is_numeric($tid)) {
        wc_signal_simple('tid', $tid, $prefix);
      }
    }
	}
}
