Instructions
============

 1. Build and upload Arduino sketch `microcontroller` to Teensy 3 or
    compatible.

 2. Start server:

        $ cd server
        $ npm start

 3. Open index page in browser, in full screen mode on a 16:9 screen. Example
    URL (adapt to your system):

        http://localhost:8080


Coding conventions
==================

  * Maximum line length: 80 characters

  * Comments in Markdown

  * JavaScript passes JSLint.


Ideas
=====

  * `boolean` → `bool`

  * Adjust piezo during scan, trying to keep current constant.

  * Slowly scroll up graphs on fade-out, e.g. by sending small data packages,
    or by running an animation, putting it in “fade out mode”, or even using a
    CSS animation.

    Maybe introduce `graph.transitionOut()` and call it from the finish
    function of the scan image.

  * Allow submission of programs as a succession of modes. Programs / Modes can
    be interrupted by sending another one.


Legal
=====

Unless mentioned otherwise, stuff is licensed under the WTFPL version 2:
<http://www.wtfpl.net/>
