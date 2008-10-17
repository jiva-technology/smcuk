<?php
  $view = new stdClass();
  $view->name = 'user_list';
  $view->description = 'Filterable list of users with profiles';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'User Listing';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'No neighbors match your criteria';
  $view->page_empty_format = '1';
  $view->page_type = 'teaser';
  $view->url = 'userlisting';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '10';
  $view->menu = TRUE;
  $view->menu_title = 'User Listing';
  $view->menu_tab = FALSE;
  $view->menu_tab_weight = '0';
  $view->menu_tab_default = FALSE;
  $view->menu_tab_default_parent = NULL;
  $view->menu_parent_tab_weight = '0';
  $view->menu_parent_title = '';
  $view->sort = array (
  );
  $view->argument = array (
  );
  $view->field = array (
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'uprofile',
),
    ),
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'term_node_1',
      'field' => 'tid',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
),
    ),
    array (
      'tablename' => 'node_data_field_gender',
      'field' => 'field_gender_value_default',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'Male',
  1 => 'Female',
),
    ),
    array (
      'tablename' => 'node_data_field_status',
      'field' => 'field_status_value_default',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'Single',
  1 => 'Married',
  2 => 'Divorced',
  3 => 'In a Relationship',
),
    ),
    array (
      'tablename' => 'node_data_field_children',
      'field' => 'field_children_value_default',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'No Children',
  1 => 'Have Child(ren)',
  2 => 'My Pets Are My Kids',
),
    ),
  );
  $view->exposed_filter = array (
    array (
      'tablename' => 'term_node_1',
      'field' => 'tid',
      'label' => 'Community',
      'optional' => '1',
      'is_default' => '0',
      'operator' => '1',
      'single' => '1',
    ),
    array (
      'tablename' => 'node_data_field_gender',
      'field' => 'field_gender_value_default',
      'label' => 'Gender',
      'optional' => '1',
      'is_default' => '0',
      'operator' => '1',
      'single' => '1',
    ),
    array (
      'tablename' => 'node_data_field_status',
      'field' => 'field_status_value_default',
      'label' => 'Status',
      'optional' => '1',
      'is_default' => '0',
      'operator' => '1',
      'single' => '1',
    ),
    array (
      'tablename' => 'node_data_field_children',
      'field' => 'field_children_value_default',
      'label' => 'Children',
      'optional' => '1',
      'is_default' => '0',
      'operator' => '1',
      'single' => '1',
    ),
  );
  $view->requires = array(node, term_node_1, node_data_field_gender, node_data_field_status, node_data_field_children);
  $views[$view->name] = $view;
  