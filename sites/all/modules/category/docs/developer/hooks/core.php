<?php
// $Id: core.php,v 1.2 2006/03/05 07:17:03 jaza Exp $

/**
 * @file
 * These are the hooks that are invoked by the Drupal category package.
 *
 * Core hooks are typically called in all modules at once using
 * module_invoke_all().
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Manipulate categories and containers, and respond to category-related events.
 *
 * Hook_category() is designed to extend the functionality of hook_nodeapi() 
 * for category-specific situations. It allows other modules to define things 
 * such as extra attributes for categories, and extra general category 
 * settings.
 *
 * @param $op
 *   What kind of action is being performed. Possible values:
 *   - "form": Inject form fields into the add/edit container node form. May 
 *     be phased out soon, in favour of hook_form_alter().
 *   - "settings": Inject form fields into the 'administer -> settings -> 
 *     category' page. This allows all modules in the category package to 
 *     centralize their settings on one page. Each module's settings should 
 *     be grouped inside a collapsed fieldset.
 *   - "view": The node is about to be viewed. Modules can append additional 
 *     output to $node->body if they wish.
 * @param $node
 *   - The node the action is being performed on (optional).
 * @return
 *   This varies depending on the operation.
 *   - The "form" and "settings" operations should return a $form array, 
 *     suitable for passing to drupal_get_form().
 *   - The "view" operation should return the themed output to be appended 
 *     to $node->body;
 */
function hook_category($op, $node = NULL) {
  switch ($op) {
    case 'form':
      $form['category_foo'] = array(
        '#type' => 'fieldset',
        '#title' => t('Foo information'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['category_foo']['category_foo_bar'] = array(
        '#type' => 'textfield',
        '#title' => t('Bar text'),
        '#default_value' => $node->foo_bar,
        '#description' => t('The bar text for foo.'),
      );
      return $form;

    case 'settings':
      $form['category_foo'] = array(
        '#type' => 'fieldset',
        '#title' => t('Foo settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['category_foo']['foo_bar'] = array(
        '#type' => 'checkbox',
        '#title' => t('Bar text for foo'),
        '#default_value' => variable_get('category_foo_bar', 0),
        '#description' => t('If checked, foo gets bar text.'),
      );
      return $form;

    case 'view':
      return theme('foo_bar', $node);
  }
}

/**
 * Define one or more export formats for categories. Each export format 
 * defined here can then be enabled or disabled on a per-container basis.
 *
 * @return
 *   An array of export formats, where the key of each element is the system 
 *   name of the export format, and the value is the human-readable name of 
 *   the format.
 */
function hook_category_export() {
  return array('foo' => t('FooML'));
}

/**
 * Respond to category import and export events.
 *
 * Modules can hook in to the category import and export processes at various 
 * stages, and can affect how these processes occur.
 *
 * @param $op
 *   What kind of action is being performed. Possible values:
 *   - "import_taxonomy_form" and "import_book_form": Inject form fields into 
 *     the book/taxonomy import form. May be phased out soon, in favour of 
 *     hook_form_alter().
 *   - "import_taxonomy_prepare" and "import_book_prepare": perform any 
 *     necessary processing on inputted form data, before proceeding with the 
 *     import.
 *   - "import_taxonomy_submit" and "import_book_submit": perform additional 
 *     actions after the import has taken place.
 * @param &$data
 *   The data that gets imported. Hooks that implement the "_prepare" 
 *   operations should assign values to this array, or they will get lost.
 * @param $edit
 *   Optional. The form values submitted by the user. Hooks that implement 
 *   the "_prepare" operations should copy the values from this array to 
 *   somewhere more permanent, or they will get lost.
 * @param $legacy_map
 *   Optional. An array that maps old IDs to the IDs of their new imported 
 *   equivalents. Particularly useful for hooks implementing the 
 *   "import_taxonomy_submit" operation.
 * @return
 *   This varies depending on the operation.
 *   - The "import_taxonomy_form" and "import_book_form" operations should 
 *     return a $form array suitable for processing by drupal_get_form().
 *   - The "import_taxonomy_prepare", "import_book_prepare", 
 *     "import_taxonomy_submit", and "import_book_submit" operations have no 
 *     return value.
 */
function hook_category_legacy($op, &$data, $edit = NULL, $legacy_map = NULL) {

  switch ($op) {
    case 'import_taxonomy_form':
    case 'import_book_form':
      $form['category_foo'] = array(
        '#type' => 'fieldset',
        '#title' => t('Foo information'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['category_foo']['foo_bar'] = array(
        '#type' => 'textfield',
        '#title' => t('Bar text'),
        '#description' => t('This text will be given to the bar field for all imported data.'),
      );
  
      return $form;

    case 'import_taxonomy_prepare':
    case 'import_book_prepare':
      foreach ($data as $key => $item) {
        $fields = array('foo_bar');
        foreach ($fields as $field) {
          $data[$key][$field] = $edit[$field];
        }
      }
      break;

    case 'import_taxonomy_submit':
    case 'import_book_submit':
      drupal_set_message(t('Appended foo information to new categories.'));
    }
}
/**
 * @} End of "addtogroup hooks".
 */

?>
