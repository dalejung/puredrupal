<?php

function common_view_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'common_view') .'/includes',
    ),
    'handlers' => array(
      'common_view_handler_field_image' => array(
        'parent' => 'views_handler_field',
      ),
      'common_view_handler_field_node' => array(
        'parent' => 'views_handler_field_node',
      ),
      'common_view_handler_field' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

function common_view_views_data() {
  $data['common_view']['table']['group'] = t('Common View');

  $data['common_view']['table']['join']['node'] = array(
    '#global' => array(),
  );

  $data['common_view']['common_image'] = array(
    'title' => t('Image'),
    'help' => t('Common View: Image'),
    'field' => array(
      'notafield' => TRUE,
      'click sortable' => FALSE,
      'handler' => 'common_view_handler_field_image',
      'handler function' => 'common_view_image_node',
    ), 
  );

  $data['common_view']['common_teaser_image'] = array(
    'title' => t('Teaser Image'),
    'help' => t('Common View: Teaser Image'),
    'field' => array(
      'notafield' => TRUE,
      'click sortable' => FALSE,
      'handler' => 'common_view_handler_field_image',
      'handler function' => 'common_view_teaser_image_node',
    ), 
  );

  $data['common_view']['common_link'] = array(
    'title' => t('Link'),
    'help' => t('Common View: Link'),
    'field' => array(
      'notafield' => TRUE,
      'click sortable' => FALSE,
      'handler' => 'common_view_handler_field_node',
      'handler function' => 'common_view_link',
    ), 
  );

  $data['common_view']['common_title'] = array(
    'title' => t('Title'),
    'help' => t('Common View: Title'),
    'field' => array(
      'notafield' => TRUE,
      'click sortable' => FALSE,
      'handler' => 'common_view_handler_field_node',
      'handler function' => 'common_view_title',
    ), 
  );

  return $data;
}
