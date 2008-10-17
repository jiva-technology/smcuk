<?php
/**
 * Implementation of hook_default_panel_minis()
 */
function _advanced_profile_default_panel_minis() {
  $mini = new stdClass();
  $mini->pid = 'new';
    $mini->name = 'user_basics';
    $mini->title = 'All About User';
    $mini->requiredcontexts = array (
    4 => 
    array (
      'name' => 'user',
      'id' => 1,
      'identifier' => 'User',
      'keyword' => 'user',
    ),
    5 => 
    array (
      'name' => 'node',
      'id' => 1,
      'identifier' => 'Node',
      'keyword' => 'node',
    ),
  );
    $mini->contexts = array();
    $mini->relationships = array();
  $display = new stdClass();
  $display->did = 'new';
  $display->name = NULL;
  $display->layout = 'flexible';
  $display->layout_settings = array (
    'width_type' => '%',
    'percent_width' => '98',
    'rows' => '1',
    'row_1' => 
    array (
      'columns' => '3',
      'width_1' => '20',
      'width_2' => '20',
      'width_3' => '60',
      'names' => 
      array (
        0 => 'Left',
        1 => 'Middle',
        2 => 'Right',
      ),
    ),
    'sidebars' => 
    array (
      'left' => 0,
      'left_width' => 200,
      'right' => 0,
      'right_width' => 200,
      'width_type' => 'px',
    ),
  );
  $display->panel_settings = array (
    'style' => 'default',
    'style_settings' => 
    array (
    ),
    'edit_style' => 'Edit style settings',
    'individual' => 0,
    'panel' => 
    array (
      'row_1_1' => 
      array (
        'style' => '',
        'edit_style' => 'Edit style settings',
      ),
      'row_1_2' => 
      array (
        'style' => '',
        'edit_style' => 'Edit style settings',
      ),
      'row_1_3' => 
      array (
        'style' => '',
        'edit_style' => 'Edit style settings',
      ),
    ),
    'did' => '11',
  );
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
    $pane->pid = 'new-1';
    $pane->panel = 'row_1_1';
    $pane->type = 'user_picture';
    $pane->subtype = 'description';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 'requiredcontext_user_1',
      'override_title' => 1,
      'override_title_text' => '',
      'css_id' => '',
      'css_class' => '',
    );
  $display->content['new-1'] = $pane;
  $display->panels['row_1_1'][0] = 'new-1';
  $pane = new stdClass();
    $pane->pid = 'new-2';
    $pane->panel = 'row_1_2';
    $pane->type = 'userinfo';
    $pane->subtype = 'description';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 'requiredcontext_user_1',
      'override_title' => 1,
      'override_title_text' => '',
      'css_id' => '',
      'css_class' => '',
    );
  $display->content['new-2'] = $pane;
  $display->panels['row_1_2'][0] = 'new-2';
  $pane = new stdClass();
    $pane->pid = 'new-3';
    $pane->panel = 'row_1_3';
    $pane->type = 'content_fieldgroup';
    $pane->subtype = 'description';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 'requiredcontext_node_1',
      'override_title' => 1,
      'override_title_text' => '',
      'css_id' => '',
      'css_class' => '',
      'type_name' => 'uprofile',
      'group' => 'group_about',
      'empty' => '',
    );
  $display->content['new-3'] = $pane;
  $display->panels['row_1_3'][0] = 'new-3';
  $mini->display = $display;
  $minis['user_basics'] = $mini;


  $mini = new stdClass();
  $mini->pid = 'new';
    $mini->name = 'guestbook';
    $mini->title = 'Guestbook';
    $mini->requiredcontexts = array (
    0 => 
    array (
      'name' => 'node',
      'id' => 1,
      'identifier' => 'Node',
      'keyword' => 'node',
    ),
  );
    $mini->contexts = array();
    $mini->relationships = array();
  $display = new stdClass();
  $display->did = 'new';
  $display->name = NULL;
  $display->layout = 'twocol';
  $display->layout_settings = NULL;
  $display->panel_settings = NULL;
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
    $pane->pid = 'new-1';
    $pane->panel = 'left';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->access = array();
    $pane->configuration = array (
      'override_title' => 0,
      'override_title_text' => '',
      'css_id' => '',
      'css_class' => '',
      'title' => '',
      'body' => 'Welcome to my guestbook. Please leave a message.',
      'format' => '1',
    );
  $display->content['new-1'] = $pane;
  $display->panels['left'][0] = 'new-1';
  $pane = new stdClass();
    $pane->pid = 'new-2';
    $pane->panel = 'left';
    $pane->type = 'node_comments';
    $pane->subtype = 'comments';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 'requiredcontext_node_1',
      'override_title' => 1,
      'override_title_text' => '',
      'css_id' => '',
      'css_class' => '',
      'mode' => '4',
      'order' => '1',
      'comments_per_page' => '50',
    );
  $display->content['new-2'] = $pane;
  $display->panels['left'][1] = 'new-2';
  $pane = new stdClass();
    $pane->pid = 'new-3';
    $pane->panel = 'right';
    $pane->type = 'node_comment_form';
    $pane->subtype = 'comment_form';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 'requiredcontext_node_1',
      'override_title' => 1,
      'override_title_text' => '',
      'css_id' => '',
      'css_class' => '',
    );
  $display->content['new-3'] = $pane;
  $display->panels['right'][0] = 'new-3';
  $mini->display = $display;
  $minis['guestbook'] = $mini;


  return $minis;
}


