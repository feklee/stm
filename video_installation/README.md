Instructions
============

 1. Upload code to Teensy 3 or compatible.

 2. Start server:

        $ cd server
        $ node app.js

 3. Open `app.html` in browser, in full screen mode on a 16:9 screen.


Coding conventions
==================

  * Maximum line length: 80 characters

  * Comments in Markdown

  * JavaScript passes JSLint.


Ideas
=====

  * Slowly scroll up graphs on fade-out, e.g. by sending small data packages,
    or by running an animation, putting it in “fade out mode”, or even using a
    CSS animation.

    Maybe introduce `graph.fadeOut()` and call it from the finish function of
    the scan image.

  * Allow submission of programs as a succession of modes. Programs / Modes can
    be interrupted by sending another one.

  * Speed up, to get faster feedback loop and avoid crashes: Do approach, scan,
    retract in one go, and only in the end send data.


Legal
=====

Unless mentioned otherwise, stuff is licensed under the WTFPL version 2:
<http://www.wtfpl.net/>
