<?php
$view = new stdClass();
  $view->name = 'user_tracker';
  $view->description = 'Shows posts started by or participated in by a given user';
  $view->access = array (
);
  $view->view_args_php = 'global $user;
// Make the first argument the current user if not already set
if (!$args[0]) {
  $args[0] = $user->uid;
}
';
  $view->page = TRUE;
  $view->page_title = 'My Posts';
  $view->page_header = '';
  $view->page_header_format = '4';
  $view->page_footer = '';
  $view->page_footer_format = '4';
  $view->page_empty = 'No posts found.';
  $view->page_empty_format = '4';
  $view->page_type = 'table';
  $view->url = 'user/$arg/tracker';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '15';
  $view->sort = array (
    array (
      'tablename' => 'node_comment_statistics',
      'field' => 'last_comment_timestamp',
      'sortorder' => 'DESC',
      'options' => 'normal',
    ),
  );
  $view->argument = array (
    array (
      'type' => 'uidtouch',
      'argdefault' => '7',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Title',
      'handler' => 'views_handler_field_nodelink_with_mark',
      'options' => 'link',
    ),
    array (
      'tablename' => 'users',
      'field' => 'name',
      'label' => 'Poster',
    ),
    array (
      'tablename' => 'node_comment_statistics',
      'field' => 'comment_count',
      'label' => 'Replies',
      'handler' => 'views_handler_comments_with_new',
    ),
    array (
      'tablename' => 'node_comment_statistics',
      'field' => 'last_comment_timestamp',
      'label' => 'Updated',
      'handler' => 'views_handler_field_since',
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
  0 => 'blog',
  1 => 'book',
  2 => 'forum',
  3 => 'advpoll_ranking',
  4 => 'advpoll_binary',
  5 => 'page',
  6 => 'story',
),
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node_comment_statistics, node, users);
  $views[$view->name] = $view;
