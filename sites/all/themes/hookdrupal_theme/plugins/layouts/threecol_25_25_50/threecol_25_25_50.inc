<?php
// $Id: threecol_25_25_50.inc,v 1.1 2010/01/11 05:35:13 jwolf Exp $

/**
 * implementation of hook_panels_layouts
 */
function hookdrupal_theme_threecol_25_25_50_panels_layouts() {
  $items['threecol_25_25_50'] = array(
    'title' => t('Panany three column 25/25/50'),
    'icon' => 'threecol_25_25_50.png',
    'theme' => 'threecol_25_25_50',
    'css' => 'threecol_25_25_50.css',
    'theme arguments' => array('id', 'content'),
    'panels' => array(
      'left' => t('Left column'),
      'middle' => t('Middle column'),
      'right' => t('Right column')
    ),
  );

  return $items;
}

