********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Workflow Module
Author: John VanDyk
Maintainers: John VanDyk http://drupal.org/user/2375
  Mark Fredrickson http://drupal.org/user/31994
  Jacob Singh http://drupal.org/user/68912
Drupal: 5
********************************************************************
DESCRIPTION:

The workflow module enables you to create arbitrary workflows in 
Drupal and associate them with node types.

Workflows are made up of workflow states.

Moving from one state to another is called a transition.

Actions are associated with transitions (the actions module 5.x-2.x
or later must be installed for this).

Alex Reisner introduced role-based permissions for workflow states
and generally enhanced this module.

Earl Miles wrote the original workflow_access module, and Gabor
Hojtsy upgraded it to 5.x

********************************************************************
INSTALLATION:

1. Place the entire workflow directory into your Drupal modules/
   directory.


2. Enable the workflow module by navigating to:

     Administer > Site building > Modules

   Enabling the workflow module will create the necessary database 
   tables for you.

3. If you want anyone besides the administrative user to be able
   to configure workflows (usually a bad idea), they must be given
   the "administer workflow" access permission:
   
     Administer > User management > Access control

   When the module is enabled and the user has the "administer
   workflow" permission, a "workflow" menu should appear in the 
   menu system under Administer > Site building > Workflow.

	 You may also allow only some users to schedule transitions. Select
	 the "schedule workflow transitions" permission to allow transitions.

********************************************************************
GETTING STARTED:

Let's create a new workflow. Click on Administer -> Site building -> 
Workflow and click on the "Add workflow" tab.

We'll start simple. Call our workflow "Draft-Done" and click Add Workflow.

Now lets add some workflow states to our workflow. Click "add state" and
enter "draft" and click the Add State button. Do the same for "done".

So we've got a workflow with two states, "draft" and "done". Now we
have to tell each state which other states it can move to. With only
two states, this is easy. Click on the "edit" link to edit the workflow
and see its states.

The "From / To -->" column lists all states. To the right are columns
for each state. Within each cell is a list of roles with checkboxes.

This is confusing. It's easiest to understand if you read rows
across from the left. For example, we start with the creation
state. Who may move a node from its creation state to the "draft"
state? Well, the author of the node, for one. So check the "author"
checkbox.

Who may move the node from the "draft" state to the "done" state?
This is up to you. If you want authors to be able to do this,
check the "author" checkbox under the "done" state. If you had
another role, say "editor", that you wanted to give the ability
to decree a node as "done", you'd check the checkbox next to
the "editor" role and not the author role. In this scenario authors
would turn in drafts and editors would say when they are "done".

Be sure to click the Save button to save your settings.

Now let's tell Drupal which node types should use this workflow. Click
on Administer -> Site building -> Workflow. Let's assign the Draft-Done 
workflow to the "story" node type and click Save Workflow Mapping.

Now we could add an action (previously configured using the actions
module). Click on the Actions link above your workflow. This will bring
you to the Triggers page showing all events that can trigger actions. Each
workflow transition from one state to another is an event that can trigger
an action. You can see all of these transitions under the Workflow tab on
the Triggers page. Add the action to the transition.

Now create a new story by going to Create content -> Story. Note that
there is no sign of workflow here because the story is in its
initial state. Click submit to create the story.

Now click the edit tab. Note that there is a select box for workflow
with the "draft" state chosen.

Changing the state to "done" and clicking Submit will fire the action
you set up earlier.

********************************************************************
WORKFLOW ACCESS module:

The workflow package contains an additional module, workflow access. This
module allows workflow administrators to set which roles can access nodes
based on their state. You can also set edit and delete permissions by state.

To use workflow access, visit Administer -> Site building -> Modules and enable
the  "Workflow access" module. Then visit Administer -> Site building -> 
Workflow and select the workflow on which you would like to enable access 
control.

Each state has it's own set of permissions, where you can edit which roles
may view, edit or delete a given node. By default, both anonymous and 
authenticated users may view. No roles have edit permissions. Make the security
changes for your site and click "Submit." Workflow Access will automatically 
reset the permissions on any nodes in that workflow.

You may use Workflow Access with other node access systems (such as Organic 
Groups). Be aware, however, that Drupal uses an explicit allow system, so if a 
user has access through Organic Groups, he/she will be able to view a node, even
if the workflow permissions would not otherwise allow it.

You can safely disable the Workflow Access module. Any nodes that were under
access control before will be reset to being viewable by everyone, but editable
only to users with the "Administer nodes" permission.