<?php
// $Id$

function panels_solr_search_info_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Search Info'),
    'description' => t('Search Info'),
    'category' => 'Panels Solr',
    'render last' => TRUE,
    'hook theme' => 'panels_solr_search_info_theme',	
  );
}

function panels_solr_search_info_content_type_render($subtype, $conf, $panel_args, $context) {
  $response = apachesolr_static_response_cache();
  if(empty($response)) {
    return;
  }


  $content = theme('panels_solr_info',$response);	
  $block->content = $content;
  $block->title = '';
  $block->delta = 'panels_solr_info';
  return $block;
}

function panels_solr_search_info_theme(&$theme) {
  $theme['panels_solr_search_info'] = array(
    'template' => 'panels-solr-search-info',
    'arguments' => array('response' => null),
    'path' => drupal_get_path('module','panels_solr').'/plugins/content_types',
  );	
}

function template_preprocess_panels_solr_info(&$vars) {
  $response = $vars['response'];
  if(empty($response)) {
    return;
  }

  $menu = menu_get_item();
  $search_path = $menu['path'];
  $keys = trim(panels_solr_get_keys());

  $count = $response->response->numFound;	

  $vars['result_count'] = $count;
  $vars['search_terms'] = $keys;

  $vars['start'] = $response->response->start + 1;
  
  $vars['end'] = $response->response->start + count($response->response->docs);
  
}
