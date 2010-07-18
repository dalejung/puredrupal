<?php
class common_view_handler_field_node extends views_handler_field_node {
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
    return $this->render_link($content, $values);
  }

  function render_link($data, $values) {
    if (!empty($this->options['link_to_node']) && $data !== NULL && $data !== '') {
      $node = node_load($values->{$this->aliases['nid']});
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = common_view_link($node);
      $this->options['alter']['alt'] = check_plain($data);
      if (
        !empty($node->field_link_target_blank[0]['value']) 
        && 'on' == $node->field_link_target_blank[0]['value']
      ) {
        $this->options['alter']['target'] = '_blank';
      } else {
        // Somewhere up the chain, this object is being reused when processing a 
        // group of links. As such, the target property carries through from item 
        // to item. This line is necessary to prevent ALL links from gaining a 
        // target after the initial target occures.
        unset($this->options['alter']['target']);
      }
    }
    return $data;
  }
}
