<?php
// $Id$

function window_cache_votingapi_update(&$vobj) {
	wc_votingapi_signal($vobj);
}

function window_cache_votingapi_delete(&$vobj) {
	wc_votingapi_signal($vobj);
}

function window_cache_votingapi_insert(&$vobj) {
	wc_votingapi_signal($vobj);
}

function wc_votingapi_signal(&$vobj) {

	$type = $vobj->content_type;
	$id = $vobj->content_id;
	$uid = $vobj->uid;

  wc_signal_simple('user', $uid);

	switch($type) {
		case 'node':
			$node = node_load($id);
			wc_node_signal($node,'votingapi');
			break;
		case 'comment':
			$comment = db_fetch_object(db_query('SELECT nid from {comments} where cid = "%d"',$id));
			$nid = $comment->nid;
			$node = node_load($nid);
			wc_node_signal($node,'votingapi-comment');
			wc_signal_simple('node', $nid);
			wc_signal_simple('comment-nid', $nid);
			break;
	}
}

