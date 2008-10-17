prepopulate.module -- populate forms from $_GET
---

by [ea. Farris] [1], based on an idea from chx

prepopulate.module is an attempt to solve the problem that resulted from
the discussion at:

  http://www.drupal.org/node/27155

where the $node object, it was (correctly, I believe) decided, should
not be prefilled from the $_GET variables, and instead, the power of the
FormsAPI should be used to modify the #default_value of the form
elements themselves.

This functionality will make things like bookmarklets easier to write,
since it basically allows forms to be prefilled from the URL, using a
syntax like:

http://www.drupalsite.org/node/add/blog?edit[title]=this is the title&edit[body]=body goes here

At this time, the module could use some testing and refining. Seems to
be working well in my limited tests.


---
References

[1] : http://www.drupal.org/user/812/contact
