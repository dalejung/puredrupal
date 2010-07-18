<?php
class common_view_handler_field extends views_handler_field_node {
  function query() {
    $this->add_additional_fields();
  }

  function init(&$view, $options) {
    parent::init($view, $options);
    // Make sure we grab enough information to build a pseudo-node with enough
    // credentials at render-time.
    $this->additional_fields['type'] = array('table' => 'node', 'field' => 'type');
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    $this->additional_fields['vid'] = array('table' => 'node', 'field' => 'vid');
  }

  function render($values) {
    // Build a pseudo-node from the retrieved values.
    $nid = $values->{$this->aliases['nid']};
    $node = node_load($nid);

    $func = $this->definition['handler function'];
    if (function_exists($func)) {
      $content = $func($node);
    }
    return $content;
  }
}
