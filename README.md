unpage
======

Windows utility to force swapped-out pages to be read back into memory

Usage
-----

Type `unpage -h` for a short help text.

Type `unpage -l` to get a list of all running processes.

Type `unpage <pid> ...` to unpage the specified processes, e.g. `unpage 1234 2121 9876`.

Matching by process name is not currently implemented.


How it works
------------

See [this Stack Overflow question](http://stackoverflow.com/questions/2939599/is-it-possible-to-unpage-all-memory-in-windows).
