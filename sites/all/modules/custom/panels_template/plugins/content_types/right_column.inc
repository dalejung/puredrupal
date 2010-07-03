<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Right Column'),
  'description' => t('Answer Ask Link'),
  'category' => 'Panels Template',
);

function panels_template_right_column_content_type_render($subtype, $conf, $panel_args, $context) {

  $block = new stdClass();

  $block->content = panels_template_block_render('right_column');

  $block->title = check_plain('Right Column');

  $block->delta = 'right_column';
  $block->css_class = 'right-column';
  // Added in by me for default list
  $block->skip_separator = TRUE;
  return $block;
}

function panels_template_right_column_content_type_edit_form(&$form, &$form_state) {
    // provide a blank form so we have a place to have context setting.
}

