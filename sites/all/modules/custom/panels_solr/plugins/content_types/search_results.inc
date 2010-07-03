<?php

function panels_solr_search_results_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Search Results'),
    'description' => t('Search Results'),
    'category' => 'Panels Solr',
  );
}

function panels_solr_search_results_content_type_render($subtype, $conf, $panel_args, $context) {

	$menu = menu_get_item();
	$search_path = $menu['path'];
	$content = panels_solr_search_view($search_path);

  $block->content = $content;
  $block->title = '';
  $block->delta = 'panels_solr_results';
  return $block;
}
