<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Left Column'),
  'description' => t('Panels Template Left Column'),
  'category' => 'Panels Template',
);

function panels_template_base_left_column_content_type_render($subtype, $conf, $panel_args, $context) {

  $block = new stdClass();

  $block->content = panels_template_region_render('left_column');

  $block->title = check_plain('Left Column');

  $block->delta = 'left_column';
  $block->css_class = 'left-column';
  // Added in by me for default list
  $block->skip_separator = TRUE;
  return $block;
}

function panels_template_left_column_content_type_edit_form(&$form, &$form_state) {
    // provide a blank form so we have a place to have context setting.
}

