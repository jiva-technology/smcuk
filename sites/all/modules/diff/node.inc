<?php
// $Id: node.inc,v 1.1.4.1 2008/02/10 04:34:41 weitzman Exp $

/**
 * Implementation of hook_diff() for node.module (body and title).
 */
function node_diff(&$old_node, &$new_node) {
  
  $result = array();
  $type = node_get_types('type', $new_node);
  $result[] = array(
    'name' => $type->title_label,
    'old' => array($old_node->title),
    'new' => array($new_node->title),
    'format' => array(
      'show_header' => false,
    )
  );
  if ($type->has_body) {
    $result[] = array(
      'name' => $type->body_label,
      'old' => explode("\n", $old_node->body),
      'new' => explode("\n", $new_node->body),
    );
  }
  return $result;	
}
