<?php
// $Id: paths.inc,v 1.1 2007/06/27 21:44:51 nedjo Exp $

/**
 * @file
 *  Define paths to exclude from dynamic loading.
 */

function views_bookmark_dynamicload_paths() {
  return array('views_bookmark*');
}

function fivestar_dynamicload_paths() {
  return array('fivestar/vote*');
}

function fasttoggle_dynamicload_paths() {
  return array('*/toggle/*');
}

function user_dynamicload_paths() {
  return array(
    'logout',
    'user/login',
    'user/register',
  );
}
