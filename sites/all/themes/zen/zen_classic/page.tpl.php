<?php

/**
 * This snippet loads up different page-type.tpl.php layout
 * files automatically. For use in a page.tpl.php file.
 *
 * This works with Drupal 4.5,  Drupal 4.6 and Drupal 4.7
 */

if ($node->type == 'blog') {/* check if it's a blog node */
  include 'page-blog.tpl.php'; /*load  page-blog.tpl.php */
  return; }

if ($node->type == 'wiki') {/* check if it's a wiki node */
  include 'page-wiki.tpl.php'; /*load  page-wiki.tpl.php */
  return; }

if ($node->type == 'userlink') {/* check if it's a socialbookmark node */
  include 'page-socialbookmark.tpl.php'; /*load  page-socialbookmark.tpl.php */
  return; }

if ($node->type == 'chatroom') {/* check if it's a socialbookmark node */
  include 'page-chatroom.tpl.php'; /*load  page-chatroom.tpl.php */
  return; }

if ($node->type == 'forum') {/* check if it's a socialbookmark node */
  include 'page-forum.tpl.php'; /*load  page-forum.tpl.php */
  return; }

include 'page-default.tpl.php'; /*if none of the above applies, load the page-default.tpl.php */
return;

?>