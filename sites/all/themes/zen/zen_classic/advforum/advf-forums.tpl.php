<?php
// $Id: advf-forums.tpl.php,v 1.5 2008/05/08 01:23:55 michellec Exp $

/**
 * @file forums.tpl.php
 * Default theme implementation to display a forum which may contain forum
 * containers as well as forum topics.
 *
 * Variables available:
 * - $links: An array of links that allow a user to post new forum topics.
 *   It may also contain a string telling a user they must log in in order
 *   to post. Empty if there are no topics on the page. (ie: forum overview)
 * - $links_orig: Same as $links but not emptied on forum overview page.
 * - $forums: The forums to display (as processed by forum-list.tpl.php)
 * - $topics: The topics to display (as processed by forum-topic-list.tpl.php)
 * - $forums_defined: A flag to indicate that the forums are configured.
 *
 * @see template_preprocess_forums()
 * @see advanced_forum_preprocess_forums()
 */
?>

<?php if ($forums_defined): ?>
<div id="viewfirstunreadpost">
<?php
// Fetch the next node id from the readpath                                                                                 
$next_nid = readpath_get_next_nid();

if ($next_nid > 0) {

  // Get the page # and comment id if any                                                                                   
  $link = readpath_get_link_info($next_nid);                                                                                

  $link['title'] = "Jump to first unread post";
  $link['href'] = "node/$next_nid";
  $links['next_unread'] = $link;
}
else {
  $links['next_unread']['title'] = "No unread posts.";
  $links['next_unread']['href'] = '';
}
?>
</div>

<div id="forum">

<div class="forum-description">
<?php print $forum_description ?>
</div>

  <div class="forum-top-links"><?php print theme('links', $links, array('class'=>'links forumlinks')); ?></div>
  <?php print $forums; ?>
  <?php print $topics; ?>
</div>
<?php endif; ?>
