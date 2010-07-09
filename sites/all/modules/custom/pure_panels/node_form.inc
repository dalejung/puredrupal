<?php

function pure_panels_post_form_alter(&$form, &$form_state) {
  if ($form['#id'] == 'node-form') {
    pure_panels_template_dropdowns($form, $form_state);
  }
}

function pure_panels_template_dropdowns(&$form, &$form_state) {
  $node = $form['#node'];

  $types = pure_panels_get_applicable_types($node);
  if (!$types) {
    return;
  }

  $template_types = pure_panels_get_template_types();

  $templates = array(
    '#type' => 'fieldset',
    '#title' => 'Panel Templates',
    '#collapsible' => TRUE,
  );
  foreach ($types as $name) {
    $options = array(
      '' => t('None'),
    );
    $template_type = $template_types[$name];
    $options += pure_panels_pure_panels_list_type($name);
    $templates['pure_panels_' . $name] = array(
      '#type' => 'select',
      '#title' => $template_type['label'],
      '#options' => $options,
      '#default_value' => $node->pure_panels[$name] ? $node->pure_panels[$name] : '',
    );
  }
  drupal_alter('pure_panels_template_dropdowns', $templates);
  $form['panel_templates'] = $templates;
}

/**
 * Defined what template_types are available for each content type
 * For now, default to all with an alter
 */
function pure_panels_get_applicable_types(&$node) {
  static $enabled = array();

  if ($enabled) {
    return $enabled[$node->type];
  }

  $cache = cache_get('pure_panels_applicable_types')->data;
  if ($cache && $cache->data) {
    $enabled = $cache;
    return $enabled[$node->type];
  } 

  // hook_pure_panels_applicable_types().
  // Define a nested set of panel templates grouped by the content types they
  // can be used with.
  //
  // function mymodule_pure_panels_applicable_types() {
  //   $types = array(
  //     'article' => array(
  //       'article_default',
  //       'article_extra_ads',
  //     ),
  //     'channel' => array(
  //       'channel_default',
  //       'channel_premium',
  //     ),
  //     'gallery' => array(
  //       'gallery_default',
  //       'gallery_grid',
  //     ),
  //   );
  //   return $types;
  // }
  foreach (module_implements('pure_panels_applicable_types') as $module) {
    $func = $module . '_pure_panels_applicable_types';
    $ret = $func();
    if (is_array($ret)) {
      $enabled = array_merge_recursive($enabled, $ret);
    }
  }

  // hook_pure_panels_applicable_types_alter($types).
  // Override the default application types.
  drupal_alter('pure_panels_applicable_types', $enabled);

  cache_set('pure_panels_applicable_types', $enabled);

  return $enabled[$node->type];
}

function pure_panels_nodeapi(&$node, $op, $a3, $a4) {
  switch ($op) {
    case 'update':
    case 'delete':
    case 'insert':
      pure_panels_node_write($node);
      break;
    case 'load':
      return pure_panels_node_load($node); 
      break;
  }
}

/**
 * Write the panels template
 */
function pure_panels_node_write(&$node) {
  $types = pure_panels_get_applicable_types($node);
  if (!$types) {
    return;
  }
  foreach ($types as $name) {
    $obj = new stdClass;
    $obj->nid = $node->nid;
    $obj->vid = $node->vid;
    $obj->template_type = $name;
    $obj->name = $node->{'pure_panels_' . $name};

    drupal_write_record('pure_panels_node', $obj, array('vid', 'template_type'));
    // Only insert new one for selected names
    if (!db_affected_rows() && $obj->name) {
      drupal_write_record('pure_panels_node', $obj);
    }
  }
}

/**
 * Attach the pure_panels additions
 */
function pure_panels_node_load(&$node) {
  $ret = array();
  $types = pure_panels_get_applicable_types($node);
  if (!$types) {
    return;
  }
  $templates = pure_panels_node_load_templates($node);
  $ret['pure_panels'] = $templates;
  return $ret;
}

/**
 * Grab from database
 */
function pure_panels_node_load_templates(&$node) {
  $sql = "SELECT template_type, name FROM pure_panels_node WHERE vid = %d";
  $results = db_query($sql, $node->vid);
  $templates = array();
  while ($row = db_fetch_object($results)) {
    $templates[$row->template_type] = $row->name;  
  }
  return $templates;
}
