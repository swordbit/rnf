Advanced Contact
http://drupal.org/project/advcontact
================


DESCRIPTION
------------
This module provides additional features to the core contact module including providing a default subject and setting the category through URI arguments.

This can be useful for sites that want to be able to link to a specific contact form from different sections of the site.

This module doesn't do much now but I would like it to possibly add support for more features like user contact options and per category options. At this point the module is very simple but functional with the above listed features and nothing else.


REQUIREMENTS
------------
Drupal 6.x


INSTALLING
------------
1. Copy the 'advcontact' folder to your sites/all/modules directory.
2. Go to Administer > Site building > Modules. Enable the module.


CONFIGURING AND USING
---------------------
With this module you can:
        * Pass "category" as a GET arguement to set the category of the contact. This coresponds directly to the text of the category.
        * Pass "subject" as a GET arguement to set the subject of the contact form.
        * Pass "message" as a GET arguement to set the message of the contact form.

An example would be:
l('Provide feedback about our new frontpage', 'contact', NULL, 'category=Website feedback&subject=New frontpage feedback') 

With this module the listed code would link to a page("example.com/contact?category=Website feedback&subject=New frontpage feedback") that has the category "Website feedback" selected and hidden and the subject filled in with "New frontpage feedback". So when the form is filled out and submitted all the things that would happen if that form where normally filled out and that category selected will happen.

Related how-to guide: http://drupal.org/node/304108


UPGRADING
---------
1. One of the most IMPORTANT things to do BEFORE you upgrade, is to backup your site's files and database. More info: http://drupal.org/node/22281
2. Disable actual module. To do so go to Administer > Site building > Modules. Disable the module.
3. Just overwrite (or replace) the older module folder with the newer version.
4. Enable the new module. To do so go to Administer > Site building > Modules. Enable the module.
5. Run the update script. To do so go to the following address: www.yourwebsite.com/update.php
Follow instructions on screen. You must be log in as an administrator (user #1) to do this step.

More info about upgrading: http://drupal.org/node/250790

