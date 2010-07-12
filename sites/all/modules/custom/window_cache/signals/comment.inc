<?php
// $Id$

/**
 * hook_comment 
 */
function window_cache_comment(&$a1, $op) {
	switch($op) {
		case 'insert':
		case 'update':
    case 'delete':
    case 'publish':
    case 'unpublish':
			wc_comment_signal($a1);
			break;
	}
}

/**
 * 
 */
function wc_comment_signal($comment) {
	$comment = (array) $comment;
	$nid = $comment['nid'];
	$node = node_load($nid);
	$uid = $comment['uid'];

	wc_signal_simple('node',$nid);
	wc_signal_simple('comment-nid',$nid);
  wc_signal_simple('user', $uid);
}
