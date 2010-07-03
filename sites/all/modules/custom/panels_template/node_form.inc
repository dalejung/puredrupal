<?php

function panels_template_post_form_alter(&$form, &$form_state) {
  if ($form['#id'] == 'node-form') {
    panels_template_template_dropdowns($form, $form_state);
  }
}

function panels_template_template_dropdowns(&$form, &$form_state) {
  $node = $form['#node'];

  $types = panels_template_get_applicable_types($node);
  if (!$types) {
    return;
  }

  $template_types = panels_template_get_template_types();

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
    $options += panels_template_panels_template_list_type($name);
    $templates['panels_template_' . $name] = array(
      '#type' => 'select',
      '#title' => $template_type['label'],
      '#options' => $options,
      '#default_value' => $node->panels_template[$name] ? $node->panels_template[$name] : '',
    );
  }
  drupal_alter('panels_template_template_dropdowns', $templates);
  $form['panel_templates'] = $templates;
}

/**
 * Defined what template_types are available for each content type
 * For now, default to all with an alter
 */
function panels_template_get_applicable_types(&$node) {
  static $enabled = array();

  if ($enabled) {
    return $enabled[$node->type];
  }

  $cache = cache_get('panels_template_applicable_types')->data;
  if ($cache && $cache->data) {
    $enabled = $cache;
    return $enabled[$node->type];
  } 

  // hook_panels_template_applicable_types().
  // Define a nested set of panel templates grouped by the content types they
  // can be used with.
  //
  // function mymodule_panels_template_applicable_types() {
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
  foreach (module_implements('panels_template_applicable_types') as $module) {
    $func = $module . '_panels_template_applicable_types';
    $ret = $func();
    if (is_array($ret)) {
      $enabled = array_merge_recursive($enabled, $ret);
    }
  }

  // hook_panels_template_applicable_types_alter($types).
  // Override the default application types.
  drupal_alter('panels_template_applicable_types', $enabled);

  cache_set('panels_template_applicable_types', $enabled);

  return $enabled[$node->type];
}

function panels_template_nodeapi(&$node, $op, $a3, $a4) {
  switch ($op) {
    case 'update':
    case 'delete':
    case 'insert':
      panels_template_node_write($node);
      break;
    case 'load':
      return panels_template_node_load($node); 
      break;
  }
}

/**
 * Write the panels template
 */
function panels_template_node_write(&$node) {
  $types = panels_template_get_applicable_types($node);
  if (!$types) {
    return;
  }
  foreach ($types as $name) {
    $obj = new stdClass;
    $obj->nid = $node->nid;
    $obj->vid = $node->vid;
    $obj->template_type = $name;
    $obj->name = $node->{'panels_template_' . $name};

    drupal_write_record('panels_template_node', $obj, array('vid', 'template_type'));
    // Only insert new one for selected names
    if (!db_affected_rows() && $obj->name) {
      drupal_write_record('panels_template_node', $obj);
    }
  }
}

/**
 * Attach the panels_template additions
 */
function panels_template_node_load(&$node) {
  $ret = array();
  $types = panels_template_get_applicable_types($node);
  if (!$types) {
    return;
  }
  $templates = panels_template_node_load_templates($node);
  $ret['panels_template'] = $templates;
  return $ret;
}

/**
 * Grab from database
 */
function panels_template_node_load_templates(&$node) {
  $sql = "SELECT template_type, name FROM panels_template_node WHERE vid = %d";
  $results = db_query($sql, $node->vid);
  $templates = array();
  while ($row = db_fetch_object($results)) {
    $templates[$row->template_type] = $row->name;  
  }
  return $templates;
}
