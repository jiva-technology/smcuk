<?php
$view = new stdClass();
  $view->name = 'buddylist_of_uid';
  $view->description = 'Buddies of the given user';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Buddylist';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'User has no buddies';
  $view->page_empty_format = '1';
  $view->page_type = 'list';
  $view->url = 'buddies-of-uid';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '10';
  $view->sort = array (
    array (
      'tablename' => 'node',
      'field' => 'created',
      'sortorder' => 'DESC',
      'options' => 'normal',
    ),
  );
  $view->argument = array (
    array (
      'type' => 'buddylist_buddies',
      'argdefault' => '7',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
    array (
      'tablename' => 'users',
      'field' => 'uid',
      'label' => '',
    ),
    array (
      'tablename' => 'usernode_users',
      'field' => 'name',
      'label' => '',
      'handler' => 'usernode_views_handler_field_username_text',
    ),
    array (
      'tablename' => 'usernode_users',
      'field' => 'uid',
      'label' => '',
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
  0 => 'usernode',
),
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node, users, usernode_users);

  $views[$view->name] = $view;