/**
 * Implementation of hook_default_panel_views().
 */
function _advanced_profile_default_panel_views() {
  $panel_view = new stdClass();
    $panel_view->pvid = 'new';
    $panel_view->view = 'blog_of_uid';
    $panel_view->name = 'blog_of_uid';
    $panel_view->description = 'Blog teaser list';
    $panel_view->title = 'My Blog';
    $panel_view->category = 'Advanced Profile';
    $panel_view->category_weight = '-1';
    $panel_view->view_type = 'block';
    $panel_view->use_pager = '1';
    $panel_view->pager_id = '0';
    $panel_view->nodes_per_page = '5';
    $panel_view->offset = '0';
    $panel_view->link_to_view = '0';
    $panel_view->more_link = '1';
    $panel_view->feed_icons = '1';
    $panel_view->url_override = '0';
    $panel_view->url = '';
    $panel_view->url_from_panel = '0';
    $panel_view->contexts = array (
    0 => 
    array (
      'type' => 'context',
      'context' => 'user',
      'panel' => '0',
      'fixed' => '',
      'label' => 'User: UID is Author',
    ),
  );
    $panel_view->allow_type = '0';
    $panel_view->allow_nodes_per_page = '0';
    $panel_view->allow_offset = '0';
    $panel_view->allow_use_pager = '1';
    $panel_view->allow_link_to_view = '0';
    $panel_view->allow_more_link = '1';
    $panel_view->allow_feed_icons = '1';
    $panel_view->allow_url_override = '0';
    $panel_view->allow_url_from_panel = '0';
  $panel_views['blog_of_uid'] = $panel_view;


  $panel_view = new stdClass();
    $panel_view->pvid = 'new';
    $panel_view->view = 'buddylist_of_uid';
    $panel_view->name = 'buddylist_of_uid';
    $panel_view->description = 'Buddies of the given user';
    $panel_view->title = 'My Buddies';
    $panel_view->category = 'Advanced Profile';
    $panel_view->category_weight = '-1';
    $panel_view->view_type = 'embed';
    $panel_view->use_pager = '1';
    $panel_view->pager_id = '0';
    $panel_view->nodes_per_page = '10';
    $panel_view->offset = '0';
    $panel_view->link_to_view = '0';
    $panel_view->more_link = '1';
    $panel_view->feed_icons = '0';
    $panel_view->url_override = '0';
    $panel_view->url = '';
    $panel_view->url_from_panel = '0';
    $panel_view->contexts = array (
    0 => 
    array (
      'type' => 'context',
      'context' => 'user',
      'panel' => '0',
      'fixed' => '',
      'label' => 'Buddylist: Usernode is buddy of UID',
    ),
  );
    $panel_view->allow_type = '0';
    $panel_view->allow_nodes_per_page = '1';
    $panel_view->allow_offset = '0';
    $panel_view->allow_use_pager = '1';
    $panel_view->allow_link_to_view = '0';
    $panel_view->allow_more_link = '1';
    $panel_view->allow_feed_icons = '0';
    $panel_view->allow_url_override = '0';
    $panel_view->allow_url_from_panel = '0';
  $panel_views['buddylist_of_uid'] = $panel_view;


  $panel_view = new stdClass();
    $panel_view->pvid = 'new';
    $panel_view->view = 'user_tracker';
    $panel_view->name = 'user_tracker';
    $panel_view->description = 'Shows posts started by or participated in by a given user';
    $panel_view->title = 'My Posts';
    $panel_view->category = 'Advanced Profile';
    $panel_view->category_weight = '-1';
    $panel_view->view_type = 'embed';
    $panel_view->use_pager = '1';
    $panel_view->pager_id = '0';
    $panel_view->nodes_per_page = '5';
    $panel_view->offset = '0';
    $panel_view->link_to_view = '0';
    $panel_view->more_link = '0';
    $panel_view->feed_icons = '1';
    $panel_view->url_override = '0';
    $panel_view->url = '';
    $panel_view->url_from_panel = '0';
    $panel_view->contexts = array (
    0 => 
    array (
      'type' => 'context',
      'context' => 'user',
      'panel' => '0',
      'fixed' => '',
      'label' => 'User Profile - Posts by user',
    ),
  );
    $panel_view->allow_type = '0';
    $panel_view->allow_nodes_per_page = '1';
    $panel_view->allow_offset = '0';
    $panel_view->allow_use_pager = '1';
    $panel_view->allow_link_to_view = '0';
    $panel_view->allow_more_link = '1';
    $panel_view->allow_feed_icons = '1';
    $panel_view->allow_url_override = '0';
    $panel_view->allow_url_from_panel = '0';
  $panel_views['user_tracker'] = $panel_view;


  return $panel_views;
}


