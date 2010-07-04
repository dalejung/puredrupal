<?php
// $Id: default.settings.php,v 1.8.2.4 2009/09/14 12:59:18 goba Exp $

$db_url = 'mysql://hookdrupal:hookdrupal@localhost/hookdrupal';
$db_prefix = '';
$update_free_access = FALSE;

ini_set('arg_separator.output',     '&amp;');
ini_set('magic_quotes_runtime',     0);
ini_set('magic_quotes_sybase',      0);
ini_set('session.cache_expire',     200000);
ini_set('session.cache_limiter',    'none');
ini_set('session.cookie_lifetime',  2000000);
ini_set('session.gc_maxlifetime',   200000);
ini_set('session.save_handler',     'user');
ini_set('session.use_cookies',      1);
ini_set('session.use_only_cookies', 1);
ini_set('session.use_trans_sid',    0);
ini_set('url_rewriter.tags',        '');

// Memcache
$conf['cache_inc'] ='sites/all/modules/contrib/memcache/memcache.inc';

$conf['memcache_servers'] = array(
  'localhost:36841' => 'default',
  'localhost:36842' => 'node',
  'localhost:36843' => 'window_cache',
);
$conf['memcache_bins'] = array(
  'cache_page' => 'default',
  'cache_node' => 'node',
  'cache_windows' => 'window_cache',
  'cache_signals' => 'window_cache',
);
