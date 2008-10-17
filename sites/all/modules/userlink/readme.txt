// $Id: readme.txt,v 1.3 2007/01/27 22:23:35 marcp Exp $

The userlink module allows users to save and share links. To access the full potential of the userlink module, we recommend using the functionality offered by freetagging, and the contributed module tagadelic (http://drupal.org/project/tagadelic).

Userlinks are fully themeable, and the module comes with a css file that defines the necessary values to theme your links. To use the css file, paste it into your existing theme, and modify it as needed.

Userlink comes with a series of userlink-related blocks: view all links, view all link categories, view popular link categories, recent links, my links, my link categories, and the link menu. There is some overlap here, but it provides the end user with more choices. Enable the ones you want, and ignore the rest.

To install the userlink module:

1. navigate to administer --> modules, and enable the userlink module. The database table will be created for you.
2. navigate to administer --> access control, and assign rights to userlinks.
3. navigate to administer --> categories
   a. add a vocabulary with the following recommended settings:
     (1) For name, description, and help text, put in the values that work best for you -- these settings aren't particularly critical
     (2) For type, check "userlink"
     (3) For hierarchy, select "single"
     (4) Select "free tagging", "multiple select", and "required"
4. Navigate to administer --> blocks, and activate the blocks you want
5. Start saving, tagging, and sharing some links.
6. To configure the way links appear in your browser when clicked, navigate to administer --> settings --> userlink


rousehouse - January 22/07
=======================
The following additions have been made to this version of the module.
- Upgraded the module to be compatible with Drupal 5.0.x.x
- Added an optional prefix to links
- Added ability to customize prefix
- 