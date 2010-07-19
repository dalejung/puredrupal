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

    $view = $this->definition['common_view view'];
    $content = common_view($view, $node);
    return $this->render_link($content, $values);
  }

  function render_link($data, $values) {
    if (!empty($this->options['link_to_node']) && $data !== NULL && $data !== '') {
      $node = node_load($values->{$this->aliases['nid']});
      $this->options['alter']['make_link'] = TRUE;
      // Grab link via common_vieww
      $this->options['alter']['path'] = common_view('link', $node);
      $this->options['alter']['alt'] = check_plain($data);
      if (isset($this->aliases['language'])) {
        $languages = language_list();
        if (isset($languages[$values->{$this->aliases['language']}])) {
          $this->options['alter']['language'] = $languages[$values->{$this->aliases['language']}];
        }
        else {
          unset($this->options['alter']['language']);
        }
      }
    }
    return $data;
  }
}
