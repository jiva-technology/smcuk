<?php
// $Id: nodereview_node_nodereview.inc,v 1.6 2007/02/25 02:50:10 crell Exp $

/* ----- Node hooks ----- */

/**
 * Implementation of hook_access().
 *
 */
function nodereview_access($op, $node) {
  global $user;

  if ($op == 'create') {
    // Only users with permission to do so may create this node type.
    return user_access('submit reviews');

  }

  // Users who create a node may edit or delete it later, assuming they have the
  // necessary permissions.
  if ($op == 'update' || $op == 'delete') {
    if (user_access('administer reviews') || (user_access('edit own reviews') && ($user->uid == $node->uid))) {
      return TRUE;
    }
  }

  if ($op == 'view') {
    return user_access('read reviews');
  }
}


/**
 * Implementation of hook_form().
 *
 */
function nodereview_form(&$node) {

  // This is for the LOCAL_TASK version, node/nid/addreview
  if (! $node->reviewed_nid) $node->reviewed_nid = arg(1);

  $node_info = node_get_types('type', 'nodereview');

  $form['reviewed_nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->reviewed_nid,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t($node_info->title_label),
    '#default_value' => $node->title,
    '#required' => TRUE
  );

  $reviewed_node = node_load(array('nid'=>$node->reviewed_nid));

  $votes = array();
  $votes_obj = votingapi_get_user_votes('node', $node->reviewed_nid, $node->uid);
  foreach ($votes_obj as $vote) {
    $votes[$vote->tag] = $vote->value;
  }

  $form['reviews']['#tree'] = TRUE;
  $form['reviews']['#weight'] = 0;

  $result = db_query("SELECT * FROM {nodereview_axes} WHERE node_type ='%s' ORDER BY weight, tag", $reviewed_node->type);

  while ($record = db_fetch_object($result)) {
    _nodereview_form_review($form, $record, $node->reviews, $votes);
  }

  // We'll use a single filter format for all textareas on the page
  $form['filter'] = filter_form($node->format);

  return $form;
}

/**
 * Add the review axis to the node edit form
 * 
 * @param $form
 *  The form object for the node edit form
 * @param $axis
 *  An object representing the axis for which to display the form component
 * @param $reviews
 *  An array of review objects from this node
 * @param $votes
 *  An array of votes (scores) for this review, keyed by axis name
 */
function _nodereview_form_review(&$form, $axis, $reviews, $votes) {
  
  static $options;
  if (!isset($options)) {
    $options = array(
      10 => 1,
      20 => 2,
      30 => 3,
      40 => 4,
      50 => 5,
      60 => 6,
      70 => 7, 
      80 => 8,
      90 => 9,
      100 => 10,
    );
  }
  $form['reviews'][$axis->aid] = array(
    '#type' => 'fieldset',
    '#title' => $axis->tag,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ); 
  $form['reviews'][$axis->aid]['score'] = array(
    '#type' => 'select',
    '#title' => t('Score'),
    '#options' => $options,
    '#default_value' => $votes[$axis->tag] ? $votes[$axis->tag] : 50,
    '#description' => t('Score for this axis'),
    '#required' => TRUE,
  );
  $form['reviews'][$axis->aid]['review'] = array(
    '#type' => 'textarea',
    '#title' => t('Review'),
    '#default_value' => $reviews[$axis->aid]->review,
    '#required' => TRUE,
  );
 
}


/**
 * Implementation of hook_validate().
 *
 */
function nodereview_validate(&$node) {

  if (! $node->reviewed_nid || ! db_result(db_query("SELECT Count(*) FROM {node} WHERE nid=%d", $node->reviewed_nid))) {
    // There is no such node, so error
    form_set_error('reviewed_nid', t('The node you are trying to review does not exist.'));
  }
  foreach ($node->reviews as $aid => $review) {
    if (! $review['score']) {
      form_set_error('', t('You must enter a score for every axis.'));
    }
    if (! $review['review']) {
      form_set_error('', t('You must enter a review for every axis.'));
    }
  }
}

/**
 * Implementation of hook_load().
 *
 */
function nodereview_load($node) {

  // Get the basic information from the nodereview node table
  $additions = db_fetch_object(db_query('SELECT reviewed_nid FROM {nodereview} WHERE nid = %d', $node->nid));

  // And now get the actual review info.  The numeric scores are stored with voteapi,
  // while textual reviews are in our own auxiliary table.  We need to merge them properly

  $votes_obj = votingapi_get_user_votes('node', $additions->reviewed_nid, $node->uid);
  $votes = array();
  foreach ($votes_obj as $vote) {
    $votes[$vote->tag] = $vote;
  }

  $additions->reviews = array();
  $result = db_query('SELECT nr.aid, tag, review FROM {nodereview_reviews} nr INNER JOIN {nodereview_axes} na ON nr.aid=na.aid WHERE nid = %d ORDER BY na.weight, na.tag', $node->nid);
  while ($record = db_fetch_object($result)) {
    // Add in the numeric scores
    $record->score = $votes[$record->tag]->value;
    $additions->reviews[$record->aid] = $record;
  }

  return $additions;
}


