<?php
// $Id$

/**
 * Specially named hook. for .inc file. This looks a little silly due to the
 * redundancy, but that's really just because the content type shares a
 * name with the module.
 */
function pure_panels_template_pure_panels_template_ctools_content_types() {
  return array(
    'title' => t('Panel templates'),
    'content type' => 'pure_panels_template_pure_panels_template_content_type_content_type',
  );
}

/**
 * Return each available panel template available as a subtype.
 */
function pure_panels_template_pure_panels_template_content_type_content_type($subtype_id, $plugin) {
  $template = pure_panels_template_load($subtype_id);
  return _pure_panels_template_pure_panels_template_content_type_content_type($template);
}

/**
 * Return each available panel template available as a subtype.
 */
function pure_panels_template_pure_panels_template_content_type_content_types($plugin) {
  $types = array();
  foreach (pure_panels_template_load_all() as $template) {
    $type = _pure_panels_template_pure_panels_template_content_type_content_type($template);
    if ($type) {
      $types[$template->name] = $type;
    }
  }
  return $types;
}

/**
 * Return an info array describing a single panel template.
 */
function _pure_panels_template_pure_panels_template_content_type_content_type($template) {
  if (!empty($template->disabled)) {
    return;
  }

  $title = filter_xss_admin($template->title);
  $type = array(
    'title' => $title,
    // For now panel templates will just use the contrib block icon.
    'icon' => 'icon_panel_template.png',
    'description' => $title,
    'category' => !empty($template->category) ? $template->category : t('Mini panel'),
  );
  if (!empty($template->requiredcontexts)) {
    $type['required context'] = array();
    foreach ($template->requiredcontexts as $context) {
      $info = ctools_get_context($context['name']);
      // TODO: allow an optional setting
      $type['required context'][] = new ctools_context_required($context['identifier'], $info['context name']);
    }
  }
  return $type;
}

/**
 * Render a panel template called from a panels display.
 */
function pure_panels_template_pure_panels_template_content_type_render($subtype, $conf, $panel_args, &$contexts) {
  static $viewing = array();
  $template = pure_panels_template_load($subtype);
  if (!$template) {
    return FALSE;
  }
  if (!empty($viewing[$template->name])) {
    return FALSE;
  }

  // Load up any contexts we might be using.
  $context = ctools_context_match_required_contexts($template->requiredcontexts, $contexts);
  $template->context = $template->display->context = ctools_context_load_contexts($template, FALSE, $context);

  if (empty($template) || !empty($template->disabled)) {
    return;
  }
  $viewing[$template->name] = TRUE;

  $template->display->args = $panel_args;
  $template->display->css_id = pure_panels_template_get_id($subtype);
  $template->display->owner = $template;
  // unique ID of this template.
  $template->display->owner->id = $template->name;

  $block = new stdClass();
  $block->module  = 'pure_panels_template';
  $block->delta   = $subtype;
  $block->title = filter_xss_admin($template->title);
  $block->content = panels_render_display($template->display);

  unset($viewing[$template->name]);
  return $block;
}

/**
 * Edit form for the panel template content type.
 */
function pure_panels_template_pure_panels_template_content_type_edit_form(&$form, &$form_state) {
  // Empty form to ensure we have the override title + context gadgets.
}

/**
 * Provide the administrative title of a panel template.
 */
function pure_panels_template_pure_panels_template_content_type_admin_title($subtype, $conf) {
  $template = pure_panels_template_load($subtype);
  if (!$template) {
    return t('Deleted/missing panel template @name', array('@name' => $subtype));
  }

  $title = filter_xss_admin($template->title);
  if (empty($title)) {
    $title = t('Untitled panel template');
  }
  return $title;
}

