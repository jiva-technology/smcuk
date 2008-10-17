<?php
/* $Id .... $ */
// $Id ....$
// $Id: coder_comment.inc,v 1.1.6.6 2008/02/28 11:50:29 douggreen Exp $
// $Id: coder_comment.inc,v 1.1.6.6 2008/02/28 11:50:29 douggreen Exp $ ok
/**
 * @file
 * This file implements tests for the Drupal commenting standards.
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder
 */

/**
* This is a comment error.
 *This is a comment error.
 */

/*
 * @see some_function()
 * @see some_function(), some-template.tpl.php
 * @see some_function(), some-template.tpl.php, ok.tpl.php
 *
 * sdfsf @see not ok
 * @see some_function, some-template.tpl.php, not_ok.tpl.php
 * @see some_function() some-template.tpl.php, not_ok.tpl.php
 * @see some_function(), some-template.tpl.php not_ok.tpl.php
 * @see some_function(), some-template.tpl.php, not_ok.tpl.php foo.php
 * @see some_function(),
 * @see some_function(), some-template.tpl.php, not_ok.tpl.php.
 */
