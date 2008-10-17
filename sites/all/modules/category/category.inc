<?php
// $Id: category.inc,v 1.64.2.1 2007/02/02 08:24:56 jaza Exp $

/**
 * @file
 * API functions for the category system as provided by the category module.
 * This file is required in order for the category module to work.
 *
 * Other modules should only call functions in this file when interfacing
 * with the category system - functions in category.module itself should not
 * need to be called directly.
 */

/**
 * Retrieve the category object matching a category ID.
 *
 * @param $cid
 *   The node/category ID of the category to be retrieved.
 * @param $reset
 *   Whether or not to reset the cache of categories. Default is FALSE.
 *
 * @return
 *   Populated category object if the category is found, FALSE otherwise.
 */
function category_get_category($cid, $reset = FALSE) {
  if (!is_numeric($cid)) {
    return FALSE;
  }
  $query = 'SELECT c.*, n.title FROM {category} c INNER JOIN {node} n ON c.cid = n.nid';

  return category_get_cached_item('get_category', $cid, 'cid', $query, 'c.cid = %d', FALSE, $reset);
}

/**
 * Try to map a string to an existing category, as for glossary use.
 *
 * Provides a case-insensitive and trimmed mapping, to maximize the
 * likelihood of a successful match.
 *
 * @param title
 *   Title of the category to search for.
 *
 * @return
 *   An array of matching category objects.
 */
function category_get_category_by_name($title) {
  $db_result = db_query("SELECT c.*, n.title FROM {category} c INNER JOIN {node} n ON c.cid = n.nid WHERE LOWER('%s') LIKE LOWER(n.title)", trim($title));
  $result = array();
  while ($category = db_fetch_object($db_result)) {
    $result[] = $category;
  }

  return $result;
}

/**
 * Return the container object matching a container ID.
 */
function category_get_container($cnid) {
  static $containers = array();

  if (!array_key_exists($cnid, $containers)) {
    $result = db_query('SELECT cn.*, n.title, ct.type, cd.allowed_parent, c.description AS cont_description, c.weight AS cont_weight, c.depth AS cont_depth FROM {category} c INNER JOIN {category_cont} cn ON c.cid = cn.cid INNER JOIN {node} n ON c.cid = n.nid LEFT JOIN {category_cont_node_types} ct ON c.cid = ct.cid LEFT JOIN {category_cont_distant} cd ON c.cid = cd.cid WHERE c.cid = %d ORDER BY c.weight, n.title', $cnid);
    $node_types = array();
    $node_distant = array();
    while ($cont = db_fetch_object($result)) {
      if (isset($cont->type)) {
        $node_types[] = $cont->type;
        $cont->nodes = $node_types;
      }
      unset($cont->type);
      if (isset($cont->allowed_parent)) {
        $node_distant[] = $cont->allowed_parent;
        $cont->children_allowed_parents = $node_distant;
      }
      if (!isset($cont->children_allowed_parents)) {
        $cont->children_allowed_parents = array('0');
      }
      unset($cont->allowed_parent);
      $cont->children_have_distant = !in_array('0', $cont->children_allowed_parents);
      if (empty($cont->admin_title)) {
        $cont->admin_title = $cont->title;
        $cont->has_admin_title = FALSE;
      }
      else {
        $cont->has_admin_title = TRUE;
      }
      $containers[$cnid] = $cont;
    }
  }

  return (isset($containers[$cnid]) ? $containers[$cnid] : NULL);
}

/**
 * Return an array of all container objects.
 *
 * @param $type
 *   If set, return only those containers associated with this node type.
 */
function category_get_containers($type = NULL) {
  $type_sql = $type ? " WHERE nt.type = '%s'" : "";
  $result = db_query("SELECT c.*, n.title, n.status, nt.type, cd.allowed_parent FROM {category_cont} c INNER JOIN {category} t ON c.cid = t.cid INNER JOIN {node} n ON c.cid = n.nid LEFT JOIN {category_cont_node_types} nt ON c.cid = nt.cid LEFT JOIN {category_cont_distant} cd ON c.cid = cd.cid". $type_sql ." ORDER BY t.weight, n.title", $type);

  $containers = array();
  $node_types = array();
  $node_distant = array();
  while ($cont = db_fetch_object($result)) {
    if (isset($cont->type)) {
      $node_types[$cont->cid][$cont->type] = $cont->type;
      unset($cont->type);
      $cont->nodes = $node_types[$cont->cid];
    }
    if (isset($cont->allowed_parent)) {
      $node_distant[$cont->cid][$cont->allowed_parent] = $cont->allowed_parent;
      unset($cont->allowed_parent);
      $cont->children_allowed_parents = $node_distant[$cont->cid];
    }
    if (!isset($cont->children_allowed_parents)) {
      $cont->children_allowed_parents = array('0');
    }
    $cont->children_have_distant = (count($cont->children_allowed_parents) > 1 || !in_array('0', $cont->children_allowed_parents));
    if (empty($cont->admin_title)) {
      $cont->admin_title = $cont->title;
      $cont->has_admin_title = FALSE;
    }
    else {
      $cont->has_admin_title = TRUE;
    }
    $containers[$cont->cid] = $cont;
  }

  return $containers;
}

/**
 * Determine if the specified node is a category or a container.
 *
 * @param $nid
 *   The node ID of the node to be checked.
 *
 * @return
 *   Boolean TRUE if the node is a category or a container, FALSE otherwise.
 */
function category_is_cat_or_cont($nid, $reset = FALSE) {
  if (!is_numeric($nid)) {
    return FALSE;
  }

  $query = 'SELECT n.nid FROM {node} n INNER JOIN {category} c ON n.nid = c.cid';
  $return = category_get_cached_item('is_cat_or_cont', $nid, 'nid', $query, 'n.nid = %d', FALSE, $reset);
  return !empty($return->nid);
}

/**
 * Given a new or existing node, and a type of either 'category_cat' or
 * 'category_cont' (or undefined), determine if a category node is a category
 * or a container, and return its type. This function assumes that the
 * node is either a category or a container - it will not return the correct
 * value if the node is neither (this is by design).
 *
 * The value of $type does not have to be the same as the value of $node->type,
 * since outlined categories can be of a different type.
 *
 * @param $node
 *   The node being tested. Does not have to be fully populated.
 * @param $type
 *   The type of category being used. Either 'category_cat', 'category_cont',
 *   or an empty value (such as NULL).
 *
 * @return
 *   Either 'category_cat' or 'category_cont' (never ANYTHING else).
 */
function category_node_get_type($node, $type = NULL) {
  if (empty($type)) {
    $type = $node->type;
  }
  if (empty($type) || !($type == 'category_cat' || $type == 'category_cont')) {
    $category = category_get_category($node->nid);
    if ($category->cnid) {
      $type = 'category_cat';
    }
    else {
      $type = 'category_cont';
    }
  }

  return $type;
}

/**
 * Find all parents of a given category ID.
 * This function will return the parents in an indexed array sorted by the parents' weight and title.
 */
