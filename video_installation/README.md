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

  * `ModeQueue`:

      + Actually an array is sufficient, because wrap-around will never happen.

      + Make sure that the same mode is not twice in queue, because parameters
        can only be for one. Idea for JSON:

            {"approach":{"queuePosition":1},"retract":{"queuePosition":3},
            "scan":{"queuePosition":2,"sideLen":16}}

        Then store the positions in the mode objects. To find the next
        position, iterate over them and compare `queuePosition()` to position
        of last object.

        Then one can also use the size of an array `modes` for initializing the
        JSON parser.

        Mode object method `removeFromQueue()`: Sets position to
        `idleMode.queuePosition() + 1`.

        Or just keep as is, and live with the overwriting fact, note it down
        maybe.

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
