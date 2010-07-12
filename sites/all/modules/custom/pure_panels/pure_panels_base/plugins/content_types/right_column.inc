<?php
// $Id$

$plugin = array(
  'single' => TRUE,
  'title' => t('Right Column'),
  'description' => t('Pure Panels Template: Right Column'),
  'category' => 'Pure Panels Template',
  'required context' => new ctools_context_optional(t('Node Viewed'),'node'),
);

function pure_panels_base_right_column_content_type_render($subtype, $conf, $panel_args, $context) {
  if($context) {
    $node = $context->data;
  }

  $block = new stdClass();

  $block->content = pure_panels_region_render('right_column', $node);

  $block->title = check_plain('Right Column');

  $block->delta = 'right_column';
  $block->css_class = 'right-column';
  // Added in by me for default list
  $block->skip_separator = TRUE;
  return $block;
}

function pure_panels_base_right_column_content_type_edit_form(&$form, &$form_state) {
    // provide a blank form so we have a place to have context setting.
}