function category_get_parents($cid, $key = 'cid', $distant = TRUE, $reset = FALSE) {
  static $parents, $distant_parents;

  if (!$cid) {
    return array();
  }

  if (($distant && !isset($distant_parents)) || (!$distant && !isset($parents)) || $reset) {
    $distant_sql = $distant ? '' : ' AND (c.cnid = cc.cnid OR c.cnid = 0)';
    $result = db_query(db_rewrite_sql('SELECT c.*, h.cid AS child, n.title, cn.admin_title FROM {category} c INNER JOIN {category_hierarchy} h ON c.cid = h.parent INNER JOIN {node} n ON c.cid = n.nid LEFT JOIN {category_cont} cn ON c.cid = cn.cid INNER JOIN {category} cc ON h.cid = cc.cid WHERE n.status = 1'. $distant_sql .' ORDER BY c.weight, n.title'), $cid);
    if ($distant) {
      $distant_parents = array();
    }
    else {
      $parents = array();
    }
    while ($parent = db_fetch_object($result)) {
      if (empty($parent->admin_title)) {
        $parent->admin_title = $parent->title;
      }
      if ($distant) {
        $distant_parents[$parent->child][] = $parent;
      }
      else {
        $parents[$parent->child][] = $parent;
      }
    }
  }

  return $distant ? (isset($distant_parents[$cid]) ? $distant_parents[$cid] : array()) : (isset($parents[$cid]) ? $parents[$cid] : array());
}

/**
 * Find all ancestors of a given category ID.
 */
function category_get_parents_all($cid, $distant = FALSE) {
  $parents = array();
  if ($cid) {
    $parents[] = category_get_category($cid);
    $n = 0;
    while ($parent = category_get_parents($parents[$n]->cid, 'cid', $distant)) {
      $parents = array_merge($parents, $parent);
      $n++;
    }
  }
  return $parents;
}

/**
 * Find all children of a category ID.
 */
function category_get_children($cid, $cnid = 0, $key = 'cid', $reset = FALSE) {
  static $children;

  if ($cnid) {
    $result = db_query(db_rewrite_sql('SELECT c.*, n.title FROM {category} c INNER JOIN {category_hierarchy} h ON c.cid = h.cid INNER JOIN {node} n ON c.cid = n.nid WHERE h.parent = %d AND n.status = 1 AND c.cnid = %d ORDER BY c.weight, n.title'), $cid, $cnid);

    $results = array();
    while ($category = db_fetch_object($result)) {
      $results[$category->$key] = $category;
    }

    return $results;
  }
  elseif (!isset($children) || $reset) {
    $result = db_query(db_rewrite_sql('SELECT c.*, h.parent, n.title FROM {category} c INNER JOIN {category_hierarchy} h ON c.cid = h.cid INNER JOIN {node} n ON c.cid = n.nid WHERE n.status = 1 ORDER BY c.weight, n.title'), $cid);

    $children = array();
    while ($category = db_fetch_object($result)) {
      $children[$category->parent][$category->$key] = $category;
    }
  }

  return isset($children[$cid]) ? $children[$cid] : array();
}

/**
 * Given a node, this function returns an array of 'category' objects
 * representing the path in the category tree from the root to the
 * parent of the given node.
 *
 * @param $node
 *   A category node object for which to compute the path.
 * @param $hidden
 *   Boolean indicating whether or not to include hidden containers in the
 *   returned array (defaults to FALSE).
 * @param $nodes
 *   The array that gets recursively built up and then returned (internal use
 *   only).
 *
 * @return
 *   an array of category node objects representing the path of
 *   nodes root to parent of the given node. Returns an empty array if
 *   the node does not exist or is not part of a category hierarchy.
 *
 */
function category_location($node, $hidden = FALSE, $nodes = array()) {
  $parent = category_get_category($node->parent);
  if ($parent->title) {
    if (!isset($parent->nid)) {
      $parent->nid = $parent->cid;
      unset($parent->cid);
    }
    $parents = category_get_parents($parent->nid, 'cid', TRUE);
    $parent->parent = reset($parents);
    $parent->parent = isset($parent->parent->cid) ? $parent->parent->cid : 0;
    $nodes = category_location($parent, $hidden, $nodes);

    $is_hidden = FALSE;
    if (!$hidden) {
      $is_cat = ($parent->cnid != 0);
      if (!$is_cat) {
        $cont = category_get_container($parent->nid);
        $is_hidden = $cont->hidden_cont;
      }
    }
    if (!$is_hidden) {
      $nodes[] = $parent;
    }
  }
  return $nodes;
}

/**
 * Accumulates the nodes up to the root of the category tree from the given
 * node.
 *
 * @param $node
 *   A category node object for which to compute the path.
 * @param $hidden
 *   Boolean indicating whether or not to include hidden containers in the
 *   returned array (defaults to FALSE).
 * @param $nodes
 *   The array that gets recursively built up and then returned (internal use
 *   only).
 */
function category_location_down($node, $hidden = FALSE, $nodes = array()) {
  $children = category_get_children($node->nid);
  if (!empty($children)) {
    $last_direct_child = end($children);
    $last_direct_child->nid = $last_direct_child->cid;
    unset($last_direct_child->cid);
    if (!empty($last_direct_child)) {
      $is_hidden = FALSE;
      if (!$hidden) {
        $is_cat = ($last_direct_child->cnid != 0);
        if (!$is_cat) {
          $cont = category_get_container($last_direct_child->nid);
          $is_hidden = $cont->hidden_cont;
        }
      }
      if (!$is_hidden) {
        $nodes[] = $last_direct_child;
      }
      $nodes = category_location_down($last_direct_child, $hidden, $nodes);
    }
  }
  return $nodes;
}

/**
 * Create a hierarchical representation of a set of categories.
 *
 * @param $cnid
 *   Which container to generate the tree for.
 *
 * @param $parent
 *   The node ID under which to generate the tree. If 0, generate the tree
 *   for everything under the container.
 *
 * @param $depth
 *   Internal use only.
 *
 * @param $max_depth
 *   The number of levels of the tree to return. Leave NULL to return all levels.
 *
 * @param $distant
 *   Whether other containers or categories under them should be returned as
 *   part of the tree. Default is false.
 *
 * @return
 *   An array of all category objects in the tree. Each category object is extended
 *   to have "depth" and "parents" attributes in addition to its normal ones.
 */
