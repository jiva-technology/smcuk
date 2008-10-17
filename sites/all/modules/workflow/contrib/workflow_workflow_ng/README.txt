********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Workflow Ng Workflow
Author: Jacob Singh <jacobsingh -a t- gmail.com
Drupal: 5
********************************************************************
DESCRIPTION:

This module allows you to integrate workflow and workflow_ng 
(providing you've also got the workflow_token patch applied).By 
intergating workflow and workflow_ng, you can do some powerful
workflow_ng conditions and actions to fire events based on workflow
state changes.

********************************************************************
INSTALLATION:

1. Enable the workflow module by navigating to:

     administer > build > modules

   Enable the workflow_ng_workflow

********************************************************************
GETTING STARTED:

Go to admin/build/workflow_ng.  
You'll notice that there is a new configuration enbaled called 
"Email Author when workflow state changes".
Click edit to enable it, and everytime a workflow state changes the 
node author will get a notification and a message will be displayed on the site.

Edit this workflow_ng configuration to get an idea of how to build your own.

If you have workflow_owner (http://drupal.org/project/workflow_owner) enabled,
you'll get another example config: "Send email to new state owner".

