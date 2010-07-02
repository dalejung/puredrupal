<?php
// $Id: panels_template.admin.inc,v 1.6.2.9 2009/09/08 16:23:48 merlinofchaos Exp $

/**
 * @file
 *
 * Adtemplatestrative items for the panels template module.
 */

/**
 * Implementation of hook_menu().
 */
function _panels_template_menu() {
  // Provide some common options to reduce code repetition.
  // By using array addition and making sure these are the rightmost
  // value, they won't override anything already set.
  $base = array(
    'access arguments' => array('create template panels'),
    'file' => 'panels_template.admin.inc',
  );

  $items['admin/build/panel-template'] = array(
    'title' => 'Panel Templates',
    'page callback' => 'panels_template_list_page',
    'description' => 'Create and adtemplatester template panels (panels exposed as blocks).',
  ) + $base;
  $items['admin/build/panel-template/list'] = array(
    'title' => 'List',
    'page callback' => 'panels_template_list_page',
    'weight' => -10,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/add'] = array(
    'title' => 'Add',
    'page callback' => 'panels_template_add_page',
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/import'] = array(
    'title' => 'Import',
    'page callback' => 'panels_template_import_template',
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/settings'] = array(
    'title' => 'Settings',
    'page callback' => 'panels_template_settings',
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  // Also provide settings on the main panel UI
  $items['admin/build/panels/settings/panels-template'] = array(
    'title' => 'Panel Templates',
    'page callback' => 'panels_template_settings',
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/disable'] = array(
    'page callback' => 'panels_template_disable_page',
    'weight' => -1,
    'type' => MENU_CALLBACK,
  ) + $base;
  $items['admin/build/panel-template/enable'] = array(
    'page callback' => 'panels_template_enable_page',
    'weight' => -1,
    'type' => MENU_CALLBACK,
  ) + $base;

  $base['access arguments'] = array('adtemplatester template panels');

  $items['admin/build/panel-template/%panels_template_admin'] = array(
    'title' => 'Preview',
    'page callback' => 'panels_template_preview_panel',
    'page arguments' => array(3),
    'weight' => -10,
    'type' => MENU_CALLBACK,
  ) + $base;
  $items['admin/build/panel-template/%panels_template_admin/preview'] = array(
    'title' => 'Preview',
    'page callback' => 'panels_template_preview_panel',
    'page arguments' => array(3),
    'weight' => -10,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/%panels_template_admin/edit-layout'] = array(
    'title' => 'Layout',
    'page callback' => 'panels_template_edit_layout',
    'page arguments' => array(3),
    'weight' => -9,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/%panels_template_admin/edit-general'] = array(
    'title' => 'Settings',
    'page callback' => 'panels_template_edit',
    'page arguments' => array(3),
    'weight' => -5,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/%panels_template_admin/edit-context'] = array(
    'title' => 'Context',
    'page callback' => 'panels_template_edit_context',
    'page arguments' => array(3),
    'weight' => -2,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/%panels_template_admin/edit-content'] = array(
    'title' => 'Content',
    'page callback' => 'panels_template_edit_content',
    'page arguments' => array(3),
    'weight' => -1,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/%panels_template_admin/export'] = array(
    'title' => 'Export',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('panels_template_edit_export', 3),
    'weight' => 0,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/panel-template/%panels_template_admin/delete'] = array(
    'title' => 'Delete template panel',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('panels_template_delete_confirm', 3),
    'type' => MENU_CALLBACK,
  ) + $base;

  return $items;
}

/**
 * Settings for template panels.
 */
function panels_template_settings() {
  panels_load_include('common');
  return drupal_get_form('panels_common_settings', 'panels_template');
}

/**
 * Provide a list of template panels, with links to edit or delete them.
 */
function panels_template_list_page() {
  panels_load_include('plugins');
  $layouts = panels_get_layouts();
  $items = array();
  $sorts = array();

  $header = array(
    array('data' => t('Title'), 'field' => 'title'),
    array('data' => t('Name'), 'field' => 'name', 'sort' => 'asc'),
    array('data' => t('Type'), 'field' => 'type'),
    array('data' => t('Template Type'), 'field' => 'template_type'),
    t('Layout'),
    t('Operations'),
  );
  $ts = tablesort_init($header);

  // Load all template panels and their displays.
  $panel_templates = panels_template_load_all(TRUE);

  foreach ($panel_templates as $panel_template) {
    $ops = array();
    if (empty($panel_template->disabled)) {
      $ops[] = l(t('Edit'), "admin/build/panel-template/{$panel_template->name}/edit-general");
      $ops[] = l(t('Export'), "admin/build/panel-template/{$panel_template->name}/export");
    }
    if ($panel_template->type != t('Default')) {
      $text = ($panel_template->type == t('Overridden')) ? t('Revert') : t('Delete');
      $ops[] = l($text, "admin/build/panel-template/{$panel_template->name}/delete");
    }
    else {
      if (empty($panel_template->disabled)) {
        $ops[] = l(t('Disable'), "admin/build/panel-template/disable/{$panel_template->name}", array('query' => drupal_get_destination()));
      }
      else {
        $ops[] = l(t('Enable'), "admin/build/panel-template/enable/{$panel_template->name}", array('query' => drupal_get_destination()));
      }
    }

    $item = array();
    $item[] = check_plain($panel_template->title);
    $item[] = check_plain($panel_template->name);
    // this is safe as it's always programmatic
    $item[] = $panel_template->type;

    $item[] = $panel_template->template_type;

    if (empty($panel_template->display)) {
      $panel_template->display = $displays[$panel_template->did];
    }

    $item[] = check_plain($layouts[$panel_template->display->layout]['title']);
    $item[] = implode(' | ', $ops);
    $items[] = $item;
    switch ($ts['sql']) {
      case 'title':
        $sorts[] = $item[0];
        break;

      case 'type':
        $sorts[] = $panel_template->type . $item[0];
        break;

      case 'name':
      default:
        $sorts[] = $item[1];
        break;
    }
  }

  if (drupal_strtolower($ts['sort']) == 'desc') {
    arsort($sorts);
  }
  else {
    asort($sorts);
  }

  $i = array();
  foreach ($sorts as $id => $title) {
    $i[] = $items[$id];
  }

  return theme('table', $header, $i);
}

/**
 * Provide a form to confirm deletion of a template panel.
 */
function panels_template_delete_confirm(&$form_state, $panel_template) {
  if (!is_object($panel_template)) {
    $panel_template = panels_template_load($panel_template);
  }
  $form['name'] = array('#type' => 'value', '#value' => $panel_template->name);
  $form['did'] = array('#type' => 'value', '#value' => $panel_template->did);
  return confirm_form($form,
    t('Are you sure you want to delete the template panel "@title"?', array('@title' => $panel_template->title)),
    !empty($_GET['destination']) ? $_GET['destination'] : 'admin/build/panel-template',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Handle the submit button to delete a template panel.
 */
function panels_template_delete_confirm_submit($form, &$form_state) {
  $template = panels_template_load($form_state['values']['name']);
  panels_template_delete($template);
  $form_state['redirect'] = 'admin/build/panel-template';
}

/**
 * Provide an adtemplatestrative preview of a template panel.
 */
function panels_template_preview_panel($template) {
  $template->display->args = array();
  $template->display->css_id = panels_template_get_id($template->name);

  drupal_add_js(drupal_get_path('module', 'panels_template') . '/panels_template.js');
  $theme = variable_get('theme_default', 'zen');
  drupal_add_css(drupal_get_path('theme',$theme).'/' . $theme .'.css');
  drupal_add_css($path = drupal_get_path('module', 'panels_template') . '/panels_template.css');

  ctools_include('context');
  $template->context = $template->display->context = ctools_context_load_contexts($template);

  drupal_set_title(filter_xss_admin($template->title));
  return panels_render_display($template->display);
}

/**
 * Page callback to export a template panel to PHP code.
 */
function panels_template_edit_export(&$form_state, $panel_template) {
  if (!is_object($panel_template)) {
    $panel_template = panels_template_load($panel_template);
  }
  drupal_set_title(check_plain($panel_template->title));
  $code = panels_template_export($panel_template);

  $lines = substr_count($code, "\n");
  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => $panel_template->title,
    '#default_value' => $code,
    '#rows' => $lines,
  );
  return $form;
}

/**
 * Page callback to import a template panel from PHP code.
 */
function panels_template_import_template() {
  if (isset($_POST['form_id']) && $_POST['form_id'] == 'panels_template_edit_form') {
    $panel_template = unserialize($_SESSION['pm_import']);
    drupal_set_title(t('Import panel template "@s"', array('@s' => $panel_template->title)));
    return drupal_get_form('panels_template_edit_form', $panel_template);
  }

  return drupal_get_form('panels_template_import_form');
}

/**
 * Form for the template panel import.
 */
function panels_template_import_form() {
  $form['panel_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Panel template code'),
    '#cols' => 60,
    '#rows' => 15,
    '#description' => t('Cut and paste the results of an exported template panel here.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  $form['#redirect'] = NULL;
  return $form;
}

/**
 * Handle the submit button on importing a template panel.
 */
function panels_template_import_form_submit($form, &$form_state) {
  ob_start();
  eval($form_state['values']['panel_template']);
  ob_end_clean();

  if (isset($template)) {
    drupal_set_title(t('Import template panel "@s"', array('@s' => $template->title)));
    $template->pid = 'new';
    // As $template contains non-stdClass objects,
    // it needs to be serialized before being stored in the session variable.
    $_SESSION['pm_import'] = serialize($template);
    $output = drupal_get_form('panels_template_edit_form', $template);
    print theme('page', $output);
    exit;
  }
  else {
    drupal_set_message(t('Unable to get a template panel out of that.'));
  }
}

/**
 * Handle the add template panel page.
 */
function panels_template_add_page($layout = NULL) {
  panels_load_include('plugins');
  panels_load_include('common');
  $layouts = panels_common_get_allowed_layouts('panels_template');
  $output = '';

  if ($layout === NULL) {
    foreach ($layouts as $id => $layout) {
      $output .= panels_print_layout_link($id, $layout, $_GET['q'] . '/' . $id);
    }
    return $output;
  }

  if (!$layouts[$layout]) {
    return drupal_not_found();
  }

  $panel_template = new stdClass();
  $panel_template->display = panels_new_display();
  $panel_template->display->layout = $layout;
  $panel_template->pid = 'new';
  $panel_template->did = 'new';
  $panel_template->title = '';
  $panel_template->name = '';
  $panel_template->template_type = '';
  $panel_template->category = '';

  drupal_set_title(t('Add template panel'));
  return panels_template_edit($panel_template);
}

/**
 * Edit a template panel.
 *
 * Called from both the add and edit points to provide for common flow.
 */
function panels_template_edit($panel_template) {
  if (!is_object($panel_template)) {
    $panel_template = panels_template_load($panel_template);
  }
  if ($panel_template) {
    drupal_set_title(check_plain($panel_template->title));
  }

  return drupal_get_form('panels_template_edit_form', $panel_template);
}

/**
 * Form to edit the settings of a template panel.
 */
function panels_template_edit_form(&$form_state, $panel_template) {
  panels_load_include('common');
  panels_load_include('plugins');
  drupal_add_css(panels_get_path('css/panels_admin.css'));

  $form['pid'] = array(
    '#type' => 'value',
    '#value' => isset($panel_template->pid) ? $panel_template->pid : '',
  );

  $form['panel_template'] = array(
    '#type' => 'value',
    '#value' => $panel_template,
  );

  $form['right'] = array(
    '#prefix' => '<div class="layout-container">',
    '#suffix' => '</div>',
  );
  $form['left'] = array(
    '#prefix' => '<div class="info-container">',
    '#suffix' => '</div>',
  );

  $form['left']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $form['left']['settings']['title'] = array(
    '#type' => 'textfield',
    '#size' => 24,
    '#default_value' => $panel_template->title,
    '#title' => t('Panel Template title'),
    '#description' => t('The title for this template panel. It can be overridden in the block configuration.'),
  );

  $form['left']['settings']['name'] = array(
    '#type' => 'textfield',
    '#size' => 24,
    '#default_value' => $panel_template->name,
    '#title' => t('Panel Template name'),
    '#description' => t('A unique name used to identify this panel page internally. It must be only be alpha characters and underscores. No spaces, numbers or uppercase characters.'),
  );

  $template_types = panels_template_get_template_types();
  $options = array();
  foreach ($template_types as $name => $type) {
    $options[$name] = $type['label'];
  }

  $form['left']['settings']['template_type'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $panel_template->template_type,
    '#title' => t('Template Type'),
    '#description' => '',
  );

  $form['left']['settings']['category'] = array(
    '#type' => 'textfield',
    '#size' => 24,
    '#default_value' => $panel_template->category,
    '#title' => t('Template Category'),
    '#description' => '',
  );

  ctools_include('context');
  $panel_template->context = $panel_template->display->context = ctools_context_load_contexts($panel_template);

  $form['right']['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout'),
  );

  $layout = panels_get_layout($panel_template->display->layout);

  $form['right']['layout']['layout-icon'] = array(
    '#value' => panels_print_layout_icon($panel_template->display->layout, $layout),
  );

  $form['right']['layout']['layout-display'] = array(
    '#value' => check_plain($layout['title']),
  );
  $form['right']['layout']['layout-content'] = array(
    '#value' => theme('panels_common_content_list', $panel_template->display),
  );

  $contexts = theme('panels_common_context_list', $panel_template);
  if ($contexts) {
    $form['right']['context'] = array(
      '#type' => 'fieldset',
      '#title' => t('Contexts'),
    );

    $form['right']['context']['context'] = array(
      '#value' => $contexts,
    );
  }

  $label = (!empty($panel_template->pid) && $panel_template->pid == 'new') ? t('Save and proceed') : t('Save');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $label,
  );

  return $form;
}

/**
 * Validate submission of the template panel edit form.
 */
function panels_template_edit_form_validate($form, &$form_state) {
  // Test uniqueness of name:
  if (!$form_state['values']['name']) {
    form_error($form['left']['settings']['name'], t('Panel template name is required.'));
  }
  elseif (preg_match("/[^A-Za-z0-9_]/", $form_state['values']['name'])) {
    form_error($form['left']['settings']['name'], t('Name must be alphanumeric or underscores only.'));
  }
  elseif (preg_match("/[^A-Za-z0-9 ]/", $form_state['values']['category'])) {
    form_error($form['left']['settings']['category'], t('Categories may contain only alphanumerics or spaces.'));
  }
  else {
    $query = "SELECT pid FROM {panels_template} WHERE name = '%s'";
    if (!empty($form_state['values']['pid']) && is_numeric($form_state['values']['pid'])) {
      $query .= ' AND pid != ' . $form_state['values']['pid'];
    }
    if (db_result(db_query($query, $form_state['values']['name']))) {
      form_error($form['left']['settings']['name'], t('Panel name must be unique.'));
    }
  }
}

/**
 * Process submission of the template panel edit form.
 */
function panels_template_edit_form_submit($form, &$form_state) {
  $panel_template = $form_state['values']['panel_template'];
  $panel_template->title = $form_state['values']['title'];
  $panel_template->name = $form_state['values']['name'];
  $panel_template->template_type = $form_state['values']['template_type'];
  $panel_template->category = empty($form_state['values']['category']) ? '' : $form_state['values']['category'];

  if (isset($panel_template->pid) && $panel_template->pid == 'new') {
    unset($_SESSION['pm_import']);
    drupal_set_message(t('Your new template panel %title has been saved.', array('%title' => $panel_template->title)));
    panels_template_save($panel_template);
    $form_state['values']['pid'] = $panel_template->pid;

    $layout = panels_get_layout($panel_template->display->layout);

    $form_state['redirect'] = "admin/build/panel-template/{$panel_template->name}/edit-context/next";
  }
  else {
    drupal_set_message(t('Your changes have been saved.'));
    panels_template_save($panel_template);
  }
}

/**
 * Form to edit context features of a template panel.
 */
function panels_template_edit_context($panel_template, $next = NULL) {
  ctools_include('context-admin');
  ctools_context_admin_includes();

  if (!empty($_POST)) {
    $panel_template = ctools_object_cache_get('context_object:panel_template', $panel_template->name);
  }
  else {
    ctools_object_cache_set('context_object:panel_template', $panel_template->name, $panel_template);
  }

  drupal_set_title(check_plain($panel_template->title));
  return drupal_get_form('panels_template_context_form', $panel_template, $next);
}

/**
 * Form to edit the context settings of a template panel.
 */
function panels_template_context_form(&$form_state, $panel_template, $next = NULL) {
  drupal_add_css(panels_get_path('css/panels_admin.css'));
  panels_load_include('plugins');
  $layout = panels_get_layout($panel_template->display->layout);

  $form['panel_template'] = array(
    '#type' => 'value',
    '#value' => $panel_template,
  );

  $form_state['panel_template'] = $panel_template;

  $form['right'] = array(
    '#prefix' => '<div class="right-container">',
    '#suffix' => '</div>',
  );

  $form['left'] = array(
    '#prefix' => '<div class="left-container">',
    '#suffix' => '</div>',
  );

  ctools_context_add_context_form('panel_template', $form, $form_state, $form['left']['contexts_table'], $panel_template);
  ctools_context_add_relationship_form('panel_template', $form, $form_state, $form['right']['relationships_table'], $panel_template);

  $label = $next ? t('Save and proceed') : t('Save');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $label,
  );

  return $form;
}