function category_get_tree($cnid, $parent = NULL, $depth = -1, $max_depth = NULL, $distant = FALSE) {
  static $children, $parents, $categories;
  $tree = array();

  if (empty($cnid) && $depth == -1) {
    $distant = TRUE;
  }
  if (!isset($parent)) {
    $parent = $cnid;
  }

  $depth++;

  // We cache trees, so it's not CPU-intensive to call get_tree() on a category
  // and its children, too.
  if (!isset($children[$cnid])) {
    $distant_sql = ($distant) ? '' : 'AND c.cnid = %d ';

    $children[$cnid] = array();
    $result = db_query(db_rewrite_sql('SELECT c.cid, c.*, h.parent, n.title, cn.admin_title FROM {category} c INNER JOIN {category_hierarchy} h ON c.cid = h.cid INNER JOIN {node} n ON c.cid = n.nid LEFT JOIN {category_cont} cn ON c.cid = cn.cid LEFT JOIN {category} c2 ON h.parent = c2.cid WHERE (c2.cid IS NOT NULL OR h.parent = 0) AND n.status = 1 '. $distant_sql .'ORDER BY c.weight, n.title', 'n', 'nid'), $cnid);

    while ($category = db_fetch_object($result)) {
      if (!$distant && $depth == 0) {
        $parent_cat = category_get_category($category->parent);
        if ($parent_cat->cid != $cnid && $parent_cat->cnid != $cnid) {
          $category->parent = $cnid;
        }
      }

      $children[$cnid][$category->parent][] = $category->cid;
      $categories[$cnid][$category->cid] = $category;
      $parents[$cnid][$category->cid][] = $category->parent;
    }
  }

  $max_depth = (is_null($max_depth)) ? count($children[$cnid]) : $max_depth;
  if ($children[$cnid][$parent]) {
    foreach ($children[$cnid][$parent] as $child) {
      if ($max_depth > $depth) {
        $cat = drupal_clone($categories[$cnid][$child]);
        $cat->depth = $depth;
        // The "parent" attribute is not useful, as it would show one parent only.
        unset($cat->parent);
        $cat->parents = $parents[$cnid][$child];

        if (!$cat->cnid && empty($cat->admin_title)) {
          $cat->admin_title = $cat->title;
        }
        $tree[] = $cat;

        if ($children[$cnid][$child]) {
          $tree = array_merge($tree, category_get_tree($cnid, $child, $depth, $max_depth, $distant));
        }
      }
    }
  }

  return $tree;
}

/**
 * Flatten the output from category_get_tree() into a linear list of all
 * containers and categories in the site. Only the first occurrence of a
 * category in the tree is reflected in this list. Iterating through this
 * list from beginning to end is equivalent to performing a pre-order traversal
 * of the site's category hierarchy.
 *
 * @param $reset
 *   Whether or not to flush this function's cached list. Default is FALSE.
 *
 * @return
 *   A linear array containing every category and container on the site, where
 *   each item occurs only once.
 */
function category_get_trees_flat($reset = FALSE) {
  static $list;

  if (!isset($list) || $reset) {
    $list = array();
    $tree = category_get_tree(0);

    foreach ($tree as $category) {
      $list[$category->cid] = $category;
    }
  }

  return $list;
}

/**
 * Fetches the node object of the previous category or container in the site's
 * hierarchy.
 */
function category_prev($cid) {
  $list = category_get_trees_flat();

  reset($list);
  while (current($list) !== FALSE) {
    if (key($list) == $cid) {
      break;
    }
    next($list);
  }

  return prev($list);
}

/**
 * Fetches the node object of the next category or container in the site's
 * hierarchy.
 */
function category_next($cid) {
  $list = category_get_trees_flat();

  reset($list);
  while (current($list) !== FALSE) {
    if (key($list) == $cid) {
      break;
    }
    next($list);
  }

  return next($list);
}

/**
 * Traverses the category tree.  Applies the $visit_pre() callback to each
 * node, is called recursively for each child of the node (in weight,
 * title order).  Finally appends the output of the $visit_post()
 * callback to the output before returning the generated output.
 *
 * @param nid
 *  - the node id (nid) of the root node of the category hierarchy.
 * @param depth
 *  - the depth of the given node in the category hierarchy.
 * @param visit_pre
 *  - a function callback to be called upon visiting a node in the tree
 * @param visit_post
 *  - a function callback to be called after visiting a node in the tree,
 *    but before recursively visiting children.
 * @return
 *  - the output generated in visiting each node
 */
function category_recurse($cid = 0, $depth = 1, $visit_pre, $visit_post, $export_type = NULL) {
  $result = db_query(db_rewrite_sql('SELECT n.nid, n.title, c.weight FROM {node} n INNER JOIN {category} c ON n.nid = c.cid WHERE n.status = 1 AND n.nid = %d ORDER BY c.weight, n.title'), $cid);
  while ($page = db_fetch_object($result)) {
    // Load the node:
    $node = node_load($page->nid);
    $type = category_node_get_type($node);
    $is_cat = $type == 'category_cat';

    if ($node) {
      if (!$is_cat && $node->hidden_cont) {
        $children = db_query(db_rewrite_sql('SELECT n.nid, n.title, c.weight FROM {node} n INNER JOIN {category_hierarchy} h ON n.nid = h.cid INNER JOIN {category} c ON n.nid = c.cid INNER JOIN {node} n2 ON h.parent = n2.nid INNER JOIN {category} c2 ON h.parent = c2.cid WHERE n.status = 1 AND h.parent = %d ORDER BY c.weight, n.title, c2.weight, n2.title'), $node->nid);
        $cids = array();
        while ($childpage = db_fetch_object($children)) {
          if (!isset($cids[$childpage->nid])) {
            $childnode = node_load($childpage->nid);
            if ($childnode->nid != $node->nid) {
              $output .= category_recurse($childnode->nid, $depth, $visit_pre, $visit_post);
            }
            $cids[$childpage->nid] = TRUE;
          }
        }
      }
      else {
        if (!isset($export_type) || (isset($node->export_types) && in_array($export_type, $node->export_types))) {
          if (function_exists($visit_pre)) {
            $output .= call_user_func($visit_pre, $node, $depth, $cid);
          }
          else {
            $output .= category_node_visitor_html_pre($node, $depth, $cid);
          }

          $children = db_query(db_rewrite_sql('SELECT n.nid, n.title, c.weight FROM {node} n INNER JOIN {category_hierarchy} h ON n.nid = h.cid INNER JOIN {category} c ON n.nid = c.cid INNER JOIN {node} n2 ON h.parent = n2.nid INNER JOIN {category} c2 ON h.parent = c2.cid WHERE n.status = 1 AND h.parent = %d ORDER BY c.weight, n.title, c2.weight, n2.title'), $node->nid);
          $cids = array();
          while ($childpage = db_fetch_object($children)) {
            if (!isset($cids[$childpage->nid])) {
              $childnode = node_load($childpage->nid);
              if ($childnode->nid != $node->nid) {
                $output .= category_recurse($childnode->nid, $depth + 1, $visit_pre, $visit_post);
              }
              $cids[$childpage->nid] = TRUE;
            }
          }
          if (function_exists($visit_post)) {
            $output .= call_user_func($visit_post, $node, $depth);
          }
          else {
            # default
            $output .= category_node_visitor_html_post($node, $depth);
          }
        }
      }
    }
  }

  return $output;
}

/**
 * Implementation of hook_node_type().
 */
