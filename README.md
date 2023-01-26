
Simplenews Content Selection
====

Simplenews Content Selection allows you to select already created content to
assemble in the body of a newsletter (provided by the Simplenews module)
content type. 

In Backdrop's version of this module, the selection of content is done through a provided View.


Dependencies
------
- [Simplenews](https://backdropcms.org/project/simplenews)
- [Views Bulk Operations](https://backdropcms.org/project/views_bulk_operations)

Installation
------------
Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.


How it works
--------------------------------------------------------------------------------
This module provides a View and a custom Views Bulk Operation called "Create newsletter". This View is
available as the "Newsletter Creation" tab under "Content".

Visit the View, check some content in the provided View, choose the "Create newsletter" action and you
will be provided a sorting form along with the option to create a Table of
Contents.

When validating, you will be redirected to a newsletter creation form with the
body field already filled in with the content you selected.

You may also create custom Views of Content and add the VBO action "Create newsletter".

Configuration
--------------------------------------------------------------------------------
You can configure the module at admin/config/services/simplenews/settings/simplenews_content_selection
you can change the default view mode (which is the one provided by Simplenews Content Selection) used
when generating the output of nodes selected on newsletter creation.

When multiple content types are flagged as newsletter content types, you can
select which content type to generate when using Simplenews Content Selection. This is hidden by default
as the "Newsletter" content type is the only one by default.


Overriding output
--------------------------------------------------------------------------------
The output generated and placed in the body is built from the view mode set in
the module configuration.

You can override the Backdrop way in your theme, for example by specifying a
custom template for this view mode.

You can, for example, create a `node--post--simplenews-content-selection.tpl.php` that will be taken into
account after flushing the theme registry and when creating a new newsletter. This way you can modify the way
nodes are themed when they are inserted into the Body field of the newsletter node.

You can also override the Table of Contents output by implemeting a function in
your `template.php` file: `function YOURTHEME_simplenews_content_selection_toc($variables) {}`

Issues
------
Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/simplenews_content_selection/issues.


Contributors (Drupal version)
--------------------------------------------------------------------------------
SebCorbin http://drupal.org/user/412171 (current Drupal maintainer)
This module was sponsored by Makina Corpus (http://www.makina-corpus.com).

Ported to Backdrop and maintained by [argiepiano](https://github.com/argiepiano)

License
-------
This project is GPL v2 software. 
See the LICENSE.txt file in this directory for complete text.
