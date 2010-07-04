<?php

function panels_solr_search_form_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Search Form'),
    'description' => t('Search Form'),
    'category' => 'Panels Solr',
		'render last' => TRUE,
  );
}

function panels_solr_search_form_content_type_render($subtype, $conf, $panel_args, $context) {

	$type = 'panels_solr';
  $search_path = panels_solr_search_path();

	$keys = trim(panels_solr_get_keys());
	$content = drupal_get_form('search_form', $search_path, $keys, $type);

  $block->content = $content;
  $block->title = '';
  $block->delta = 'panels_solr_form';
  return $block;
}