function category_node_type($op, $info) {
  if ($op == 'update' && !empty($info->old_type) && $info->type != $info->old_type) {
    db_query("UPDATE {category_cont_node_types} SET type = '%s' WHERE type = '%s'", $info->type, $info->old_type);
  }
  elseif ($op == 'delete') {
    db_query("DELETE FROM {category_cont_node_types} WHERE type = '%s'", $info->type);
  }
}

/**
 * Find all category objects related to a given category ID.
 */
function category_get_related($cid, $key = 'cid') {
  if ($cid) {
    $result = db_query(db_rewrite_sql('SELECT c.*, n.title, cid1, cid2 FROM {category_relation}, {category} c, {node} n WHERE (c.cid = cid1 OR c.cid = cid2) AND (cid1 = %d OR cid2 = %d) AND c.cid = n.nid AND c.cid != %d AND n.status = 1 ORDER BY c.weight, n.title'), $cid, $cid, $cid);
    $related = array();
    while ($category = db_fetch_object($result)) {
      $related[$category->$key] = $category;
    }
    return $related;
  }
  else {
    return array();
  }
}

/**
 * Return an array of synonyms of the given category ID.
 */
function category_get_synonyms($cid) {
  if ($cid) {
    $result = db_query('SELECT name FROM {category_synonym} WHERE cid = %d', $cid);
    while ($synonym = db_fetch_array($result)) {
      $synonyms[] = $synonym['name'];
    }
    return $synonyms ? $synonyms : array();
  }
  else {
    return array();
  }
}

/**
 * Return the category object that has the same given string as a synonym.
 */
function category_get_synonym_root($synonym) {
  return db_fetch_object(db_query("SELECT c.*, n.title FROM {category} c INNER JOIN {category_synonym} s ON c.cid = s.cid AND s.name = '%s' INNER JOIN {node} n ON c.cid = n.nid", $synonym));
}

function category_del_category_recursive($cid) {
  $cids = array($cid);
  while ($cids) {
    $children_cids = $orphans = array();
    foreach ($cids as $cat) {
      // See if any of the category's children are about to be become orphans:
      if ($children = category_get_children($cat, 0, 'cid', TRUE)) {
        foreach ($children as $child) {
          // If the category has multiple parents, we don't delete it.
          $parents = category_get_parents($child->cid, 'cid', TRUE, TRUE);
          if (count($parents) == 1) {
            $orphans[] = $child->cid;
          }
        }
      }

      node_delete($cat);
    }

    $cids = $orphans;
  }

  cache_clear_all();
}

/**
 * Deletes a container and all its categories. If the container or any of its
 * categories has distant children, then those children are modified to no
 * longer have the relevant distant parent.
 *
 * @param $cnid
 *   The container to be deleted (along with all its categories).
 */
function category_del_container($cnid) {
  $cont = new stdClass();
  $cont->cid = $cnid;

  $tree = category_get_tree($cnid);
  $tree += array('cont' => $cont);
  $deleted = array();
  foreach ($tree as $category) {
    if (!isset($deleted[$category->cid])) {
      if ($children = category_get_children($category->cid, 0, 'cid', TRUE)) {
        foreach ($children as $child) {
          if ($child->cnid != 0 && $child->cnid != $cnid) {
            $node = node_load($child->cid);
            if (isset($node->parents) && is_array($node->parents)) {
              foreach ($node->parents as $key => $parent) {
                if ($parent == $category->cid) {
                  unset($node->parents[$key]);
                }
              }
              if (empty($node->parents)) {
                $node->parents[0] = $node->cnid;
              }
            }
            node_save($node);
          }
        }
      }

      node_delete($category->cid);
      $deleted[$category->cid] = TRUE;
    }
  }

  cache_clear_all();
}

/**
 * Save category associations for a given node.
 */
function category_node_save(&$node, $is_legacy = FALSE) {
  global $user;
  static $legacy_calls = array();

  if (!$is_legacy || !isset($legacy_calls[$node->nid])) {
    $cats_flat = array();
    category_node_delete($node->nid);

    // Free tagging containers do not send their cids in the form,
    // so we'll detect them here and process them independently.
    if (isset($node->category['tags'])) {
      $typed_input = $node->category['tags'];

      foreach ($typed_input as $cnid => $cnid_value) {
        // This regexp allows the following types of user input:
        // this, "somecmpany, llc", "and ""this"" w,o.rks", foo bar
        $regexp = '%(?:^|,\ *)("(?>[^"]*)(?>""[^"]* )*"|(?: [^",]*))%x';
        preg_match_all($regexp, $cnid_value, $matches);
        $typed_cats = $matches[1];

        foreach ($typed_cats as $typed_cat) {
          // If a user has escaped a term (to demonstrate that it is a group,
          // or includes a comma or quote character), we remove the escape
          // formatting so to save the term into the DB as the user intends.
          $typed_cat = str_replace('""', '"', preg_replace('/^"(.*)"$/', '\1', $typed_cat));
          $typed_cat = trim($typed_cat);
          if ($typed_cat == "") { continue; }

          // See if the term exists in the chosen container
          // and return the cid, otherwise, add a new record.
          $possibilities = category_get_category_by_name($typed_cat);
          $typed_cat_cid = NULL; // cid match if any.
          foreach ($possibilities as $possibility) {
            if ($possibility->cnid == $cnid) {
              $typed_cat_cid = $possibility->cid;
            }
          }

          if (!$typed_cat_cid) {
            $tag_node = new stdClass();
            $tag_node->title = $typed_cat;
            $tag_node->cnid = $cnid;
            $tag_node->type = 'category_cat';
            $tag_node->parents[0] = $tag_node->parent = $cnid;
            $node_options = variable_get('node_options_'. $tag_node->type, array('status', 'promote'));
            $tag_node->status = in_array('status', $node_options);
            $tag_node->promote = in_array('promote', $node_options);
            $tag_node->sticky = in_array('sticky', $node_options);
            $tag_node->revision = in_array('revision', $node_options);
            $tag_node->name = $user->name ? $user->name : 0;
            $tag_node->date = date('j M Y H:i:s');
            $tag_node = node_submit($tag_node);
            node_save($tag_node);
            $typed_cat_cid = $tag_node->nid;
          }

          if (!isset($cats_flat[$typed_cat_cid])) {
            db_query('INSERT INTO {category_node} (nid, cid) VALUES (%d, %d)', $node->nid, $typed_cat_cid);
            $cats_flat[$typed_cat_cid] = $typed_cat_cid;
          }
        }
      }
    }
    else if (isset($node->category['legacy'])) {
      foreach ($node->category['legacy'] as $cid) {
        if (!empty($cid)) {
          db_query('INSERT INTO {category_node} (nid, cid) VALUES (%d, %d)', $cid, $node->nid);
          $cats_flat[$node->nid] = $node->nid;
        }
      }
    }
    if (is_array($node->category)) {
      foreach ($node->category as $key => $cat) {
        if ($key != 'tags' && $key != 'legacy') {
          if (is_array($cat)) {
            foreach ($cat as $cid) {
              if (!empty($cid)) {
                db_query('INSERT INTO {category_node} (nid, cid) VALUES (%d, %d)', $node->nid, $cid);
                $cats_flat[$cid] = $cid;
              }
            }
          }
          else if (isset($cat->cid)) {
            db_query('INSERT INTO {category_node} (nid, cid) VALUES (%d, %d)', $node->nid, $cat->cid);
            $cats_flat[$cat->cid] = $cat->cid;
          }
          else if ($cat) {
            db_query('INSERT INTO {category_node} (nid, cid) VALUES (%d, %d)', $node->nid, $cat);
            $cats_flat[$cat] = $cat;
          }
        }
      }
    }

    $node->category = category_node_get_categories($node->nid, 'cid', TRUE);
    $legacy_calls[$node->nid] = TRUE;
  }
}

