********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Workflow Owner
Author: Jacob Singh <jacobsingh -a t- gmail.com>
Drupal: 5
********************************************************************
DESCRIPTION:

This module adds the ability for users to associate users as "Owners" of workflow states defined by the workflow module.


********************************************************************
INSTALLATION:

1. Enable the workflow owner module by navigating to:

     administer > build > modules

   Enable the module, this will create a new table, {workflow_state_owners}
   
2. Create a workflow, and start using it!

********************************************************************
GETTING STARTED:

For instance, create a workflow with the states:

Draft
Review
Final Review

and you have 3 users:

mr_writer
ms_editor
dr_publisher

and you apply that workflow to the story node.

mr_writer logs in and creates a story "big party on the 5th").  
mr_writer sees a new fieldset on the node form called call "state owners"
from there, he can assign users to each state, so for draft, he adds himself,
 for Review, he puts in ms_editor and for Final Review, he puts in dr_publisher.

When the workflow state changes to Review, the story will show in ms_editors
 workflows owned queue (available from user/ as a local task).

In addition, if you have implemented workflow_ng, you can fire events such
 as sending emails out to new state owners when they become the owner.

