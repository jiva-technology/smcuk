Drupal dynamicload.module README.txt
==============================================================================

Module for implementing dynamic content loading in Drupal.

When enabled, links are replaced with javascript calls to dynamically
load only the requested page's content.

To test:

* Install and enable module
* Go to block configuration and click 'configure' for a block.
* For the settings in the 'dynamic loading' fieldset, select
  'Apply dynamic loading' and designate a target area (main content
  area or one of the enabled blocks).

Example usages:

A. Menu loads into main content area

1. With menu module, create a menu with links to several pages (nodes, etc.).
   Ideally, the pages would have little content, e.g., a short list of links.
   Enable and position the newly created menu block.
2. Configure the block created in step 1. Check 'Apply dynamic loading'
   and select "Main content area" as the target.

When users click on menu items, they load into the main content area.

B. A  menu block loads into a custom block.

1. With menu module, create a menu with links to several pages (nodes, etc.).
   Ideally, the pages would have little content, e.g., a short list of links.
   Enable and position the newly created menu block.
2. With the block module, create a block that will be the 'target' (where 
   content loads). Enable and position the newly created block.
3. Configure the block created in step 1. Check 'Apply dynamic loading'
   and select the block created in step 2 as the target.

When users click on links in the menu block, the content load into
the block module block. (Note: some page elements are left out for
pages loading into blocks instead of the main content area--breadcrumbs
tabs, etc.).

C. Dynamically load all content (experimental)

At Administer > Site configuration > Dynamicload, select the option to 
load all content. Set paths to exclude (generally recommended to exclude 
admin*).

All links will be processed and loaded dynamically.


Requirements
------------------------------------------------------------------------------
This module is written for Drupal 5.0+ and requires the Javascript Tools 
module to be enabled.


Developer usage
------------------------------------------------------------------------------
To trigger dynamic loading, include a "dynamicload" class on content. 
Individual links with the class will be loaded dynamically. E.g.,:

$link = l(t('my link'), 'my/path', array('class' => 'dynamicload'));

Alternately you can apply the class to an enclosing element and all links
contained in that element will be processed:

$output = '<div class="dynamicload"><a href="some/path">'. t('my text') 
.'</a></div>'