<?php
/**
 * Provide a context driven right column for panel nodes
 */
function panels_template_block($op = 'list', $delta = 0, $edit = array()) {
  if ($op == 'list') {
    $blocks['right_column'] = array('info' => t('Panels Template: Right Column'));
    $blocks['left_column'] = array('info' => t('Panels Template: Left Column'));
    return $blocks;
  } 
  elseif ($op == 'view') {
    switch ($delta) {
      case 'right_column':
        $block = array(
          'subject' => '<none>',
          'content' => panels_template_block_render('right_column'),
        );
        break;
      case 'left_column':
        $block = array(
          'subject' => '<none>',
          'content' => panels_template_block_render('left_column'),
        );
        break;
    }
    return $block;
  }
}


function panels_template_block_render($type) {
  // Grab default 
  if (function_exists('context_active_values')) { 
    $defaults = context_active_values('panels_template_' . $type);
    $default = $defaults[0];
  }

  // Right nid from node
  $node = menu_get_object();
  $override = $node->panels_template[$type];

  $final = $override ? $override : $default;

  if (!$final) {
    return;
  }

  $display = panels_template_grab_display($final);
  if (!$display) {
    return;
  }
  $content = panels_render_display($display);
  return $content;
}


function panels_template_context_reactions() {
  $template_types = panels_template_get_template_types();
  $types = array('left_column', 'right_column');
  $templates = array();

  foreach ($types as $name) {
    $options = array(
      '' => t('None'),
    );
    $template_type = $template_types[$name];
    $options += panels_template_panels_template_list_type($name); 
    $templates['panels_template_' . $name] = array(
      '#type' => 'select',
      '#title' => 'Panels Templates: ' . $template_type['label'],
      '#options' => $options,
    );
  }

  return $templates;
}

/**
 * If the context has a sitewide context enabled, we assume that it provides
 * default values. If ANY context that is not sidewide has the panel node set
 * it will blank the default ones
 */
function panels_template_context_active_contexts_alter(&$contexts) {
  $defaults = array();
  $overrides = array();
  foreach ($contexts as $context) {
    if ($context->sitewide) {
      $defaults[] = $context;
    }
    else {
      $overrides[] = $context;
    }
  }

  foreach ($overrides as $override) {
    // Right
    if ($override->panels_template_right_column) {
      foreach ($defaults as $default) {
        unset($default->panels_template_right_column);
      }
    }
    // Left
    if ($override->panels_template_left_column) {
      foreach ($defaults as $default) {
        unset($default->panels_template_left_column);
      }
    }
  }
}