/**
 * Given a category id, count the number of published nodes in it.
 */
function category_category_count_nodes($cid, $type = 0) {
  static $count;

  if (!isset($count[$type])) {
    // $type == 0 always evaluates true if $type is a string
    if (is_numeric($type)) {
      $result = db_query(db_rewrite_sql('SELECT c.cid, COUNT(n.nid) AS cnt FROM {category_node} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.status = 1 GROUP BY c.cid'));
    }
    else {
      $result = db_query(db_rewrite_sql("SELECT c.cid, COUNT(n.nid) AS cnt FROM {category_node} c INNER JOIN {node} n ON c.nid = n.nid WHERE n.status = 1 AND n.type = '%s' GROUP BY c.cid"), $type);
    }
    while ($category = db_fetch_object($result)) {
      $count[$type][$category->cid] = $category->cnt;
    }
  }

  return isset($count[$type][$cid]) ? $count[$type][$cid] : 0;
}

/**
 * Find all categories associated to the given node, ordered by container and category weight.
 */
function category_node_get_categories($nid, $key = 'cid', $reset = FALSE) {
  static $categories = array();

  if (!isset($categories[$nid]) || $reset) {
    $categories[$nid] = array();

    $result = db_query(db_rewrite_sql('SELECT n.nid, r.nid AS node_id, c.*, n.title FROM {category} c INNER JOIN {category_node} r ON c.cid = r.cid INNER JOIN {category} cn ON c.cnid = cn.cid INNER JOIN {node} n ON c.cid = n.nid INNER JOIN {node} cnn ON cn.cid = cnn.nid WHERE n.status = 1 AND r.nid = %d ORDER BY cn.weight, cnn.title, c.weight, n.title'), $nid);

    while ($category = db_fetch_object($result)) {
      $categories[$nid][$category->$key] = $category;
    }
  }

  return $categories[$nid];
}

/**
 * Find all categories associated to the given node, within one container.
 */
function category_node_get_categories_by_container($nid, $cnid, $key = 'cid') {
  $result = db_query(db_rewrite_sql('SELECT c.*, n.title FROM {category} c INNER JOIN {category_node} r ON c.cid = r.cid INNER JOIN {node} n ON c.cid = n.nid WHERE c.cnid = %d AND r.nid = %d AND n.status = 1 ORDER BY c.weight'), $cnid, $nid);
  $categories = array();
  while ($category = db_fetch_object($result)) {
    $categories[$category->$key] = $category;
  }
  return $categories;
}

/**
 * Remove associations of a node to its categories.
 */
function category_node_delete($nid, $is_legacy = FALSE) {
  static $legacy_calls = array();

  if (!$is_legacy || !isset($legacy_calls[$nid])) {
    db_query('DELETE FROM {category_node} WHERE nid = %d', $nid);

    $legacy_calls[$nid] = TRUE;
  }
}

/**
 * Finds all nodes that match selected category conditions.
 *
 * @param $cids
 *   An array of category IDs to match.
 * @param $operator
 *   How to interpret multiple IDs in the array. Can be "or" or "and".
 * @param $depth
 *   How many levels deep to traverse the category tree. Can be a nonnegative
 *   integer or "all".
 * @param $pager
 *   Whether the nodes are to be used with a pager (the case on most Drupal
 *   pages) or not (in an XML feed, for example).
 * @param $distant
 *   Whether other containers or categories under them should be returned as
 *   part of the tree. Default is false.
 * @param $order
 *   The order clause for the query that retrieve the nodes.
 * @return
 *   A resource identifier pointing to the query results.
 */
function category_select_nodes($cids = array(), $operator = 'or', $depth = 0, $pager = TRUE, $distant = FALSE, $order = 'n.sticky DESC, n.created DESC') {
  if (count($cids) > 0) {
    // For each category ID, generate an array of descendant category IDs to the right depth.
    $descendant_cids = array();
    if ($depth === 'all' || $depth < 0) {
      $depth = NULL;
    }
    foreach ($cids as $index => $cid) {
      $category = category_get_category($cid);
      $cnid = $category->cnid ? $category->cnid : $cid;
      $tree = category_get_tree($cnid, $cid, -1, $depth, $distant);
      $descendant_cids[] = array_merge(array($cid), array_map('_category_get_cid_from_category', $tree));
    }

    $group_by = 'n.nid, n.sticky, n.title, n.created';
    if ($operator == 'or') {
      $str_cids = implode(',', call_user_func_array('array_merge', $descendant_cids));
      $sql = 'SELECT DISTINCT(n.nid), n.sticky, n.title, n.created FROM {node} n INNER JOIN {category_node} cn ON n.nid = cn.nid WHERE cn.cid IN ('. $str_cids .') AND n.status = 1 GROUP BY '. $group_by .' ORDER BY '. $order;
      $sql_count = 'SELECT COUNT(DISTINCT(n.nid)) FROM {node} n INNER JOIN {category_node} cn ON n.nid = cn.nid WHERE cn.cid IN ('. $str_cids .') AND n.status = 1';
    }
    else {
      $joins = '';
      $wheres = '';
      foreach ($descendant_cids as $index => $cids) {
        $joins .= ' INNER JOIN {category_node} cn'. $index .' ON n.nid = cn'. $index .'.nid';
        $wheres .= ' AND cn'. $index .'.cid IN ('. implode(',', $cids) .')';
      }
      $sql = 'SELECT n.nid, n.sticky, n.title, n.created FROM {node} n '. $joins .' WHERE n.status = 1 '. $wheres .' GROUP BY '. $group_by .' ORDER BY '. $order;
      $sql_count = 'SELECT COUNT(n.nid) FROM {node} n '. $joins .' WHERE n.status = 1 ' . $wheres;
    }
    $sql = db_rewrite_sql($sql);
    $sql_count = db_rewrite_sql($sql_count);
    if ($pager) {
      $result = pager_query($sql, variable_get('default_nodes_main', 10), 0, $sql_count);
    }
    else {
      $result = db_query_range($sql, 0, variable_get('feed_default_items', 10));
    }
  }

  return $result;
}

