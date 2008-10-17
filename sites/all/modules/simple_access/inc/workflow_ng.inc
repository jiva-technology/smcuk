<?php
// $Id: workflow_ng.inc,v 1.1.2.3 2008/10/02 04:19:17 gordon Exp $ 

/**
 * @file
 * Add workflow ng functionality to simple access
 */

/**
 * Implementation of hook_action_info()
 */
function simple_access_action_info() {
  return array(
    'simple_access_action_owner_grant' => array(
      '#label' => t('Grant permissions to content owner'),
      '#arguements' => array(
        'node' => array(
          '#entity' => 'node',
          '#label' => t('Content'),
        ),
      ),
      '#module' => t('Node'),
    ),
    'simple_access_action_owner_revoke' => array(
      '#label' => t('Revoke permissions from content owner'),
      '#arguements' => array(
        'node' => array(
          '#entity' => 'node',
          '#label' => t('Content'),
        ),
      ),
      '#module' => t('Node'),
    ),
    'simple_access_action_group_grant' => array(
      '#label' => t('Grant permissions to groups'),
      '#arguements' => array(
        'node' => array(
          '#entity' => 'node',
          '#label' => t('Content'),
        ),
      ),
      '#module' => t('Node'),
    ),
    'simple_access_action_group_revoke' => array(
      '#label' => t('Revoke permissions from groups'),
      '#arguements' => array(
        'node' => array(
          '#entity' => 'node',
          '#label' => t('Content'),
        ),
      ),
      '#module' => t('Node'),
    ),
    'simple_access_action_profile_enable' => array(
      '#label' => t('Enable access profile'),
      '#arguements' => array(
        'node' => array(
          '#entity' => 'node',
          '#label' => t('Content'),
        ),
      ),
      '#module' => t('Node'),
    ),
    'simple_access_action_profile_disable' => array(
      '#label' => t('Disable access profile'),
      '#arguements' => array(
        'node' => array(
          '#entity' => 'node',
          '#label' => t('Content'),
        ),
      ),
      '#module' => t('Node'),
    ),
  );
};

/**
 * Configure grant content owner permissions
 */
function simple_access_action_owner_grant_form($settings = array()) {
  $form = array();

  $form['sa_owner_permissions'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Grant owner permissions'),
    '#default_value' => $settings['sa_owner_permissions'],
    '#options' => array(
      'sa_view' => t('View'),
      'sa_update' => t('Update'),
      'sa_delete' => t('Delete'),
    ),
    '#description' => t('Select permissions to grant for the content owner'),
  );

  return $form;
}

function simple_access_action_owner_grant_submit($form_id, $form_values) {
  $settings = array('sa_owner_permissions' => $form_values['sa_owner_permissions']);
  return $settings;
}

/**
 * Action to grant permissions to the owner
 */
function simple_access_action_owner_grant($conf, $data) {
  $node = $data['data']['node'];

  foreach (array_filter($conf['sa_owner_permissions']) as $option) {
    $node->simple_access_owner[$option] = 1;
  }

  return array('node' => $node);
}

/**
 * Configure revoke content owner permissions
 */
function simple_access_action_owner_revoke_form($settings = array()) {
  $form = array();

  $form['sa_owner_permissions'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Revoke owner permissions'),
    '#default_value' => $settings['sa_owner_permissions'],
    '#options' => array(
      'sa_view' => t('View'),
      'sa_update' => t('Update'),
      'sa_delete' => t('Delete'),
    ),
    '#description' => t('Select permissions to revoke for the content owner'),
  );

  return $form;
}

function simple_access_action_owner_revoke_submit($form_id, $form_values) {
  $settings = array('sa_owner_permissions' => $form_values['sa_owner_permissions']);
  return $settings;
}

/**
 * Action to grant permissions to the owner
 */
function simple_access_action_owner_revoke($conf, $data) {
  $node = $data['data']['node'];

  foreach (array_filter($conf['sa_owner_permissions']) as $option) {
    $node->simple_access_owner[$option] = 0;
  }

  return array('node' => $node);
}

/**
 * Configure grant group permissions
 */
