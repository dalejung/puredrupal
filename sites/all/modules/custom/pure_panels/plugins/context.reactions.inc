<?php
/**
 * Panels Template reactions
 */
class pure_panels_reaction extends context_reaction {

  function options_form($context) {
    $form = array();

    $template_types = pure_panels_get_template_types();
    foreach($template_types as $type => $template) {
      if ($template['context_enabled'] === FALSE) {
        continue;
      } 
      $options = array('' => t('N/A'));
      $options += array('none' => t('None'));
      $options += pure_panels_get_templates($type); 
      $form[$type] = array(
        '#type' => 'select',
        '#title' => $template['label'],
        '#options' => $options,
        '#default_value' => $this->get_from_context($context, $type), 
      );
    }
    return $form;
  }

  function get_from_context($context, $type) {
    return isset($context->reactions['panels_template'][$type]) ? $context->reactions['panels_template'][$type] : '';
  }

  /**
   * Grab the panel template found
   */
  function get_template($type) {
    $actives = $this->get_contexts();
    $default = '';
    $return = '';
    foreach($actives as $context) {
      if($val = $context->reactions['panels_template'][$type]) {
        if($context->conditions['sitewide']['values'][1]) {
          $default = $val;
        } else {
          return $val;
        }
      }
    }

    return $default;
  }
}
