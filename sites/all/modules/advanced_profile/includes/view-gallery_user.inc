<?php
  $view = new stdClass();
  $view->name = 'gallery_user';
  $view->description = 'user photo gallery';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'users\' Galleries';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '<br class="clear">';
  $view->page_footer_format = '3';
  $view->page_empty = 'No photos available for this user.';
  $view->page_empty_format = '1';
  $view->page_type = 'bonus_grid';
  $view->url = 'gallery/users';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '10';
  $view->menu = TRUE;
  $view->menu_title = '';
  $view->menu_tab = TRUE;
  $view->menu_tab_weight = '2';
  $view->menu_tab_default = FALSE;
  $view->menu_tab_default_parent = NULL;
  $view->menu_parent_tab_weight = '2';
  $view->menu_parent_title = 'Gallery';
  $view->sort = array (
    array (
      'tablename' => 'users',
      'field' => 'name',
      'sortorder' => 'ASC',
      'options' => '',
    ),
    array (
      'tablename' => 'node',
      'field' => 'created',
      'sortorder' => 'ASC',
      'options' => 'normal',
    ),
  );
  $view->argument = array (
    array (
      'type' => 'uid',
      'argdefault' => '4',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
    array (
      'tablename' => 'users',
      'field' => 'name',
      'label' => '',
      'defaultsort' => 'ASC',
    ),
    array (
      'tablename' => 'node_data_field_image',
      'field' => 'field_image_fid',
      'label' => '',
      'handler' => 'content_views_field_handler_group',
      'options' => 'thickbox][thumbnail-square',
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
  0 => 'image',
),
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(users, node, node_data_field_image);
  $views[$view->name] = $view;
