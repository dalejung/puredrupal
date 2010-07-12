<?php
// $Id: term.inc,v 1.5 2009/05/11 23:49:33 merlinofchaos Exp $

/**
 * @file
 *
 * Plugin to provide a term context
 */

/**
 * Implementation of specially named hook_ctools_contexts().
 */
$plugin = array(
  'title' => t("Node being viewed"),
  'description' => t('Node being viewed'),
  'context' => 'pure_panels_template_context_create_term',
  'settings form' => 'pure_panels_template_context_panels_template_node_settings_form',
  'settings form validate' => 'pure_panels_template_context_panels_template_node_settings_form_validate',
  'settings form submit' => 'pure_panels_template_context_panels_template_node_settings_form_submit',
  'keyword' => 'node',
  'context name' => 'pure-panels-template-node',
  'js' => array('misc/autocomplete.js'),
);

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function pure_panels_template_context_create_term($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('node');
  $context->plugin = 'node';

  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $data = node_load(arg(1));
  }
  else {
    return $context;
  }

  if (!empty($data)) {
    $context->data     = $data;
    $context->title    = $data->title;
    $context->argument = $data->nid;
    return $context;
  }
}

function pure_panels_template_context_panels_template_node_settings_form($conf) {
  return $form;
}

/**
 * Validate a term.
 */
function pure_panels_template_context_panels_template_node_settings_form_validate($form, &$form_values, &$form_state) {
}

function pure_panels_template_context_panels_template_node_settings_form_submit($form, &$form_values, &$form_state) {
}
