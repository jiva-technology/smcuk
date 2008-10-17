OVERVIEW
--------

Cac_lite stands for Category Access Control Lite.  This module
restricts access so that some users may see content that is
hidden from others.  A simple scheme based on category, roles and
users controls which content is hidden.

This module is a spin-off of a module written for use with the
taxonomy module.  That module is called Taxonomy Access Control Lite.
As the name implies, these modules share some functionality with an
earlier module called Taxonomy Access Control (TAC).  If you are
shopping around for an access control module to use, consider that one
as you may find that it suits your needs.  In my case, I wanted access
control but without some of the complexity introduced by TAC.  I also
wanted more flexibility in granting access on a per user basis.

Here are some key features of cac_lite:

* Designed to be as simple as possible in installation and administration.  

* Uses Drupal's node_access table, db_rewrite_sql hook and
  category module to leave the smallest possible footprint while doing
  it's job.  For example, it introduces no new database tables.

* Grant permissions based on roles.

* Grant permissions per user.  (Give a specific user access beyond
  what his/her roles allow).

INSTALL
-------

For drupal 4.7.

Enable category module.  It's required.

Install this package the normal way.
- put this file in a subdirectory of the modules directory.
- enable using admin interface
- no database tables to install.


USAGE
-----

Log in as an administrator. (uid==1, or a user with
administer_cac_lite permission)

Create a container which you will use to categorize private nodes.
You may want to call this "Private Projects".

Associate the container with node types, as you would normally do.

Go to administer >> access control >> cac_lite.

Select the container you created in the earlier step ("Private Projects").

Add several categries to that container.

Create some content.  Choose a node type associated with "Private Projects".

Note that you can view the content you just created.  Other users cannot.

Edit the account of another user.  Go to the cac_lite access tab under edit.

Select a term you selected when creating the node and submit changes.

Now the user can also access the node you created.

USAGE TIP
---------

First create a container called 'Privacy'.  Under 'container
information', select the node types to which this Privacy applies (you
can create other containers which control privacy later, this does not
have to be the only one).  Select at least 'container'.  Also enable
hidden container.

Go to administer >> access control >> cac_lite, and select the Privacy
container.  Important: do this before adding categories to the
container.

Now, return to the Privacy container and add terms.  These should be
whatever makes sense to you.  ie. 'public', 'my eyes only',
'collaborators', 'clients'.  Whatever describes who should be able to
see the nodes with these terms applied.

Optionally, return to the Privacy page and hide it from the general
public by selecting one of the terms you just added (i.e. 'my eyes
only')

Go to administer >> access control >> cac_lite >> access by role (or
user edit pages) and configure who can see the terms you just added to
privacy.

To add more containers that control privacy, first add the container
(You may select from 'Privacy' if the container itself should be
visible only to certain users).  Then return to administer >> access
control >> cac_lite and select the new container (use multiple select,
ensure that Privacy remains selected).  Then add terms to the
contrainer.  It is important to select the container on the cac_lite
page BEFORE adding terms.  This makes sure the terms themselves are
under access control (if you skipped that step, simply return to the
terms and re-save them one by one).

IMPORTANT NOTE
--------------

If you have no access_control modules installed, when you first
install this module it will hide all existing content from most users
(except uid==1).  This has to do with the way Drupal's node_access
table works.  To make nodes visible, you have to re-save them
(i.e. edit the node and simply hit submit without changing anything).
This will cause the appropriate entries to be written to the
node_access table.  Of course, there should be a way to automate this
process.  Any volunteers?

The following SQL should also make all nodes visible to all users:

INSERT INTO node_access (nid, gid, realm, grant_view) SELECT nid, 0, 'tac_lite', 1 FROM node;

RELATED MODULES
---------------

There is a sister module called tac_lite which is nearly identical,
but uses the taxonomy module instead of the category module.  You
could theoretically install taxonomy, category, tac_lite and cac_lite
at the same time.  But you probably do not want to.

There is a sub-module of the devel module, called devel_node_access
which can give you some insight into the contents of your node_access
table.  Recommended for troubleshooting.


AUTHOR
------

Dave Cohen <http://drupal.org/user/18468>
