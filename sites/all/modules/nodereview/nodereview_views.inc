<?php
// $Id: nodereview_views.inc,v 1.5 2007/02/25 02:50:10 crell Exp $

/* ----- Views API ----- */

function nodereview_views_tables() {
 
  $tables['nodereview'] = array(
    'name' => 'nodereview',
    'provider' => 'nodereview',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid',
      ),
      'right' => array(
        'field' => 'nid'
      ),
    ),
    'filters' => array(
      'reviewed_nid' => array(
         'name' => t('Review: Reviewed Node'),
         'list' => 'nodereview_views_handler_filter_reviewed_node',
         'operator' => 'views_handler_operator_andor',
         'help' => t('This allows you to filter reviews based on the reviewed node.'),
      ),
    ),
  );
  
  return $tables;
}

function nodereview_views_arguments() {
  $arguments = array(
    'reviewed_node' => array(
      'name' => t("Review: Reviewed Node ID"), 
      'handler' => "nodereview_views_handler_arg_reviewed_node",
    ),
  );
  return $arguments;
}

function nodereview_views_handler_arg_reviewed_node($op, & $query, $argtype, $arg = '') {
  switch ($op) {
    case 'summary' :
      $query->ensure_table("nodereview");
      $query->add_field("reviewed_nid");
      $fieldinfo['field'] = "nodereview.reviewed_nid";
      return $fieldinfo;
    case 'sort':
      //$query->add_orderby('book', 'weight', $argtype);
      //$query->add_orderby('book_parent_node', 'title', $argtype);
      break;
    case 'filter' :
      $query->ensure_table("nodereview");
      $query->add_where("reviewed_nid = '$arg'");
      $query->add_where("nodereview.nid = node.nid");
      break;
    case 'link' :
      return l($query->title, "$arg/$query->nid");
    case 'title' :
      if ($query) {
        $term = db_fetch_object(db_query("SELECT title FROM {node} WHERE nid = '%d'", $query));
        return $term->title;
      }
  }
}

function nodereview_views_handler_filter_reviewed_node() {
  $reviewed_nids = array();

  $result = db_query("SELECT DISTINCT reviewed_nid FROM {nodereview} ORDER BY reviewed_nid");
  while ($obj = db_fetch_object($result)) {
    $reviewed_nids[$obj->reviewed_nid] = "$obj->reviewed_nid";
  }
  return $reviewed_nids;
}


function nodereview_views_default_views() {
  $view = new stdClass();
  $view->name = 'review_list';
  $view->description = 'A list of teasers for reviews of a given node';
  $view->access = array (
    0 => '1',
    1 => '2',
  );
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = '%1';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'No reviews have been submitted.  Maybe you should be the first?';
  $view->page_empty_format = '1';
  $view->page_type = 'teaser';
  $view->url = 'node/$arg/reviews';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '10';
  $view->sort = array (
    array (
      'tablename' => 'node',
      'field' => 'changed',
      'sortorder' => 'DESC',
      'options' => '',
    ),
  );
  $view->argument = array (
    array (
      'type' => 'reviewed_node',
      'argdefault' => '1',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
    array (
      'tablename' => 'node',
      'field' => 'changed',
      'label' => 'updated',
      'handler' => 'views_handler_field_date_small',
      'defaultsort' => 'DESC',
    ),
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'title',
      'handler' => 'views_handler_field_nodelink_with_mark',
      'options' => 'link',
    ),
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
        0 => 'nodereview',
      ),
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node);
  $views[$view->name] = $view;

  
  return $views;
}
