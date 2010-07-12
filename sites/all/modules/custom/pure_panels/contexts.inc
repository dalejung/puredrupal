<?php
// $Id$

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

/**
 * hook_context_registry
 */
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

/**
 * hook_context_reactions
 */
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
