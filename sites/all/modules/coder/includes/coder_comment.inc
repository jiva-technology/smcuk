<?php
// $Id: coder_comment.inc,v 1.1.6.10 2008/03/01 14:54:24 douggreen Exp $

/**
 * @file
 * This include file implements coder functionality for comments.
 */

/**
 * Implementation of hook_reviews().
 */
function coder_comment_reviews() {
  $rules = array(
    array(
      '#type' => 'grep_invert',
      '#source' => 'comment',
      '#value' => '$Id',
      '#case-sensitive' => true,
      '#warning_callback' => '_coder_comment_Id_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^.*\$Id.*$',
      '#not' => '^\/\/\s+\$Id\$|^\/\/\s+\$Id'.':[^\$]+\$',
      '#case-sensitive' => true,
      '#warning_callback' => '_coder_comment_Id_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^\*',
      '#warning' => 'indent secondary line of comment one space ',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '^\s*\*.+|^\/\*.+',
      '#not' => '^\s*\*\s+|^\/\*\s+|^\/\*\*|^\s*\*\/',
      '#warning' => 'put a space between the asterisk and the comment text',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '.*\@see\s*',
      '#not' => '^\s*\*\s*\@see.*$',
      '#warning' => '@see should always be at the beginning of a line, never inline in other comments.',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*.*',
      '#not' => '^\@see\s*[a-zA-Z_\.\-]+\(\)',
      '#warning' => '@see should always be followed by a function name with ()',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*.*?[\.\,;]+\s*$',
      '#warning' => '@see should have no trailing punctuation',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*[a-zA-Z_\.]+\(\)(\s+[a-zA-Z_\.\-]+)+',
      '#warning' => '@see references should be separated from the function name by ", "',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'comment',
      '#value' => '\@see\s*[a-zA-Z_\.]+\(\),.*?(\s+[a-zA-Z_\.\-]+){2,}',
      '#warning' => '@see references should be separated by ", "',
      '#severity' => 'minor',
    ),
  );
  $review = array(
    '#title' => t('Drupal Commenting Standards'),
    '#link' => 'http://drupal.org/node/318',
    '#rules' => $rules,
    '#description' => t('every developer should use'),
  );
  return array('comment' => $review);
}

function _coder_comment_Id_warning() {
  return array(
    '#warning' => t('Include the CVS keyword $Id'.'$ in each file.  This should be in the format <code>// $Id'.'$</code> or <code>// $Id'.'$</code>'),
    '#link' => 'http://drupal.org/coding-standards',
  );
}
