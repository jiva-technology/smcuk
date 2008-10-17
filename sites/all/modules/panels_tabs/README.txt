$Id: README.txt,v 1.1.2.3 2008/01/02 16:51:31 wimleers Exp $

Description
-----------
Configurable 'tabs' panel style. Provides 3 kinds of tabs:
- Normal tabs (horizontal filling disabled).
- Horizontally filling, equal width tabs: sets the width property, forcing
  each tab to be equally wide. If the text doesn't fit in the tab, the
  overflow will be hidden.
- Horizontally filling, smart width tabs: calculates the length of the text in
  each tab and compares this to the total length of the text on all tabs. It
  then sets the width property of each tab according to the percentage of text
  the tab contains.


Dependencies
------------
* Panels 2 (http://drupal.org/project/panels)
* Tabs (part of Javascript Tools, http://drupal.org/project/jstools)


Installation
------------
1) Place this module directory in your modules folder (this will usually be
"sites/all/modules/").

2) Enable the module.

3) Go to the "Layout settings" tab of the Panels page, Mini panel, ... on
which you want to apply this style.


Sponsor
-------
Paul Ektov of http://autobin.ru.


Author
------
Wim Leers

* mail: work@wimleers.com
* website: http://wimleers.com/work

The author can be contacted for paid customizations of this module as well as
Drupal consulting, development and installation.