/**
 * Implementation of hook_insert().
 *
 */
function nodereview_insert($node) {

  db_query("INSERT INTO {nodereview} (nid, reviewed_nid) VALUES (%d, %d)", $node->nid, $node->reviewed_nid);

  nodereview_save_reviews($node);
}

/**
 * Implementation of hook_update().
 *
 */
function nodereview_update($node) {
  db_query("UPDATE {nodereview} SET reviewed_nid=%d WHERE nid = %d", $node->reviewed_nid, $node->nid);

  // Delete and rebuild the textual reviews, but the votingapi takes care of overwriting old values
  db_query("DELETE FROM {nodereview_reviews} WHERE nid=%d", $node->nid);
  nodereview_save_reviews($node);
}

function nodereview_save_reviews($node) {
  global $user;

  $votes = array();

  /*
  $result = db_query("SELECT na.aid, na.tag
                        FROM {nodereview_axes} na
                          INNER JOIN {node} n ON na.node_type=n.type
                        WHERE n.nid=%d", $node->reviewed_nid);
  while ($record = db_fetch_object($result)) {
    $axes[$record->aid] = $record->tag;
  }
  */

  $axes = nodereview_list_axes(db_result(db_query("SELECT type FROM {node} WHERE nid=%d", $node->reviewed_nid)));

  foreach ($node->reviews as $aid => $review) {
    // Save the text review
    db_query("INSERT INTO {nodereview_reviews} (nid, aid, review) VALUES (%d, %d, '%s')", $node->nid, $aid, $review['review']);

    // And use the votingapi to save the score
    $votes[] = (object)array('value'=>$review['score'], 'tag'=>$axes[$aid]);
  }

  votingapi_set_vote('node', $node->reviewed_nid, $votes, $node->uid);
}

/**
 * Implementation of hook_delete().
 *
 */
function nodereview_delete($node) {
  db_query('DELETE FROM {nodereview} WHERE nid = %d', $node->nid);
  db_query('DELETE FROM {nodereview_reviews} WHERE nid=%d', $node->nid);

  // Delete the votes in the votingapi associated with this node
  votingapi_delete_votes(votingapi_get_user_votes('node', $node->reviewed_nid, $node->uid));
  votingapi_recalculate_results('node', $node->reviewed_nid);
}


/**
 * Implementation of hook_view().
 *
 */
function nodereview_view($node, $teaser = FALSE, $page = FALSE) {
  //$node = node_prepare($node, $teaser);

  $aids = array_keys($node->reviews);
  $num_aids = sizeof($aids);

  // Previews are just plain ugly, because the $node object is not really a node
  // but a mutated $_POST.  It has to be handled completely differently.
  if ($node->in_preview) {

    // We have to load in our axis data, which the $_POST doesn't have but we need
    $result = db_query("SELECT tag, aid 
      FROM {nodereview_axes} na 
        INNER JOIN {node} n ON na.node_type=n.type
      WHERE n.nid=%d", $node->reviewed_nid);
    while ($record = db_fetch_array($result)) {
      $node->reviews[$record['aid']]['tag'] = $record['tag'];
    }

    foreach ($node->reviews as $review) {
      $node->content['reviews'][$review['tag']] = array(
        '#value' => theme('nodereview_review_preview', $review, $node),
      );
    }
    return $node;
  }
  else {
    $node = node_prepare($node, $teaser);

    // Do a markup check on the fields
    for ($i=0; $i < $num_aids; $i++) {
      $node->reviews[$aids[$i]]->review = check_markup($node->reviews[$aids[$i]]->review);
    }

    if ($teaser) {
      $node->content['reviews'] = array(
          '#value' => theme('nodereview_teaser', $node),
        );
    }

    if ($page) {
      foreach ($node->reviews as $review) {
        $node->content['reviews'][$review->tag] = array(
          '#value' => theme('nodereview_review_body', $review, $node),
        );
      }
    }

    return $node;
  }
}

/* ----- Theme functions ----- */

function theme_nodereview_review_body($review, $node) {
  $output = '';

  $title = $review->tag;
  $title .= ': ' . $review->score/10 . '/10';
  $output .= theme('box', $title, $review->review);

  return $output;
}

function theme_nodereview_review_preview($review, $node) {
  $output = '';

  $title = $review['tag'];
  $title .= ': ' . $review['score']/10 . '/10';
  $output .= theme('box', $title, check_markup($review['review'], $node->format));

  return $output;
}

function theme_nodereview_teaser($node) {
  $output = '';
  static $header;

  // Presumably teaser will be called multiple times on different nodes
  // on the same page, so static cache the static strings
  if (!isset($header)) {
    $header = array(t('axis'), t('score'), t('review'));
  }

  foreach ($node->reviews as $review) {
    $row = array();
    $row[] = $review->tag;
    $row[] = $review->score/10 . '/10';
    $review_text = truncate_utf8($review->review, 50, TRUE, TRUE);
    $row[] = substr($review_text, 0, strpos($review_text, "\n"));
    $rows[] = $row;
  }

  $output = theme('table', $header, $rows, array('class'=>'review-teaser'));
  return $output;
}
