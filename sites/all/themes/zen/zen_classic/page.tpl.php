<?php
	/**
	 * This snippet loads up different page-type.tpl.php layout
	 * files automatically. For use in a page.tpl.php file.
	 *
	 * This works with Drupal 4.5,  Drupal 4.6 and Drupal 4.7
	 */
	switch(true) {
		case $node->type == 'blog':
			include 'page-blog.tpl.php';
			break;
		case $node->type == 'wiki':
			include 'page-wiki.tpl.php';
			break;
		case $node->type == 'userlink':
			include 'page-socialbookmark.tpl.php';
			break;
		case $node->type == 'chatroom':
			include 'page-chatroom.tpl.php';		
			break;
		case $node->type == 'forum':
			include 'page-forum.tpl.php';
			break;
		default:
			include 'page-default.tpl.php';
	}
?>