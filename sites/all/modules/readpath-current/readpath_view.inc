<?php
  $view = new stdClass();
  $view->name = 'readpath';
  $view->description = 'Source view for read path module.';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Unread posts';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = '';
  $view->page_empty_format = '1';
  $view->page_type = 'list';
  $view->url = 'readpath';
  $view->use_pager = FALSE;
  $view->nodes_per_page = '25';
  $view->menu = TRUE;
  $view->menu_title = 'Readpath';
  $view->menu_tab = FALSE;
  $view->menu_tab_weight = '0';
  $view->menu_tab_default = FALSE;
  $view->menu_tab_default_parent = NULL;
  $view->menu_tab_default_parent_type = 'tab';
  $view->menu_parent_tab_weight = '0';
  $view->menu_parent_title = '';
  $view->sort = array (
    array (
      'tablename' => 'node_comment_statistics',
      'field' => 'last_comment_timestamp',
      'sortorder' => 'ASC',
      'options' => 'normal',
    ),
  );
  $view->argument = array (
  );
  $view->field = array (
    array (
      'tablename' => 'node',
      'field' => 'nid',
      'label' => '',
    ),
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => '',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'forum',
),
    ),
    array (
      'tablename' => 'history',
      'field' => 'timestamp',
      'operator' => '0',
      'options' => '',
      'value' => array (
  0 => '0',
),
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node_comment_statistics, node, history);
  $views[$view->name] = $view;
