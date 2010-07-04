<?php

/**
 *
 */
function window_cache_nodeapi(&$node, $op, $a3, $a4 = NULL) {
	switch($op) {
		case 'delete':
		case 'update':
		case 'insert':
			wc_node_signal($node);
      wc_signal_invalidate_noderef($node); 
			break;
	}
}

function wc_node_signal(&$node, $prefix=NULL) {
  // signal cache
	wc_signal_simple('all-nodes', '', $prefix);
	wc_signal_simple('node', $node->nid, $prefix);
	wc_signal_simple('node-type', $node->type, $prefix);
	// Invlidate the taxonomies attached to node
	wc_signal_node_taxonomy_signal($node, $prefix);
  // Invalidates all subqueues 
	wc_signal_node_subqueues_signal($node->nid, $prefix);
}

/*
  Taken from Nodereferrers
  Invalidate all nodes referencing this node 
  Also invlidate all nodes that this one points to
*/
function wc_signal_invalidate_noderef(&$node) {
  // Depends on the nodereferrer module
  if(!module_exists('nodereferrer')) {
    return;
  }
  // Clear content cache to help maintain proper display of nodes.
  $nids = array();
  $type = content_types($node->type);
  foreach ($type['fields'] as $field) {
    // Add referenced nodes to nids. This will clean up nodereferrer fields
    // when the referencing node is updated.
    if ($field['type'] == 'nodereference') {
      $node_field = isset($node->$field['field_name']) ? $node->$field['field_name'] : array();
      foreach ($node_field as $delta => $item) {
        $nids[$item['nid']] = $item['nid'];
      }
    }
  }

  // Clear Content cache for nodes that reference the node that is being updated.
  // This will keep nodereference fields up to date when referred nodes are
  // updated. @note this currenlty doesn't work all that well since nodereference
  // doesn't respect publishing states or access control.
  if (isset($node->nid)) {
    $referrers = nodereferrer_referrers($node->nid);
    $referrer_nids = array_keys($referrers);
    $nids = array_merge($nids, $referrer_nids);
  }

  foreach ($nids as $nid) {
    wc_signal_simple('node', $nid);
  }
}
