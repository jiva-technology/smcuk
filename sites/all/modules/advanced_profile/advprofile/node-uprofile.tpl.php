<?php
  // We don't ever want to go to the usernode itself. Always redirect to the user page.
  drupal_goto("user/$node->uid", NULL, NULL, 301)
?>