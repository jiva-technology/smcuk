<?php
// $Id: user.inc,v 1.1.2.4 2007/08/08 21:26:08 nedjo Exp $

function user_activeedit_autocompletes() {
  $autocompletes = array();
  $autocompletes['user'] = array(
    '#title' => t('users'),
    '#marker' => 'user/autocomplete',
    '#submit_text' => t('Create new account'),
    '#access' => user_access('administer users'),
    '#target' => 'admin/user/user/create',
    '#form' => array(
      'user_register' => array(),
    ),
    '#transfer_element' => 'name',
  );
  return $autocompletes;
}