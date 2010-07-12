<?php
// $Id: pure_panels_template.admin.inc,v 1.6.2.12 2010/05/20 09:05:37 sdboyer Exp $

/**
 * @file
 *
 * Administrative items for the panels template module.
 */
/**
 * Implementation of hook_menu().
 */
function _pure_panels_template_menu() {
  // Provide some common options to reduce code repetition.
  // By using array addition and making sure these are the rightmost
  // value, they won't override anything already set.
  $base = array(
    'access arguments' => array('create panels template'),
    'file' => 'pure_panels_template.admin.inc',
  );

  $items['admin/build/pure-panels-template'] = array(
    'title' => 'Pure Panels Template',
    'page callback' => 'pure_panels_template_list_page',
    'description' => 'Create and administer template panels (panels exposed as blocks).',
  ) + $base;
  $items['admin/build/pure-panels-template/list'] = array(
    'title' => 'List',
    'page callback' => 'pure_panels_template_list_page',
    'weight' => -10,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/add'] = array(
    'title' => 'Add',
    'page callback' => 'pure_panels_template_add_page',
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/import'] = array(
    'title' => 'Import',
    'page callback' => 'pure_panels_template_import_template',
    'access callback' => 'ctools_access_multiperm',
    'access arguments' => array('create panels template', 'use PHP for block visibility'),
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/settings'] = array(
    'title' => 'Settings',
    'page callback' => 'pure_panels_template_settings',
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  // Also provide settings on the main panel UI
  $items['admin/build/panels/settings/pure-panels-template'] = array(
    'title' => 'Pure Panels Template',
    'page callback' => 'pure_panels_template_settings',
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/disable'] = array(
    'page callback' => 'pure_panels_template_disable_page',
    'weight' => -1,
    'type' => MENU_CALLBACK,
  ) + $base;
  $items['admin/build/pure-panels-template/enable'] = array(
    'page callback' => 'pure_panels_template_enable_page',
    'weight' => -1,
    'type' => MENU_CALLBACK,
  ) + $base;

  $base['access arguments'] = array('administer panels template');

  $items['admin/build/pure-panels-template/%pure_panels_template_admin'] = array(
    'title' => 'Preview',
    'page callback' => 'pure_panels_template_preview_panel',
    'page arguments' => array(3),
    'weight' => -10,
    'type' => MENU_CALLBACK,
  ) + $base;
  $items['admin/build/pure-panels-template/%pure_panels_template_admin/preview'] = array(
    'title' => 'Preview',
    'page callback' => 'pure_panels_template_preview_panel',
    'page arguments' => array(3),
    'weight' => -10,
    'type' => MENU_DEFAULT_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/%pure_panels_template_admin/edit-layout'] = array(
    'title' => 'Layout',
    'page callback' => 'pure_panels_template_edit_layout',
    'page arguments' => array(3),
    'weight' => -9,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/%pure_panels_template_admin/edit-general'] = array(
    'title' => 'Settings',
    'page callback' => 'pure_panels_template_edit',
    'page arguments' => array(3),
    'weight' => -5,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/%pure_panels_template_admin/edit-context'] = array(
    'title' => 'Context',
    'page callback' => 'pure_panels_template_edit_context',
    'page arguments' => array(3),
    'weight' => -2,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/%pure_panels_template_admin/edit-content'] = array(
    'title' => 'Content',
    'page callback' => 'pure_panels_template_edit_content',
    'page arguments' => array(3),
    'weight' => -1,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/%pure_panels_template_admin/export'] = array(
    'title' => 'Export',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('pure_panels_template_edit_export', 3),
    'weight' => 0,
    'type' => MENU_LOCAL_TASK,
  ) + $base;
  $items['admin/build/pure-panels-template/%pure_panels_template_admin/delete'] = array(
    'title' => 'Delete template panel',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('pure_panels_template_delete_confirm', 3),
    'type' => MENU_CALLBACK,
  ) + $base;

  return $items;
}

/**
 * Settings for template panels.
 */
function pure_panels_template_settings() {
  ctools_include('common', 'panels');
  return drupal_get_form('panels_common_settings', 'pure_panels_template');
}

/**
 * Provide a list of template panels, with links to edit or delete them.
 */
function pure_panels_template_list_page() {
  ctools_include('plugins', 'panels');
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

  // Load all panels templates and their displays.
  $templates = pure_panels_template_load_all();

  foreach ($templates as $template) {
    $ops = array();
    if (empty($template->disabled)) {
      $ops[] = l(t('Edit'), "admin/build/pure-panels-template/$template->name/edit-general");
      $ops[] = l(t('Export'), "admin/build/pure-panels-template/$template->name/export");
    }
    if ($template->type != t('Default')) {
      $text = ($template->type == t('Overridden')) ? t('Revert') : t('Delete');
      $ops[] = l($text, "admin/build/pure-panels-template/$template->name/delete");
    }
    else {
      if (empty($template->disabled)) {
        $ops[] = l(t('Disable'), "admin/build/pure-panels-template/disable/$template->name", array('query' => drupal_get_destination()));
      }
      else {
        $ops[] = l(t('Enable'), "admin/build/pure-panels-template/enable/$template->name", array('query' => drupal_get_destination()));
      }
    }

    $item = array();
    $item[] = check_plain($template->title);
    $item[] = check_plain($template->name);
    // this is safe as it's always programmatic
    $item[] = $template->type;

    $item[] = $template->template_type;

    if (empty($template->display)) {
      $template->display = $displays[$template->did];
    }

    $item[] = check_plain($layouts[$template->display->layout]['title']);
    $item[] = implode(' | ', $ops);
    $items[] = $item;
    switch ($ts['sql']) {
      case 'title':
        $sorts[] = $item[0];
        break;

      case 'name':
      default:
        $sorts[] = $item[1];
        break;

      case 'type':
        $sorts[] = $template->type . $item[0];
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
 * Provide a form to confirm deletion of a panel template.
 */
function pure_panels_template_delete_confirm(&$form_state, $template) {
  if (!is_object($template)) {
    $template = pure_panels_template_load($template);
  }
  $form['name'] = array('#type' => 'value', '#value' => $template->name);
  $form['did'] = array('#type' => 'value', '#value' => $template->did);
  return confirm_form($form,
    t('Are you sure you want to delete the panel template "@title"?', array('@title' => $template->title)),
    !empty($_GET['destination']) ? $_GET['destination'] : 'admin/build/pure-panels-template',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Handle the submit button to delete a panel template.
 */
function pure_panels_template_delete_confirm_submit($form, &$form_state) {
  $template = pure_panels_template_load($form_state['values']['name']);
  pure_panels_template_delete($template);
  $form_state['redirect'] = 'admin/build/pure-panels-template';
}

/**
 * Provide an administrative preview of a panel template.
 */
function pure_panels_template_preview_panel($template) {
  $template->display->args = array();
  $template->display->css_id = pure_panels_template_get_id($template->name);

  ctools_include('context');
  $template->context = $template->display->context = ctools_context_load_contexts($template);

  drupal_set_title(filter_xss_admin($template->title));
  return panels_render_display($template->display);
}

/**
 * Page callback to export a panel template to PHP code.
 */
function pure_panels_template_edit_export(&$form_state, $template) {
  if (!is_object($template)) {
    $template = pure_panels_template_load($template);
  }
  drupal_set_title(check_plain($template->title));
  $code = pure_panels_template_export($template);

  $lines = substr_count($code, "\n");
  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => $template->title,
    '#default_value' => $code,
    '#rows' => $lines,
  );
  return $form;
}

/**
 * Page callback to import a panel template from PHP code.
 */
function pure_panels_template_import_template() {
  if (isset($_POST['form_id']) && $_POST['form_id'] == 'pure_panels_template_edit_form') {
    $template = unserialize($_SESSION['pm_import']);
    drupal_set_title(t('Import panel template "@s"', array('@s' => $template->title)));
    return drupal_get_form('pure_panels_template_edit_form', $template);
  }

  return drupal_get_form('pure_panels_template_import_form');
}

/**
 * Form for the panel template import.
 */
function pure_panels_template_import_form() {
  $form['panel_template'] = array(
    '#type' => 'textarea',
    '#title' => t('Panel template code'),
    '#cols' => 60,
    '#rows' => 15,
    '#description' => t('Cut and paste the results of an exported panel template here.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  $form['#redirect'] = NULL;
  return $form;
}

/**
 * Handle the submit button on importing a panel template.
 */
function pure_panels_template_import_form_submit($form, &$form_state) {
  ob_start();
  eval($form_state['values']['panel_template']);
  ob_end_clean();

  if (isset($template)) {
    drupal_set_title(t('Import panel template "@s"', array('@s' => $template->title)));
    $template->pid = 'new';
    // As $template contains non-stdClass objects,
    // it needs to be serialized before being stored in the session variable.
    $_SESSION['pm_import'] = serialize($template);
    $output = drupal_get_form('pure_panels_template_edit_form', $template);
    print theme('page', $output);
    exit;
  }
  else {
    drupal_set_message(t('Unable to get a panel template out of that.'));
  }
}

/**
 * Handle the add panel template page.
 */
function pure_panels_template_add_page($layout = NULL) {
  ctools_include('plugins', 'panels');
  ctools_include('common', 'panels');
  $layouts = panels_common_get_allowed_layouts('pure_panels_template');
  $output = '';

  if ($layout === NULL) {
    foreach ($layouts as $id => $layout) {
      $output .= panels_print_layout_link($id, $layout, $_GET['q'] .'/'. $id);
    }
    return $output;
  }

  if (!$layouts[$layout]) {
    return drupal_not_found();
  }

  $template = new stdClass();
  $template->display = panels_new_display();
  $template->display->layout = $layout;
  $template->pid = 'new';
  $template->did = 'new';
  $template->title = '';
  $template->name = '';
  $template->template_type = '';
  $template->category = '';

  drupal_set_title(t('Add panel template'));
  return pure_panels_template_edit($template);
}

/**
 * Edit a panel template.
 *
 * Called from both the add and edit points to provide for common flow.
 */
function pure_panels_template_edit($template) {
  if (!is_object($template)) {
    $template = pure_panels_template_load($template);
  }
  if ($template) {
    drupal_set_title(check_plain($template->title));
  }

  return drupal_get_form('pure_panels_template_edit_form', $template);
}

/**
 * Form to edit the settings of a panel template.
 */
function pure_panels_template_edit_form(&$form_state, $template) {
  ctools_include('common', 'panels');
  ctools_include('plugins', 'panels');
  drupal_add_css(panels_get_path('css/panels_admin.css'));

  $form['pid'] = array(
    '#type' => 'value',
    '#value' => isset($template->pid) ? $template->pid : '',
  );

  $form['template'] = array(
    '#type' => 'value',
    '#value' => $template,
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
    '#default_value' => $template->title,
    '#title' => t('Panel Template title'),
    '#description' => t('The title for this panel template. It can be overridden in the block configuration.'),
  );

  $form['left']['settings']['name'] = array(
    '#type' => 'textfield',
    '#size' => 24,
    '#default_value' => $template->name,
    '#title' => t('Panel Template name'),
    '#maxlength' => 32,
    '#description' => t('A unique name used to identify this panel page internally. It must be only be alpha characters and underscores. No spaces, numbers or uppercase characters.'),
  );

  $template_types = pure_panels_get_template_types();
  $options = array();
  foreach ($template_types as $name => $type) {
    $options[$name] = $type['label'];
  }

  $form['left']['settings']['template_type'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $template->template_type,
    '#title' => t('Template Type'),
    '#description' => '',
  );

  /*
   * Taken out for now. 
   */

  /*
  $form['left']['settings']['category'] = array(
    '#type' => 'textfield',
    '#size' => 24,
    '#default_value' => $template->category,
    '#title' => t('Panel template category'),
    '#description' => t("The category that this panel template will be grouped into on the Add Content form. Only upper and lower-case alphanumeric characters are allowed. If left blank, defaults to 'Mini panels'."),
  );
   */

  ctools_include('context');
  $template->context = $template->display->context = ctools_context_load_contexts($template);

  $form['right']['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout'),
  );

  $layout = panels_get_layout($template->display->layout);

  $form['right']['layout']['layout-icon'] = array(
    '#value' => panels_print_layout_icon($template->display->layout, $layout),
  );

  $form['right']['layout']['layout-display'] = array(
    '#value' => check_plain($layout['title']),
  );
  $form['right']['layout']['layout-content'] = array(
    '#value' => theme('panels_common_content_list', $template->display),
  );

  $contexts = theme('panels_common_context_list', $template);
  if ($contexts) {
    $form['right']['context'] = array(
      '#type' => 'fieldset',
      '#title' => t('Contexts'),
    );

    $form['right']['context']['context'] = array(
      '#value' => $contexts,
    );
  }

  $label = (!empty($template->pid) && $template->pid == 'new') ? t('Save and proceed') : t('Save');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $label,
  );

  return $form;
}

/**
 * Validate submission of the panel template edit form.
 */
function pure_panels_template_edit_form_validate($form, &$form_state) {
  // Test uniqueness of name:
  if (!$form_state['values']['name']) {
    form_error($form['left']['settings']['name'], t('Panel template name is required.'));
  }
  else if (preg_match("/[^A-Za-z0-9_]/", $form_state['values']['name'])) {
    form_error($form['left']['settings']['name'], t('Name must be alphanumeric or underscores only.'));
  }
  else if (preg_match("/[^A-Za-z0-9 ]/", $form_state['values']['category'])) {
    form_error($form['left']['settings']['category'], t('Categories may contain only alphanumerics or spaces.'));
  }
  else {
    $query = "SELECT pid FROM {pure_panels_template} WHERE name = '%s'";
    if (!empty($form_state['values']['pid']) && is_numeric($form_state['values']['pid'])) {
      $query .= ' AND pid != ' . $form_state['values']['pid'];
    }
    if (db_result(db_query($query, $form_state['values']['name']))) {
      form_error($form['left']['settings']['name'], t('Panel name must be unique.'));
    }
  }
}

/**
 * Process submission of the panel template edit form.
 */
function pure_panels_template_edit_form_submit($form, &$form_state) {
  $template = $form_state['values']['template'];
  if (isset($template->pid) && $template->pid != 'new' && $template->name != $form_state['values']['name']) {
    // update all existing panel templates to point to this one appropriately.
    db_query("UPDATE {blocks} b SET delta = '%s' WHERE b.module = 'pure_panels_template' AND b.delta = '%s'", $form_state['values']['name'], $template->name);

    // Above was blocks; these are actual panel panes.
    $result = db_query("SELECT * FROM {panels_pane} WHERE type = 'pure_panels_template' and subtype = '%s'", $template->name);
    while ($pane = db_fetch_object($result)) {
      $conf = unserialize($pane->configuration);
      $conf['name'] = $form_state['values']['name'];
      db_query("UPDATE {panels_pane} SET configuration = '%s', subtype = '%s' WHERE pid = %d", serialize($conf), $conf['name'], $pane->pid);
    }
  }

  $template->title = $form_state['values']['title'];
  $template->name = $form_state['values']['name'];
  $template->template_type = $form_state['values']['template_type'];
  $template->category = empty($form_state['values']['category']) ? '' : $form_state['values']['category'];

  if (isset ($template->pid) && $template->pid == 'new') {
    unset($_SESSION['pm_import']);
    drupal_set_message(t('Your new panel template %title has been saved.', array('%title' => $template->title)));
    pure_panels_template_save($template);
    $form_state['values']['pid'] = $template->pid;

    $layout = panels_get_layout($template->display->layout);

    $form_state['redirect'] = "admin/build/pure-panels-template/$template->name/edit-context/next";
  }
  else {
    drupal_set_message(t('Your changes have been saved.'));
    pure_panels_template_save($template);
  }
}

/**
 * Form to edit context features of a panel template.
 */
function pure_panels_template_edit_context($template, $next = NULL) {
  ctools_include('context-admin');
  ctools_context_admin_includes();

  if (!empty($_POST)) {
    $template = ctools_object_cache_get('context_object:panel_template', $template->name);
  }
  else {
    ctools_object_cache_set('context_object:panel_template', $template->name, $template);
  }

  drupal_set_title(check_plain($template->title));
  return drupal_get_form('pure_panels_template_context_form', $template, $next);
}

/**
 * Form to edit the context settings of a panel template.
 */
function pure_panels_template_context_form(&$form_state, $template, $next = NULL) {
  drupal_add_css(panels_get_path('css/panels_admin.css'));
  ctools_include('plugins', 'panels');
  $layout = panels_get_layout($template->display->layout);

  $form['panel_template'] = array(
    '#type' => 'value',
    '#value' => $template,
  );

  $form_state['panel_template'] = $template;

  $form['right'] = array(
    '#prefix' => '<div class="right-container">',
    '#suffix' => '</div>',
  );

  $form['left'] = array(
    '#prefix' => '<div class="left-container">',
    '#suffix' => '</div>',
  );

  ctools_context_add_context_form('panel_template', $form, $form_state, $form['right']['contexts_table'], $template);
  ctools_context_add_required_context_form('panel_template', $form, $form_state, $form['left']['required_contexts_table'], $template);
  ctools_context_add_relationship_form('panel_template', $form, $form_state, $form['right']['relationships_table'], $template);
  //ctools_context_add_argument_form('panel_template', $form, $form_state, $form['left']['argument_table'], $template);

  $label = $next ? t('Save and proceed') : t('Save');
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $label,
  );

  return $form;
}

/**
 * Process submission of the panel template edit form.
 */
function pure_panels_template_context_form_submit($form, &$form_state) {
  $template = $form_state['panel_template'];

  drupal_set_message(t('Your changes have been saved.'));
  pure_panels_template_save($template);
  ctools_object_cache_clear('context_object:panel_template', $template->name);
  if ($form_state['values']['submit'] == t('Save and proceed')) {
    $form_state['redirect'] = "admin/build/pure-panels-template/$template->name/edit-content";
  }
}

/**
 * Enable a default panel template.
 */
function pure_panels_template_enable_page($name = NULL) {
  ctools_include('export');
  ctools_export_set_status('pure_panels_template', $name, FALSE);
  drupal_goto();
}

/**
 * Disable a default panel template.
 */
function pure_panels_template_disable_page($name = NULL) {
  ctools_include('export');
  ctools_export_set_status('pure_panels_template', $name, TRUE);
  drupal_goto();
}

/**
 * Pass through to the panels content editor.
 */
function pure_panels_template_edit_content($template) {
  if (!is_object($template)) {
    $template = pure_panels_template_load($template);
  }
  ctools_include('context');
  // Collect a list of contexts required by the arguments on this page.
  $template->display->context = $contexts = ctools_context_load_contexts($template);

  ctools_include('common', 'panels');
  $content_types = panels_common_get_allowed_types('pure_panels_template', $contexts);

  $output = panels_edit($template->display, NULL, $content_types);
  if (is_object($output)) {
    $template->display = $output;
    $template->did = $output->did;
    pure_panels_template_save($template);
    drupal_goto("admin/build/pure-panels-template/$template->name/edit-content");
  }
  // Print this with theme('page') so that blocks are disabled while editing a display.
  // This is important because negative margins in common block layouts (i.e, Garland)
  // messes up the drag & drop.
  drupal_set_title(check_plain($template->title));
  print theme('page', $output, FALSE);
}

/**
 * Pass through to the panels layout editor.
 */
function pure_panels_template_edit_layout($template) {
  if (!is_object($template)) {
    $template = pure_panels_template_load($template);
  }

  $output = panels_edit_layout($template->display, t('Save'), NULL, 'pure_panels_template');
  if (is_object($output)) {
    $template->display = $output;
    $template->did = $output->did;
    pure_panels_template_save($template);
    drupal_goto("admin/build/pure-panels-template/$template->name/edit-layout");
  }

  drupal_set_title(check_plain($template->title));
  return $output;
}
