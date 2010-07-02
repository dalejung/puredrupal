<?php

function bonnier_answers_answer_ask_link_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Answer Ask Link'),
    'description' => t('Answer Ask Link'),
    'category' => 'Answers',
  );
}

function bonnier_answers_answer_ask_link_content_type_render($subtype, $conf, $panel_args, $context) {

  $content = l('ask a question','answers/ask',array('attributes'=>array('class'=>'ask-a-question')));

  $block->content = $content;
  $block->title = 'Ask a question';
  $block->delta = 'answer_ask_link';
  $block->css_class = 'ask-a-question';
  // Added in by me for default list
  $block->skip_separator = TRUE;
  return $block;
}

function bonnier_answers_answer_ask_link_content_type_edit_form(&$form, &$form_state) {
    // provide a blank form so we have a place to have context setting.
}