function simple_access_action_group_grant_form($settings = array()) {
  $form = array();

  $form['sa_group_permissions'] = array(
    '#tree' => TRUE,
    '#theme' => 'simple_access_form',
    '#admin' => TRUE,
  );

  $groups = simple_access_get_groups();

  foreach ($groups as $gid => $group) {
    $form['sa_group_permissions'][$gid]['name'] = array(
      '#value' => $group['name'],
    );
    $form['sa_group_permissions'][$gid]['sa_view'] = array(
      '#type' => 'checkbox',
      '#default_value' => $settings['sa_group_permissions'][$gid]['sa_view'],
    );
    $form['sa_group_permissions'][$gid]['sa_update'] = array(
      '#type' => 'checkbox',
      '#default_value' => $settings['sa_group_permissions'][$gid]['sa_update'],
    );
    $form['sa_group_permissions'][$gid]['sa_delete'] = array(
      '#type' => 'checkbox',
      '#default_value' => $settings['sa_group_permissions'][$gid]['sa_delete'],
    );
  }

  return $form;
}

function simple_access_action_group_grant_submit($form_id, $form_values) {
  $settings = array('sa_group_permissions' => $form_values['sa_group_permissions']);
  return $settings;
}

/**
 * Action to grant permissions to the owner
 */
function simple_access_action_group_grant($conf, $data) {
  $node = $data['data']['node'];

  foreach ($conf['sa_group_permissions'] as $gid => $group) {
    foreach (array_keys(array_filter($group)) as $option) {
      $node->simple_access[$gid][$option] = 1;
    }
  }

  return array('node' => $node);
}

/**
 * Configure revoke group permissions
 */
function simple_access_action_group_revoke_form($settings = array()) {
  $form = array();

  $form['sa_group_permissions'] = array(
    '#tree' => TRUE,
    '#theme' => 'simple_access_form',
    '#admin' => TRUE,
  );

  $groups = simple_access_get_groups();

  foreach ($groups as $gid => $group) {
    $form['sa_group_permissions'][$gid]['name'] = array(
      '#value' => $group['name'],
    );
    $form['sa_group_permissions'][$gid]['sa_view'] = array(
      '#type' => 'checkbox',
      '#default_value' => $settings['sa_group_permissions'][$gid]['sa_view'],
    );
    $form['sa_group_permissions'][$gid]['sa_update'] = array(
      '#type' => 'checkbox',
      '#default_value' => $settings['sa_group_permissions'][$gid]['sa_update'],
    );
    $form['sa_group_permissions'][$gid]['sa_delete'] = array(
      '#type' => 'checkbox',
      '#default_value' => $settings['sa_group_permissions'][$gid]['sa_delete'],
    );
  }

  return $form;
}

function simple_access_action_group_revoke_submit($form_id, $form_values) {
  $settings = array('sa_group_permissions' => $form_values['sa_group_permissions']);
  return $settings;
}

/**
 * Action to revoke permissions to the owner
 */
function simple_access_action_group_revoke($conf, $data) {
  $node = $data['data']['node'];

  foreach ($conf['sa_group_permissions'] as $gid => $group) {
    foreach (array_keys(array_filter($group)) as $option) {
      $node->simple_access[$gid][$option] = 0;
    }
  }

  return array('node' => $node);
}

/**
 * Configure enable security profile
 */
function simple_access_action_profile_enable_form($settings = array()) {
  $form = array();

  $form['sa_profiles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Access profiles'),
    '#default_value' => $settings['sa_profiles'],
    '#options' => simple_access_get_profiles_select(),
    '#description' => t('Select permissions to grant for the content owner'),
  );

  return $form;
}

function simple_access_action_profile_enable_submit($form_id, $form_values) {
  $settings = array('sa_profiles' => $form_values['sa_profiles']);
  return $settings;
}

/**
 * Action enable access profile
 */
function simple_access_action_profile_enable($conf, $data) {
  $node = $data['data']['node'];

  foreach (array_filter($conf['sa_profiles']) as $pid) {
    if (!in_array($pid, $node->simple_access_profiles)) {
      $node->simple_access_profiles[] = $pid;
    }
  }

  return array('node' => $node);
}

/**
 * Configure disable security profile
 */
function simple_access_action_profile_disable_form($settings = array()) {
  $form = array();

  $form['sa_profiles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Access profiles'),
    '#default_value' => $settings['sa_profiles'],
    '#options' => simple_access_get_profiles_select(),
    '#description' => t('Select permissions to grant for the content owner'),
  );

  return $form;
}

function simple_access_action_profile_disable_submit($form_id, $form_values) {
  $settings = array('sa_profiles' => $form_values['sa_profiles']);
  return $settings;
}

/**
 * Action to disable access profile
 */
function simple_access_action_profile_disable($conf, $data) {
  $node = $data['data']['node'];

  foreach (array_filter($conf['sa_profiles']) as $pid) {
    if (in_array($pid, $node->simple_access_profiles)) {
      unset($node->simple_access_profiles[array_search($pid, $node->simple_access_profiles)]);
    }
  }

  return array('node' => $node);
}

