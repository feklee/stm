Instructions
============

 1. Upload code to Teensy 3 or compatible.

 2. Start server:

        $ cd server
        $ node app.js

 3. Open `app.html` in browser.


Coding conventions
==================

  * Maximum line length: 80 characters

  * Comments in Markdown

  * JavaScript passes JSLint.


Ideas
=====

  * Fader: Also compare ticked values, to avoid unnecessary updates.

  * Idle mode: Don’t record positions, or only optionally.

  * Add microsecond timestamps and use them for profiling: Calculate entire
    scan time, and output it to Node.js console.

  * `_chunkSize` → `_bufferSize`, `_log` → `_buffer`

  * Allow submission of programs as a succession of modes. Programs / Modes can
    be interrupted by sending another one.

  * Speed up, to get faster feedback loop and avoid crashes: Do approach, scan,
    retract in one go, and only in the end send data.


Legal
=====

Unless mentioned otherwise, stuff is licensed under the WTFPL version 2:
<http://www.wtfpl.net/>
