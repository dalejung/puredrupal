<?php
// $Id: default.inc,v 1.1.2.2 2009/03/24 18:32:20 merlinofchaos Exp $

/**
 * plugin definition
 */
$plugin = array(
  'title' => t('Laserfist Template'),
  'description' => t('The template panel rendering style; displays each pane with a separator.'),
  'render panel' => 'panels_template_style_template_handler',
  'settings form' => 'panels_template_style_template_settings_form',
  'settings validate' => 'panels_template_style_template_settings_validate',
  'hook theme' => array(
    'panels_template_panel_layout' => array(
      'path' => drupal_get_path('module','panels_template').'/plugins/styles',
      'template' => 'panel-style-template',
      'arguments' => array('display' => null, 'panel_id' => null, 'panes' => array(), 'settings'=>array()),
    ),
    'panels_template_simple_pane' => array(
      'arguments' => array('content' => null, 'pane' => null, 'display' => null),
    ),
    'panels_template_default_render_panel' => array(
      'arguments' => array(
        'display' => null, 
        'panel_id' => null, 
        'panes' => null,
        'settings' => null,
        ),
    ),
  ),
);

// ---------------------------------------------------------------------------
// Panels style plugin callbacks.

/**
 * Render callback.
 *
 */
function theme_panels_template_style_template_handler($display, $panel_id, $panes, $settings) {
	$theme = $settings['template_theme'];
	// If we don't have theme, we don't have anything to do. Use default
	if(!$theme) {
		return theme('panels_template_default_render_panel',$display, $panel_id, $panes, $settings);
	}

  $output = '';

	$output .= theme('panels_template_panel_layout',$display, $panel_id, $panes, $settings);

  return $output;
}

/*
 * Render the Panel in a default kind of way
 *
 **/
function theme_panels_template_default_render_panel($display, $panel_id, $panes, $settings) {
  $output = '';

  $print_separator = FALSE;
  foreach ($panes as $pane_id => $content) {
    if ($content->content != '') {
      // Add the separator if we've already displayed a pane.
      if ($print_separator && $content->css_class != 'skip-separator') {
        $output .= '<div class="panel-separator"></div>';
      }
      $output .= $text = panels_render_pane($content, $display->content[$pane_id], $display);

      // If we displayed a pane, this will become true; if not, it will become
      // false.
      $print_separator = (bool) $text;
    }
  }
  return $output;
}

/*
 *  Prepare the tpl.php by setting template_file and stick
 *  pane content into $vars
*/
function template_preprocess_panels_template_panel_layout(&$vars) {
	$settings = $vars['settings'];
	$theme = $settings['template_theme'];
	$vars['theme'] = $theme;
	// We already checked before but do it again
	if(!$theme) {
		return;		
	}

	if($template_file = $settings['template_theme']) {
		$vars['template_files'][] = str_replace('_','-',$template_file);
	}


	$panes = $vars['panes'];
	$panel_id = $vars['panel_id'];
	$display = $vars['display'];

  // Render panes and puts into $vars so themer could play with tpl  
  $vars['pane_content'] = new stdClass();
  foreach ($panes as $pane_id => $content) {
		$pane_output = '';
		$admin_links = '';

		$content->rendered = TRUE;
		if (user_access('view pane admin links') && !empty($content->admin_links)) {
			$admin_links = '<div class="admin-links">'.theme('links', $content->admin_links).'</div>';
		}
		$pane_output .= $admin_links;
    $pane_output .= panels_template_render_pane($content, $display->content[$pane_id], $display,$settings);
		$pane_key = $content->type.'_'.$content->subtype;
		$pane_key = str_replace('-','_',$pane_key);
		$vars['pane_content']->$pane_key = $pane_output;
  }
}



/*
 * Panels Template version of the render pane
 **/
function panels_template_render_pane($content, $pane, $display,$settings) {
	// If not set, use the panels render pane
	if (!$settings['simple_theme']) {
		return panels_render_pane($content, $display->content[$pane_id], $display);	
	}

	// Respect individual Pane settings
  if (!empty($pane->style['style'])) {
    $style = panels_get_style($pane->style['style']);

    if (isset($style) && isset($style['render pane'])) {
      $output = theme($style['render pane'], $content, $pane, $display);

      // This could be null if no theme function existed.
      if (isset($output)) {
        return $output;
      }
    }
  }

  // if we're here, we want to use simple pane render 
  if (empty($node->style['style'])) {
    return theme('panels_template_simple_pane', $content, $pane, $display);
  }
}

/*
 * Minimize the output of pane
 *
 **/
function theme_panels_template_simple_pane($content, $pane, $display) {
  if (!empty($content->content)) {
    $idstr = $classstr = '';
    if (!empty($content->css_id)) {
      $idstr = ' id="' . $content->css_id . '"';
    }
    if (!empty($content->css_class)) {
      $classstr = $content->css_class;
    }

		/*
			No point of the wrapper dives if we don't have css class/id
			and no title;
		*/	
		$wrapper = false;
		if($classstr || $idstr || $content->title) {
			$wrapper = TRUE;
		}

		if($wrapper) {
    $output = "<div class=\"$classstr\"$idstr>\n";

			if (!empty($content->title)) {
				$output .= "<h2 class=\"title\">$content->title</h2>\n";
			}

			$output .= "<div class=\"content\">";
		}

		$output .= $content->content;

		if($wrapper) {
			$output .= "</div>\n";
			$output .= "</div>\n";
		}
    return $output;
  }
}


/**
 * Settings form callback.
 */
function panels_template_style_template_settings_form($style_settings) {
  $form['display_theme'] = array(
    '#type' => 'textfield',
    '#title' => t('Panels Display Template Theme'),
		'#description' => t('This will only take effect for the WHOLE Panels Display'),
    '#default_value' => (isset($style_settings['display_theme'])) ? $style_settings['display_theme'] : '',
  );
  $form['template_theme'] = array(
    '#type' => 'textfield',
    '#title' => t('Panel Template Theme'),
		'#description' => t('PSEUDO Theme hook to take over this panel layout'),
    '#default_value' => (isset($style_settings['template_theme'])) ? $style_settings['template_theme'] : '',
  );
  $form['simple_theme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Simple Theme'),
		'#description' => t('Remove Panel Styling'),
    '#default_value' => (isset($style_settings['simple_theme'])) ? $style_settings['simple_theme'] : 0,
  );

  return $form;
}