/**
 * Calls category_select_nodes() using $cid, and caches the results in a
 * static variable. This cached result can then be retrieved after changes
 * have been made to the database.
 *
 * @param $cid
 *   The category ID of the category whose results are to be gotten.
 *
 * @return
 *   The cached result specified by $cid.
 */
function category_select_nodes_cache($cid) {
  static $nodes;

  if (!is_numeric($cid)) {
    return FALSE;
  }

  // Simple cache to eliminate duplicate queries
  if (!isset($nodes)) {
    $nodes = array();
  }

  if (!isset($nodes[$cid])) {
    $nodes[$cid] = array();
    $result = category_select_nodes(array($cid));
    while ($node = db_fetch_object($result)) {
      $nodes[$cid][] = $node;
    }
  }

  return $nodes[$cid];
}

/**
 * Generate a set of options for selecting a category from all containers. Can be
 * passed to form_select.
 */
function category_form_all($free_tags = 0) {
  $containers = category_get_containers();
  $options = array();
  foreach ($containers as $cnid => $container) {
    if ($container->tags && !$free_tags) { continue; }
    if (empty($container->nodes)) { continue; }

    $tree = category_get_tree($cnid);
    $options[$container->admin_title] = array();
    if ($tree) {
      foreach ($tree as $category) {
        $options[$container->admin_title][$category->cid] = str_repeat('-', $category->depth) . $category->title;
      }
    }
  }
  return $options;
}

/**
 * Accepts the result of a pager_query() call, such as that performed by
 * category_select_nodes(), and formats each node along with a pager.
*/
function category_render_nodes($result) {
  $output = '';

  if (db_num_rows($result) > 0) {
    while ($node = db_fetch_object($result)) {
      $output .= node_view(node_load($node->nid), 1);
    }
    $output .= theme('pager', NULL, variable_get('default_nodes_main', 10), 0);
  }
  else {
    $output .= '<p>'. t('There are currently no posts in this category.') .'</p>';
  }

  return $output;
}

/**
 * Parses a comma or plus separated string of category IDs.
 *
 * @param $str_cids
 *   A string of category IDs, separated by plus or comma.
 *   comma (,) means AND
 *   plus (+) means OR
 *
 * @return an associative array with an operator key (either 'and'
 *   or 'or') and a cid key containing an array of the category IDs.
 */
function category_categories_parse_string($str_cids) {
  $categories = array();
  if (preg_match('/^([0-9]+[+ ])+[0-9]+$/', $str_cids)) {
    $categories['operator'] = 'or';
    // The '+' character in a query string may be parsed as ' '.
    $categories['cids'] = preg_split('/[+ ]/', $str_cids);
  }
  else if (preg_match('/^([0-9]+,)*[0-9]+$/', $str_cids)) {
    $categories['operator'] = 'and';
    $categories['cids'] = explode(',', $str_cids);
  }
  return $categories;
}

/**
 * Generates various representation of a cateogry page with
 * all descendants and prints the requested representation to output.
 *
 * The function delegates the generation of output to helper functions.
 * The function name is derived by prepending 'category_export_' to the
 * given output type.  So, e.g., a type of 'html' results in a call to
 * the function category_export_html().
 *
 * @param type
 *   - a string encoding the type of output requested.
 *       The following types are currently supported in book module
 *          html: HTML (printer friendly output)
 *       Other types are supported in contributed modules.
 * @param nid
 *   - an integer representing the node id (nid) of the node to export
 *
 */
function category_export($type = 'html', $nid = 0, $func_prefix = 'category_export_') {
  $type = drupal_strtolower($type);
  $depth = _category_get_depth($nid);
  $export_function = $func_prefix . $type;

  if (function_exists($export_function)) {
    print call_user_func($export_function, $nid, $depth);
  }
  else {
    drupal_set_message(t('Unknown export format'));
    drupal_not_found();
  }
}

/**
 * Defines the complete set of form fields that make up the form for adding or
 * editing a category or container.
 *
 * @param $is_cat
 *   Boolean that indicates whether or not to return the form fields for a
 *   category. If FALSE, returns the form fields for a container instead.
 *   Defaults to TRUE.
 * @param $node
 *   Optional node object. Should be provided if $set_required is TRUE. Default
 *   is NULL.
 * @param $core_node_fields
 *   Boolean that indicates if the 'core' node fields should also be returned
 *   (i.e. 'title', 'body', 'format'). Should be set to FALSE if the form will
 *   be embedded within another node form that already defines these elements.
 *   Defaults to TRUE.
 *
 * @return
 *   A nested array of form elements, suitable for being passed to
 *   drupal_get_form().
 */
