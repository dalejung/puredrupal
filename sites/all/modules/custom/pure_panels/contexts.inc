<?php
/**
 * Provide a context driven right column for panel nodes
 */
function pure_panels_region_handler($type) {
  // Right nid from node
  $node = menu_get_object();
  return pure_panels_region_render($type, $node);
}

function pure_panels_region_render($type, $node=null) {
  // Grab default from context 
  if (function_exists('context_get_plugin')) { 
    $plugin = context_get_plugin('reaction','pure_panels');
    $default = $plugin->get_template($type);
  }

  $override = $node->pure_panels[$type];

  $final = $override ? $override : $default;

  if (!$final) {
    return;
  }


  $display = pure_panels_grab_display($final);
  if (!$display) {
    return;
  }
  $content = panels_render_display($display);
  return $content;
}

function pure_panels_context_plugins() {
  $plugins = pure_panels_context_reactions();
  return $plugins;
}

function pure_panels_context_registry() {
  $registry = array();
  $registry['reactions'] =  array(
    'pure_panels' => array(
      'title' => 'Panels Template',
      'description' => 'Control Panel Template Regions',
      'plugin' => 'pure_panels_reaction'   
     ),
  );

  return $registry;
}

function pure_panels_context_reactions() {
  $plugins = array();

  $plugins['pure_panels_reaction'] = array(
    'handler' => array(
    'path' => drupal_get_path('module', 'pure_panels'). '/plugins',
    'file' => 'context.reactions.inc',
    'class' => 'pure_panels_reaction',
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
function pure_panels_context_active_contexts_alter(&$contexts) {
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
    if ($override->pure_panels_right_column) {
      foreach ($defaults as $default) {
        unset($default->pure_panels_right_column);
      }
    }
    // Left
    if ($override->pure_panels_left_column) {
      foreach ($defaults as $default) {
        unset($default->pure_panels_left_column);
      }
    }
  }
}
