<?php
// $Id$

function window_cache_user($op, &$edit, &$account, $category = NULL) {
	if($op == 'update') {
		wc_signal_simple('user', $account->uid);
	}
}

