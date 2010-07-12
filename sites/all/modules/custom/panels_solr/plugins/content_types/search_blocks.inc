<?php
// $Id$

/**
 * @file
 * Provide Drupal blocks as content.
 *
 * Since blocks don't provide all of the features we do, we have to do a little
 * extra work, including providing icons and categories for core blocks. Blocks
 * from contrib modules get to provide their own stuff, or get relegated to
 * the old "Miscellaneous" category.
 */

/**
 * Callback function to supply a list of content types.
 */
function panels_solr_search_blocks_ctools_content_types() {
  return array(
    // And this is just the administrative title.
    // All our callbacks are named according to the standard pattern and can be deduced.
    'title' => t('Block'),
		'render last' => TRUE,
  );
}

/**
 * Return all block content types available.
 *
 * Modules wanting to make special adjustments the way that CTools handles their blocks
 * can implement an extension to the hook_block() family, where the function name is
 * of the form "$module . '_panels_solr_search_blocks_info'".
 */
function panels_solr_search_blocks_content_type_content_types() {
  $types = array();
	$search_modules = array('apachesolr','apachesolr_search');

	foreach($search_modules as $module) {
		$module_blocks = module_invoke($module, 'block', 'list');
		if ($module_blocks) {
			foreach ($module_blocks as $delta => $block) {
				// strip_tags used because it goes through check_plain and that
				// just looks bad.
				$info = array(
					'title' => strip_tags($block['info']),
          'category' => 'Panels Solr',
				);

				// this check means modules can remove their blocks; particularly useful
				// if they offer the block some other way (like we do for views)
				if ($info) {
					$types["$module-$delta"] = $info;
				}
			}
		}
	}
  return $types;
}

/**
 * Output function for the 'block' content type. Outputs a block
 * based on the module and delta supplied in the configuration.
 */
function panels_solr_search_blocks_content_type_render($subtype, $conf) {
  list($module, $delta) = _panels_solr_search_blocks_get_module_delta($subtype, $conf);
  $block = (object) module_invoke($module, 'block', 'view', $delta);

  if (empty($block)) {
    return;
  }

  $block->module = $module;
  $block->delta = $delta;
  $block->css_class = str_replace('_','-',$block->delta.' '.$block->module);	
  	

  if ($module == 'block' && empty($conf['override_title'])) {
    $block->subject = db_result(db_query("SELECT title FROM {blocks} WHERE module = '%s' AND delta = '%s'", 'block', $delta));
  }

  if (isset($block->subject)) {
    $block->title = $block->subject;
  }
  else {
    $block->title = NULL;
  }

  //$block->title = '';
  $block->css_class = '';

  if (user_access('administer blocks')) {
    $block->admin_links = array(
      array(
        'title' => t('Configure block'),
        'alt' => t("Configure this block's 'block settings' in administer >> site building >> blocks"),
        'href' => "admin/build/block/configure/$module/$delta",
        'query' => drupal_get_destination(),
      ),
    );
  }

  return $block;
}

/**
 * Empty form so we can have the default override title.
 */
function panels_solr_search_blocks_content_type_edit_form(&$form, &$form_state) {
  // Does nothing!
}

/**
 * Returns the administrative title for a type.
 */
function panels_solr_search_blocks_content_type_admin_title($subtype, $conf) {
  list($module, $delta) = _panels_solr_search_blocks_get_module_delta($subtype, $conf);
  $block = module_invoke($module, 'block', 'list');
  if (empty($block) || empty($block[$delta])) {
    return t('Deleted/missing block @module-@delta', array('@module' => $module, '@delta' => $delta));
  }

  $title = filter_xss_admin($block[$delta]['info']);
  return $title;
}

/**
 * Output function for the 'block' content type. Outputs a block
 * based on the module and delta supplied in the configuration.
 */
function panels_solr_search_blocks_content_type_admin_info($subtype, $conf) {
  list($module, $delta) = _panels_solr_search_blocks_get_module_delta($subtype, $conf);
  $blocks =  module_invoke($module, 'block', 'list');
	$block = (object) $blocks[$delta];
  if (!empty($block) && !empty($block->info)) {
    $block->title = $block->info;
    $block->content = $block->info;
    return $block;
  }
}

function _panels_solr_search_blocks_get_module_delta($subtype, $conf) {
  if (strpos($subtype, '-')) {
    return explode('-', $subtype, 2);
  }
  else {
    return array($conf['module'], $conf['delta']);
  }
}