function category_get_form($is_cat = TRUE, $node = NULL, $core_node_fields = TRUE) {
  $form = array();

  if (isset($node->title)) {
    category_submit($node);
  }

  if ($core_node_fields) {
    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => isset($node->title) ? $node->title : '',
      '#required' => TRUE,
      '#weight' => -5,
    );

    $body_text = '';
    if (isset($node->body)) {
      if (!(module_exists('category_display') && $node->body_is_desc)) {
        $body_text = $node->body;
      }
    }
    $form['body_filter']['body'] = array(
      '#type' => 'textarea',
      '#title' => t('Body'),
      '#default_value' => $body_text,
      '#rows' => 20,
      '#required' => FALSE,
    );

    if (isset($node->format)) {
      $form['body_filter']['format'] = filter_form($node->format);
    }
    else {
      $form['body_filter']['format'] = filter_form();
    }
  }

  $form['catinfo'] = array(
    '#type' => 'fieldset',
    '#title' => $is_cat ? t('Category information') : t('Container information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['catinfo']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $node->description ? $node->description : '',
    '#rows' => 8,
    '#description' => ($is_cat ? t('A description of the category. Generally seen by users as title text, when hovering over a link to the category.') : t('A description of the container; can be used by modules.')),
  );

  $activeselect = module_exists('activeselect');
  $activeselect_type = ($activeselect ? 'activeselect' : 'select');

  if ($is_cat) {
    $containers = array();

    if (isset($node->type)) {
      foreach (category_get_containers() as $key => $value) {
        if (_category_type_allowed_container($node->type, $key)) {
          $containers[$key] = $value->admin_title;
        }
      }
    }
    else {
      foreach (category_get_containers() as $key => $value) {
        $containers[$key] = $value->admin_title;
      }
    }

    $form['catinfo']['cnid'] = array(
      '#type' => $activeselect_type,
      '#title' => t('Container'),
      '#options' => $containers,
      '#description' => t('The container that this category belongs in.'),
      '#required' => TRUE,
    );
    if ($node->cnid) {
      $form['catinfo']['cnid']['#default_value'] = $node->cnid;
    }
    if ($activeselect) {
      $form['catinfo']['cnid']['#activeselect_path'] = 'category/activeselect';
      $form['catinfo']['cnid']['#activeselect_targets'] = 'parents';
      if ($node->nid || $node->parent_cont) {
        $form['catinfo']['cnid']['#activeselect_extra'] = ($node->nid ? $node->nid : '0'). ($node->parent_cont ? ','. $node->parent_cont : '');
      }
      else {
        $form['catinfo']['cnid']['#activeselect_extra'] = '0';
      }
    }
  }
  else {
    $form['catinfo']['help'] = array(
      '#type' => 'textfield',
      '#title' => t('Help text'),
      '#default_value' => $node->help ? $node->help : '',
      '#size' => 60,
      '#description' => t('Instructions to present to the user when choosing a category.'),
    );
    $form['catinfo']['admin_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Admin title'),
      '#default_value' => $node->has_admin_title ? $node->admin_title : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#description' => t('A title for this container that will be used within administrative interfaces, such as selection lists and tabular listings. If this is left blank, the actual title will be used instead.'),
    );
    $form['catinfo']['nodes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Types'),
      '#default_value' => $node->nodes ? $node->nodes : array(),
      '#options' => node_get_types('names'),
      '#description' => t('A list of node types you want to associate with categories in this container.'),
    );
    $form['catinfo']['hierarchy'] = array(
      '#type' => 'radios',
      '#title' => t('Hierarchy'),
      '#default_value' => isset($node->hierarchy) ? $node->hierarchy : 1,
      '#options' => array(0 => t('Disabled'), 1 => t('Single'), 2 => t('Multiple')),
      '#description' => t('Allows <a href="!help-url">a tree-like hierarchy</a> between categories within a container. Depending on your distant parent configuration, this may also allow a hierarchy between categories and containers regardless of their parent container. Be aware that the <em>multiple</em> option refers to multiple parents, as opposed to multiple levels of hierarchy.', array('!help-url' => url('admin/help/category', NULL, 'hierarchy'))),
      '#required' => TRUE,
    );
    $form['catinfo']['has_relations'] = array(
      '#type' => 'checkbox',
      '#title' => t('Related terms'),
      '#default_value' => isset($node->has_relations) ? $node->has_relations : 0,
      '#description' => t('Allows <a href="!help-url">relations</a> between arbitrary categories or containers.', array('!help-url' => url('admin/help/category'))),
    );
    $form['catinfo']['has_synonyms'] = array(
      '#type' => 'checkbox',
      '#title' => t('Synonyms'),
      '#default_value' => isset($node->has_synonyms) ? $node->has_synonyms : 0,
      '#description' => t('Allows <a href="!help-url">synonyms</a> for a category or container, such as misspellings and acronyms.', array('!help-url' => url('admin/help/category'))),
    );
    $form['catinfo']['tags'] = array(
      '#type' => 'checkbox',
      '#title' => t('Free tagging'),
      '#default_value' => isset($node->tags) ? $node->tags : 0,
      '#description' => t('Content is categorized by typing terms instead of choosing from a list.'),
    );
    $form['catinfo']['multiple'] = array(
      '#type' => 'checkbox',
      '#title' => t('Multiple select'),
      '#default_value' => isset($node->multiple) ? $node->multiple : 0,
      '#description' => t('Allows nodes to have more than one category from a particular container (always true for free tagging).'),
    );
    $form['catinfo']['required'] = array(
      '#type' => 'checkbox',
      '#title' => t('Required'),
      '#default_value' => isset($node->required) ? $node->required : 0,
      '#description' => t('If enabled, every node <strong>must</strong> have at least one category from within this container.'),
    );
    $form['catinfo']['hidden_cont'] = array(
      '#type' => 'radios',
      '#title' => t('Hidden container'),
      '#default_value' => isset($node->hidden_cont) ? $node->hidden_cont : 0,
      '#options' => array('1' => t('Enabled'), '0' => t('Disabled')),
      '#description' => t('Sets this container\'s \'hidden\' attribute. This attribute is not used by the core category module, but can affect how this container is treated by other modules. For example, hidden containers do not have menu items generated by category_menu, and are not shown in navigational elements by category_display. This hidden behaviour applies only to the container itself, not to its child categories or to their assigned nodes.'),
      '#required' => TRUE,
    );
  }

  $blank = '<'. t('root') .'>';
  $hierarchy = 1;
  $parents = array();
  $exclude = array();
  $default_parent = 0;

  if ($node->nid || $node->cnid) {
    $parents = category_get_parents($node->nid);
    for($i=0;$i<count($parents);$i++){
      $parents[$i] = $parents[$i]->cid;
    }

    $hierarchy = $is_cat ? (isset($node->hierarchy) ? $node->hierarchy : 1) : variable_get('category_distant_containers', 1);

    // This makes the parent element have all possible options on submit,
    // to prevent validation errors with activeselect.
    if (!$activeselect && $is_cat) {
      $default_parent = $node->cnid;
    }

    if (empty($parents) && $node->cnid) {
      $parents[] = $node->cnid;
    }
  }

  if ($node->nid) {
    $children = category_get_tree($node->cnid, $node->nid, -1, NULL, TRUE);

    // A category/container can't be the child of itself, nor of its children.
    foreach ($children as $child) {
      $exclude[] = $child->cid;
    }
    if ($node->nid) {
      $exclude[] = $node->nid;
    }
  }

  if ($hierarchy == 1 || $activeselect) {
    $form['catinfo']['parents'] = _category_category_select(t('Parent'), $parents, $default_parent, t('Parent category or container. Containers are marked with an asterisk (*).'), 0, $blank, $exclude);
  }
  elseif ($hierarchy == 2) {
    $form['catinfo']['parents'] = _category_category_select(t('Parents'), $parents, $default_parent, t('Parent categories or containers. Containers are marked with an asterisk (*).'), ($activeselect ? 0 : 1), $blank, $exclude);
  }

  else {
    $form['parents'] = array(
      '#type' => 'value',
      '#value' => $parents,
    );
  }

  if ($set_default) {
    $form['parent'] = array(
      '#type' => 'value',
      '#value' => $node->parent,
    );
  }

  $form['catinfo']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
      '#default_value' => isset($node->weight) ? $node->weight : 0,
    '#delta' => 10,
    '#description' => t('Pages at a given level are ordered first by weight and then by title.'),
  );
  $form['catinfo']['depth'] = array(
    '#type' => 'weight',
    '#title' => t('Depth'),
      '#default_value' => isset($node->depth) ? $node->depth : 0,
    '#delta' => 10,
    '#description' => t('The depth of child categories that a node listing reflects. Set this to a negative value to reflect an infinite depth. Although a container cannot be assigned nodes, it can still list nodes of child categories, if it has a depth that is not 0.'),
  );

  if ($is_cat) {
    if ($node->has_synonyms) {
      $form['catinfo']['synonyms'] = array(
        '#type' => 'textarea',
        '#title' => t('Synonyms'),
        '#default_value' => implode("\n", ($node->synonyms ? $node->synonyms : array())),
        '#cols' => 60,
        '#rows' => 5,
        '#description' => t('Synonyms of this category or container, one synonym per line.'),
      );
    }
    if ($node->has_relations) {
      $blank = '<'. t('none') .'>';
      $form['catinfo']['relations'] = _category_node_select(t('Related categories'), ($node->relations ? $node->relations : 0), $node->cnid, t('Other categories in this container that are related to this one.'), 1, $blank);
    }
  }

  if (!$is_cat) {
    $distant = array(
      '0' => '<'. t('this container'). '>'. ($set_default ? ' ('. check_plain($node->admin_title). ')' : ''),
      'root' => '<'. t('root'). '>',
    );
    foreach (category_get_containers() as $key => $value) {
      $distant[$key] = $value->admin_title;
      $distant[$key. '#'] = $value->admin_title. ' *';
    }
    if ($node->nid) {
      unset($distant[$node->nid]);
      unset($distant[$node->nid. '#']);
    }

    // Distant parent settings
    $form['distant'] = array(
      '#type' => 'fieldset',
      '#title' => t('Distant parents'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['distant']['children_allowed_parents'] = array(
      '#type' => 'select',
      '#title' => t('Allowed Parents'),
      '#default_value' => isset($node->children_allowed_parents) ? $node->children_allowed_parents : array('0'),
      '#options' => $distant,
      '#multiple' => TRUE,
      '#required' => TRUE,
      '#description' => t('Specifies the containers from which categories in this container can have parents. If you select containers other than this one, then categories in this container can have distant parents. It is recommended for security reasons that you only select this container (which is the default), unless you are certain that all users adding categories to this container are both experienced and trustworthy.')
    );

    // Allow other modules to add additional options to this form.
    $extra = module_invoke_all('category', 'form', $node);
    if (is_array($extra)) {
      foreach ($extra as $key => $value) {
        if (is_array($value)) {
          $form[$key] = $value;
        }
      }
    }
  }

  return $form;
}

/**
 * Determines whether or not the specified wrapper is enabled and has database
 * maintenance turned on.
 *
 * @param $type
 *   The wrapper to check. Either 'taxonomy' or 'book'. Default is 'taxonomy'.
 *
 * @return
 *   Boolean value indicating status of the specified wrapper.
 */
function category_get_wrapper_status($type = 'taxonomy', $check_db = FALSE) {
  static $taxonomy_status, $book_status;

  if (!isset($taxonomy_status)) {
    $taxonomy_status = module_exists('taxonomy') && !menu_get_item(NULL, 'admin/content/taxonomy');
    if ($check_db) {
      $taxonomy_status = $taxonomy_status && variable_get('taxonomy_maintain_db', 1);
    }
  }
  if (!isset($book_status)) {
    $book_status = module_exists('book') && !menu_get_item(NULL, 'admin/content/book');
    if ($check_db) {
      $book_status = $book_status && variable_get('book_maintain_db', 1);
    }
  }

  return ($type == 'taxonomy' ? $taxonomy_status : $book_status);
}

/**
 * Finds the containers that have the specified container as an allowed
 * distant parent.
 *
 * @param $cnid
 *   The ID of the container for which to find distant child containers.
 *
 * @return
 *   An array where the keys and the values of the elements are those of the
 *   distant child containers found. An empty array is returned if no
 *   containers are found.
 */
function category_get_distant_child_containers($cnid) {
  $containers = array();

  $result = db_query('SELECT cid FROM {category_cont_distant} WHERE allowed_parent = \'%s\'', $cnid);
  while ($cont = db_fetch_object($result)) {
    $containers[$cont->cid] = $cont->cid;
  }

  return $containers;
}

/**
 * Generic caching function for use by other category module functions. Uses an
 * 'overflow' strategy to try and perform more efficient querying.
 *
 * @param $type
 *   The type of cached query to run.
 * @param $id
 *   The ID of the cached value to find.
 * @param $id_key
 *   The field name of the ID value.
 * @param $query
 *   The query to use when getting values from the database.
 * @param $where
 *   The WHERE clause of the query.
 * @param $order_by
 *   The ORDER BY clause of the query. Defaults to FALSE.
 * @param $reset
 *   Whether or not to reset the cache for $type. Defaults to FALSE.
 *
 * @return
 *   The value specified by $id, or FALSE if no value was found.
 */
function category_get_cached_item($type, $id, $id_key, $query, $where, $order_by = FALSE, $reset = FALSE) {
  // The $overflow_count variable determines how many values this function
  // will query for in one go. Tweak if necessary.
  static $overflow_count = 20, $cache = array(), $overflow_from = array();

  if (!isset($id) || $id === '') {
    return FALSE;
  }

  if (!isset($cache[$type]) || $reset) {
    $cache[$type] = array();
    $overflow_from[$type] = 0;
  }

  if (!isset($cache[$type][$id])) {
    $result = db_query($query .' WHERE '. $where .($order_by ? ' ORDER BY '. $order_by : ''), $id);
    if ($item = db_fetch_object($result)) {
      $cache[$type][$id] = $item;
    }

    if ($overflow_from[$type] !== FALSE) {
      $result = db_query_range($query, $overflow_from[$type], $overflow_count);
      if (db_num_rows($result)) {
        $i = 0;

        while ($item = db_fetch_object($result)) {
          $cache[$type][$item->$id_key] = $item;
          $i++;
        }

        $overflow_from[$type] += $i;
      }
      else {
        $overflow_from[$type] = FALSE;
      }
    }
  }

  return isset($cache[$type][$id]) ? drupal_clone($cache[$type][$id]) : FALSE;
}

/**
 * Given a node, this function returns the depth of the node in its hierarchy.
 * A root node has depth 1, and children of a node of depth n have depth (n+1).
 *
 * @param nid
 *   The nid of the node whose depth to compute.
 *
 * @return
 *   The depth of the given node in its hierarchy.  Returns 0 if the node
 *   does not exist or is not part of a category hierarchy.
 */
function _category_get_depth($cid) {
  $depth = 0;
  if ($cid) {
    while ($cid) {
      $result = db_query(db_rewrite_sql('SELECT h.parent FROM {category_hierarchy} h INNER JOIN {category} c ON h.cid = c.cid INNER JOIN {node} n ON h.cid = n.nid WHERE h.cid = %d ORDER BY c.weight, n.title'), $cid);
      $obj =  db_fetch_object($result);
      $parent = $obj->parent;
      if ($cid == $parent->parent) {
        $cid = 0;
      }
      else {
        $cid = $parent;
      }
      $depth++;
    }
    return $depth;
  }
  else {
    return 0;
  }
}

/**
 * Helper for category_category_count_nodes().
 */
function _category_category_children($cid) {
  static $children;

  if (!isset($children)) {
    $result = db_query('SELECT cid, parent FROM {category_hierarchy}');
    while ($category = db_fetch_object($result)) {
      $children[$category->parent][] = $category->cid;
    }
  }
  return $children[$cid] ? $children[$cid] : array();
}

/**
 * Helper function for array_map purposes.
 */
function _category_get_cid_from_category($category) {
  return $category->cid;
}
