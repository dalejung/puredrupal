<?php
// $Id: threecol_50_25_25.inc,v 1.1 2010/01/11 05:35:13 jwolf Exp $

/**
 * implementation of hook_panels_layouts
 */
function panany_threecol_50_25_25_panels_layouts() {
  $items['threecol_50_25_25'] = array(
    'title' => t('Panany three column 50/25/25'),
    'icon' => 'threecol_50_25_25.png',
    'theme' => 'threecol_50_25_25',
    'css' => 'threecol_50_25_25.css',
    'theme arguments' => array('id', 'content'),
    'panels' => array(
      'left' => t('Left column'),
      'middle' => t('Middle column'),
      'right' => t('Right column')
    ),
  );

  return $items;
}

