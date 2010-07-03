<?php
/**
 * Provide a context driven right column for panel nodes
 */
function panels_template_region_handler($type) {
  // Right nid from node
  $node = menu_get_object();
  return panels_template_region_render($type, $node);
}

function panels_template_region_render($type, $node=null) {
  // Grab default from context 
  if (function_exists('context_get_plugin')) { 
    $plugin = context_get_plugin('reaction','panels_template');
    $default = $plugin->get_template($type);
  }

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

function panels_template_context_plugins() {
  $plugins = panels_template_context_reactions();
  return $plugins;
}

function panels_template_context_registry() {
  $registry = array();
  $registry['reactions'] =  array(
    'panels_template' => array(
      'title' => 'Panels Template',
      'description' => 'Control Panel Template Regions',
      'plugin' => 'panels_template_reaction'   
     ),
  );

  return $registry;
}

function panels_template_context_reactions() {
  $plugins = array();

  $plugins['panels_template_reaction'] = array(
    'handler' => array(
    'path' => drupal_get_path('module', 'panels_template'). '/plugins',
    'file' => 'context.reactions.inc',
    'class' => 'panels_template_reaction',
    'parent' => 'context_reaction',
    ),
  );
  return $plugins;
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
