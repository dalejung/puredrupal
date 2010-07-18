<?php
class common_view_handler_field_image extends views_handler_field {
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

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = $this->options;

    $field_types = _content_field_types();

    $type = 'filefield';

    $field_formatters = common_view_imagecache_formatters();
    $formatters = array(); 

    foreach ($field_formatters as $name => $info) {
      $formatters[$name] = $info['label'];
    }

    $form['format'] = array(
      '#title' => t('Format'),
      '#type' => 'select',
      '#options' => $formatters,
      '#required' => TRUE,
      '#default_value' => $options['format'],
      '#weight' => 4,
    );
  }

  function render($values) {
    // Build a pseudo-node from the retrieved values.
    $nid = $values->{$this->aliases['nid']};
    $node = node_load($nid);


    $func = $this->definition['handler function'];
    $image_node = $func($node);
    $image_path = common_view_image($image_node);
    $alt = common_view_image_alt($image_node);

    if (!$image_path) { return ''; } 

    $item['filepath'] = $image_path;

    $options = $this->options;
    $formatter = $options['format'];

    $parts =  explode('_', $formatter);
    $style = array_pop($parts);
    $presetname = implode('_', $parts);
    $class = "imagecache imagecache-$presetname imagecache-$style imagecache-$formatter";
    $link_options = array('attributes' => array('class' => $class), 'html' => TRUE);

    if ($preset = imagecache_preset_by_name($presetname)) {
      switch ($style) {
        case 'linked':
          $imagetag = theme('imagecache', $presetname, $item['filepath'], $alt, $title);
          $url = common_view_link($node);
          if (
            !empty($node->field_link_target_blank[0]['value'])
            && 'on' == $node->field_link_target_blank[0]['value']
          ) {
            $link_options['attributes']['target'] = '_blank';
          }
          return l($imagetag, $url, $link_options);

        case 'imagelink':
          $original_image_url = file_create_url($item['filepath']);
          $imagetag =  theme('imagecache', $presetname, $item['filepath'], $alt, $title);
          return l($imagetag, $original_image_url, $link_options);

        case 'url':
          return imagecache_create_url($presetname, $item['filepath']);

        case 'path':
          return imagecache_create_path($presetname, $item['filepath']);

        default:
          return theme('imagecache', $presetname, $item['filepath'], $alt, $title, array('class' => $class));
      }
    }
    return '<!-- imagecache formatter preset('. $presetname .') not found! -->';
  }
}
