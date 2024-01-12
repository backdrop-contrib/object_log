Object Log
========
<!--
The first paragraph of this file should be kept short as it will be used as the
project summary on BackdropCMS.org. Aim for about 240 characters (three lines at
80 characters each).

All lines in this file should be no more than 80 characters long for legibility,
unless including a URL or example that requires the line to not wrap.
|<- - - - - - - This line is exactly 80 characters for reference - - - - - - ->|

Detail in READMEs should be limited to the minimum required for installation and
getting started. More detailed documentation should be moved to a GitHub wiki
page; for example: https://github.com/backdrop-contrib/setup/wiki/Documentation.
-->
The Object Log module allows developers to store objects, arrays and other
variables to a log table so that they may be inspected later. Multiple stored
variables may be displayed side-by-side in the Object Log under admin/reports.

The usage is similar to Devel module's `dpm()` or `kprint_r()` functions, but
is particularly suited for debugging server-to-server requests, such as cron
runs and web services, or for requests from anonymous and other unprivileged
users.

Other uses include where the use of `dpm()` slows down the rendering of a page
to the extent where it is unusable or where Krumo is not able to render an
object.

Usage
---------------------

When you reach a point in a code at which you would like to store a variable,
call the object_log() function...

  ```php
  object_log($label, $data);
  ```

...where $label is a string representing a name to give the object in the log,
and $data is the variable you wish to store. If there is already a stored object
with the same $label, that entry will be overwritten in the log.

If you want to inspect the same object over time, you could append a date
string or other identifier to the label to give distinct entries:

```php
$label = 'object_label' . date('ymd_His')
object_log($label, $data);
```

Stored objects may be inspected by any user with the "access devel information"
permission by going to admin/reports/object_log.  While inspecting an object,
a second object may be selected to display both objects side-by-side.

Requirements
------------
<!--
List any dependencies here. Remove this section if not needed.
-->

This module requires that the following modules are also enabled:

- Devel

Installation
------------
<!--
List the steps needed to install and configure the module. Add/remove steps as
necessary.
-->

- Install this module using the official Backdrop CMS instructions at
  https://docs.backdropcms.org/documentation/extend-with-modules.

Issues
------
<!--
Link to the repo's issue queue.
-->

Bugs and Feature Requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/object_log/issues.

Current Maintainers
-------------------
<!--
List the current maintainer(s) of the module, and note if this module needs
new/additional maintainers.
-->

- [Martin Price](https://github.com/yorkshire-pudding) - [System Horizons Ltd](https://www.systemhorizons.co.uk)
- Collaboration and co-maintainers welcome!

Credits
-------
<!--
Give credit where credit's due.
If this is a Drupal port, state who ported it, and who wrote the original Drupal
module. If this module is based on another project, or uses third-party
libraries, list them here. You can also mention any organisations/companies who
sponsored the module's development.
-->

- Ported to Backdrop CMS by - [Martin Price](https://github.com/yorkshire-pudding) - [System Horizons Ltd](https://www.systemhorizons.co.uk).
- Port sponsored by [System Horizons Ltd](https://www.systemhorizons.co.uk).
- Originally written for Drupal by [Les Lim](https://www.drupal.org/u/les-lim).

License
-------
<!--
Mention what license this module is released under, and where people can find
it.
-->

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.