/**
 * Implementation of hook_default_panel_pages()
 */
function _advanced_profile_default_panel_pages() {
  $page = new stdClass();
  $page->pid = 'new';
    $page->name = 'userprofile';
    $page->title = 'User Profile';
    $page->arguments = array (
    1 => 
    array (
      'name' => 'uid',
      'default' => 'ignore',
      'title' => '%profilenode',
      'id' => 1,
      'identifier' => 'User ID',
      'keyword' => 'user',
      'argument_settings' => NULL,
    ),
  );
    $page->contexts = array();
    $page->relationships = array (
    0 => 
    array (
      'context' => 'argument_uid_1',
      'name' => 'node_from_user',
      'id' => 1,
      'identifier' => 'Node from user',
      'keyword' => 'profilenode',
    ),
  );
    $page->access = array();
    $page->path = 'user/%';
    $page->css_id = 'user-profile';
    $page->css = NULL;
    $page->no_blocks = '0';
    $page->menu = '0';
    $page->menu_tab = NULL;
    $page->menu_tab_weight = NULL;
    $page->menu_title = NULL;
    $page->menu_tab_default = NULL;
    $page->menu_tab_default_parent_type = NULL;
    $page->menu_parent_title = NULL;
    $page->menu_parent_tab_weight = NULL;
  $display = new stdClass();
  $display->did = 'new';
  $display->name = NULL;
  $display->layout = 'flexible';
  $display->layout_settings = array (
    'width_type' => '%',
    'percent_width' => '100',
    'rows' => '3',
    'row_1' => 
    array (
      'columns' => '1',
      'width_1' => '100',
      'names' => 
      array (
        0 => 'Top',
      ),
    ),
    'row_2' => 
    array (
      'columns' => '2',
      'width_1' => '50',
      'width_2' => '50',
      'names' => 
      array (
        0 => 'Left',
        1 => 'Middle',
        2 => 'Right',
      ),
    ),
    'row_3' => 
    array (
      'columns' => '1',
      'width_1' => '100',
      'names' => 
      array (
        0 => 'Bottom',
      ),
    ),
    'sidebars' => 
    array (
      'left' => 0,
      'left_width' => 200,
      'right' => 0,
      'right_width' => 200,
      'width_type' => 'px',
    ),
  );
  $display->panel_settings = array (
    'style' => 'default',
    'style_settings' => 
    array (
    ),
    'edit_style' => 'Edit style settings',
    'individual' => 0,
    'panel' => 
    array (
      'row_1_1' => 
      array (
        'style' => '',
        'edit_style' => 'Edit style settings',
      ),
      'row_2_1' => 
      array (
        'style' => '',
        'edit_style' => 'Edit style settings',
      ),
      'row_2_2' => 
      array (
        'style' => '',
        'edit_style' => 'Edit style settings',
      ),
      'row_3_1' => 
      array (
        'style' => '',
        'edit_style' => 'Edit style settings',
      ),
    ),
    'did' => '12',
  );
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
    $pane->pid = 'new-1';
    $pane->panel = 'row_1_1';
    $pane->type = 'panels_mini';
    $pane->subtype = 'user_basics';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 
      array (
        0 => 'argument_uid_1',
        1 => 'relationship_node_from_user_1',
      ),
      'override_title' => 1,
      'override_title_text' => 'All About %user',
      'css_id' => '',
      'css_class' => '',
      'name' => 'user_basics',
    );
  $display->content['new-1'] = $pane;
  $display->panels['row_1_1'][0] = 'new-1';
  $pane = new stdClass();
    $pane->pid = 'new-2';
    $pane->panel = 'row_2_1';
    $pane->type = 'views2';
    $pane->subtype = 'user_tracker';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 
      array (
        0 => 'argument_uid_1',
      ),
      'override_title' => 1,
      'override_title_text' => 'Topics I\'ve Participated In',
      'css_id' => '',
      'css_class' => '',
      'title' => 'My Posts',
      'name' => 'user_tracker',
      'more_link' => 0,
      'feed_icons' => 1,
      'use_pager' => 1,
      'pager_id' => '0',
      'nodes_per_page' => '5',
    );
  $display->content['new-2'] = $pane;
  $display->panels['row_2_1'][0] = 'new-2';
  $pane = new stdClass();
    $pane->pid = 'new-3';
    $pane->panel = 'row_2_2';
    $pane->type = 'content_fieldgroup';
    $pane->subtype = 'description';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 'relationship_node_from_user_1',
      'override_title' => 1,
      'override_title_text' => 'Basic Info',
      'css_id' => '',
      'css_class' => '',
      'type_name' => 'uprofile',
      'group' => 'group_stats',
      'empty' => 'No info provided.',
    );
  $display->content['new-3'] = $pane;
  $display->panels['row_2_2'][0] = 'new-3';
  $pane = new stdClass();
    $pane->pid = 'new-4';
    $pane->panel = 'row_2_2';
    $pane->type = 'content_fieldgroup';
    $pane->subtype = 'description';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 'relationship_node_from_user_1',
      'override_title' => 1,
      'override_title_text' => 'Contact Me',
      'css_id' => '',
      'css_class' => '',
      'type_name' => 'uprofile',
      'group' => 'group_contact',
      'empty' => 'No contacts listed.',
    );
  $display->content['new-4'] = $pane;
  $display->panels['row_2_2'][1] = 'new-4';
  $pane = new stdClass();
    $pane->pid = 'new-5';
    $pane->panel = 'row_2_2';
    $pane->type = 'content_fieldgroup';
    $pane->subtype = 'description';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 'relationship_node_from_user_1',
      'override_title' => 1,
      'override_title_text' => 'Where I Hang Out',
      'css_id' => '',
      'css_class' => '',
      'type_name' => 'uprofile',
      'group' => 'group_links',
      'empty' => 'Just here.',
    );
  $display->content['new-5'] = $pane;
  $display->panels['row_2_2'][2] = 'new-5';
  $pane = new stdClass();
    $pane->pid = 'new-6';
    $pane->panel = 'row_3_1';
    $pane->type = 'panels_mini';
    $pane->subtype = 'guestbook';
    $pane->access = array();
    $pane->configuration = array (
      'context' => 
      array (
        0 => 'relationship_node_from_user_1',
      ),
      'override_title' => 0,
      'override_title_text' => '',
      'css_id' => '',
      'css_class' => '',
      'name' => 'guestbook',
    );
  $display->content['new-6'] = $pane;
  $display->panels['row_3_1'][0] = 'new-6';
  $page->display = $display;
  $page->displays = array();
  $pages['userprofile'] = $page;


  return $pages;
}