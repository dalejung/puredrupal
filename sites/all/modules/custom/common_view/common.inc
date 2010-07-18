<?php
/*
 * Provides a set of common image, dek, tout functions
 */


/**
 * Returns a common image filepath
 */
function common_view_image(&$node) {
  $hook = 'common_image';
  return common_view_process('common_view', $hook, $node);
}
function common_view_common_image(&$node) {
  switch ($node->type) {
    default:
      // Required Image
      if ($image = $node->field_image[0]['filepath']) {
        return $image;
      }
      // Optional image
      if ($image = $node->field_tout_image[0]['filepath']) {
        return $image;
      }

      // Image is via a noderef
      $image_nid = $node->field_main_photo[0]['nid'];
      if ($image_nid) {
        $image = common_view_image($image_nid);
      }
      break;
      
  }
  return $image;
}

/**
 * Returns a common image filepath
 */
function common_view_image_node(&$node) {
  $hook = 'common_image_node';
  return common_view_process('common_view', $hook, $node);
}

function common_view_common_image_node(&$node) {
  switch ($node->type) {
    default:
      $image_nid = $node->field_main_photo[0]['nid'];
      $image = node_load($image_nid);

      if (!$image && common_view_image($node)) {
        $image = $node;
      }    
      break;
  }
  return $image;
}

/**
 * Returns a common thumbnail image filepath
 */
function common_view_teaser_image(&$node) {
  $hook = 'common_teaser_image';
  return common_view_process('common_view', $hook, $node);
}
function common_view_common_teaser_image(&$node) {
  switch ($node->type) {
    case 'image':
      $image = common_view_image($node);
      break;
    default:
      $thumbnail_nid = $node->field_thumbnail[0]['nid'];
      if ($thumbnail_nid) {
        $image = common_view_teaser_image($thumbnail_nid);
      }
      // We have no thumbnail. Grab the main photo
      if (!$thumbnail_nid) {
        $image = common_view_image($node);
      }
      break;
  }
  return $image;
}

/**
 * Returns a common thumbnail image filepath
 */
function common_view_teaser_image_node(&$node) {
  $hook = 'common_teaser_image_node';
  return common_view_process('common_view', $hook, $node);
}

function common_view_common_teaser_image_node(&$node) {
  switch ($node->type) {
    default:
      $thumbnail_nid = $node->field_thumbnail[0]['nid'];
      if (!$thumbnail_nid) {
        $thumbnail_nid = $node->field_main_photo[0]['nid'];
      }
      $image = node_load($thumbnail_nid);

      if(!$image && common_view_teaser_image($node)) {
        $image = $node;
      }    
      break;
  }
  return $image;
}

/**
 * Returns a common Dek
 */
function common_view_dek(&$node) {
  $hook = 'common_dek';
  return common_view_process('common_view', $hook, $node);
}
function common_view_common_dek(&$node) {
  switch ($node->type) {
    default:
      $dek = $node->field_dek[0]['value'];
      if (empty($dek)) {
        $dek = $node->teaser;
      }
      break;
  }
  return $dek;
}

/**
 * Returns a common credit link. Either to a node or user
 */
function common_view_credit(&$node) {
  $hook = 'common_credit';
  return common_view_process('common_view', $hook, $node);
}
function common_view_common_credit(&$node) {
  switch ($node->type) {
    case 'article':
      $credit = $node->field_author[0]['nid']; 
      break;
    case 'image':
      $credit = $node->field_photo_credit[0]['nid']; 
      break;
    default:
      $credit = $node->field_author[0]['nid']; 
      break;
  }
  if ($credit) {
    return node_load($credit);
  }
}

/**
 * Returns a common credit link. Either to a node or user
 */
function common_view_title(&$node) {
  $hook = 'common_title';
  return common_view_process('common_view', $hook, $node);
}
function common_view_common_title(&$node) {
  switch ($node->type) {
    default:
      $title = $node->title; 
      break;
  }
  return $title;
}

/**
 * Returns a common node link.
 */
function common_view_link(&$node) {
  $hook = 'common_link';
  return common_view_process('common_view', $hook, $node);
}
function common_view_common_link(&$node) {
  // If we have a target node ref.
  if (!empty($node->field_target[0]['nid']) && is_numeric($node->field_target[0]['nid'])) {
    $url = 'node/' . $node->field_target[0]['nid'];
  }
  // Redirect path takes precendence.
  if (!empty($node->field_redirect[0]['value'])) {
    $url = $node->field_redirect[0]['value'];
  }
  
  // If nothing found yet, just use the node path.
  $url = $url ? $url : 'node/' . $node->nid;

  return $url;
}

/**
 * Returns a common image filepath.
 */
function common_view_image_alt(&$node) {
  $hook = 'common_image_alt';
  return common_view_process('common_view', $hook, $node);
}
function common_view_common_image_alt(&$node) {
  switch ($node->type) {
    default:
      if ($node->field_image[0]['data']['alt']) {
        $alt = $node->field_image[0]['data']['alt'];
      } 
      if (!$alt) {
        $alt = $node->title;
      }
      break;
  }
  return $alt;
}

/**
 * Allow imagecache_presets to be overridden per content type.
 */
function common_view_imagecache_preset(&$node, $preset) {
  $hook = 'common_imagecache_preset';
  return common_view_process('common_view', $hook, $node, $preset);
}
function common_view_common_imagecache_preset(&$node, $preset) {
  // Get all of the active presets.
  $presets = imagecache_presets();
  // Work out what the new preset would be named.
  $new_preset = $preset . '_' . $node->type;
  
  // If the preset exists, use it.
  if (array_key_exists($new_preset, $presets)) {
    $preset = $new_preset;
  }

  // By default just return the preset.
  return $preset;
}