/**
 * Process submission of the template panel edit form.
 */
function panels_template_context_form_submit($form, &$form_state) {
  $panel_template = $form_state['panel_template'];

  drupal_set_message(t('Your changes have been saved.'));
  panels_template_save($panel_template);
  ctools_object_cache_clear('context_object:panel_template', $panel_template->name);
  if ($form_state['values']['submit'] == t('Save and proceed')) {
    $form_state['redirect'] = "admin/build/panel-template/{$panel_template->name}/edit-content";
  }
}

/**
 * Enable a default template panel.
 */
function panels_template_enable_page($name = NULL) {
  ctools_include('export');
  ctools_export_set_status('panels_template', $name, FALSE);
  drupal_goto();
}

/**
 * Disable a default template panel.
 */
function panels_template_disable_page($name = NULL) {
  ctools_include('export');
  ctools_export_set_status('panels_template', $name, TRUE);
  drupal_goto();
}

/**
 * Pass through to the panels content editor.
 */
function panels_template_edit_content($panel_template) {
  if (!is_object($panel_template)) {
    $panel_template = panels_template_load($panel_template);
  }
  ctools_include('context');
  // Collect a list of contexts required by the arguments on this page.
  $panel_template->display->context = $contexts = ctools_context_load_contexts($panel_template);

  panels_load_include('common');
  $content_types = panels_common_get_allowed_types('panels_template', $contexts);

  $output = panels_edit($panel_template->display, NULL, $content_types);
  if (is_object($output)) {
    $panel_template->display = $output;
    $panel_template->did = $output->did;
    panels_template_save($panel_template);
    drupal_goto("admin/build/panel-template/{$panel_template->name}/edit-content");
  }
  // Print this with theme('page') so that blocks are disabled while editing a display.
  // This is important because negative margins in common block layouts (i.e, Garland)
  // messes up the drag & drop.
  drupal_set_title(check_plain($panel_template->title));
  print theme('page', $output, FALSE);
}

/**
 * Pass through to the panels layout editor.
 */
function panels_template_edit_layout($panel_template) {
  if (!is_object($panel_template)) {
    $panel_template = panels_template_load($panel_template);
  }

  $output = panels_edit_layout($panel_template->display, t('Save'), NULL, 'panels_template');
  if (is_object($output)) {
    $panel_template->display = $output;
    $panel_template->did = $output->did;
    panels_template_save($panel_template);
    drupal_goto("admin/build/panel-template/{$panel_template->name}/edit-layout");
  }

  drupal_set_title(check_plain($panel_template->title));
  return $output;
}
