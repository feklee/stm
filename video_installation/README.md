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

  * Fader: Regularly output its value, and possibly also compare tick values,
    or get rid of comparison altogether.

  * Idle mode: Don’t record positions, or only optionally.

  * Allow submission of programs as a succession of modes. Programs / Modes can
    be interrupted by sending another one.

  * Speed up, to get faster feedback loop and avoid crashes: Do approach, scan,
    retract in one go, and only in the end send data.

  * To speed up, things to try:

      + Don’t check fader in every loop.

      + Disable serialization.


Legal
=====

Unless mentioned otherwise, stuff is licensed under the WTFPL version 2:
<http://www.wtfpl.net/>
