<?php
/**
 * hook_nodequeue() - interact with nodequeues.
 */
function window_cache_nodequeue($op, $sqid, $nid=null) {
  wc_signal_invalidate_subqueue($sqid);
}

function window_cachel_nodequeue_swap($op, $sqid, $nid=null) {
  wc_signal_invalidate_subqueue($sqid);
}

/**
 * Invalidate the requested subqueue.
 */
function wc_signal_invalidate_subqueue($sqid, $prefix=null) {
  // Invalidate all nodequeues.
	wc_signal_simple('all-nodequeues', '', $prefix);
	// Invalidate the requested subqueue.
	wc_signal_simple('subqueue', $sqid, $prefix);
}

/**
 * Invalidate all subqueues that the current node is in
 */
function wc_signal_node_subqueues_signal($nid, $prefix=null) {
 $result = db_query("SELECT * FROM {nodequeue_nodes} WHERE nid = %d", $nid);
	while ($obj = db_fetch_object($result)) {
		wc_signal_invalidate_subqueue($obj->sqid, $prefix);	
	}
}

