

\mainpage GnuCashew ~ Web Based Version of GnuCash

First off... I love <a href="https://www.gnucash.org" target="_blank">GnuCash!</a>

Secondly... this is ~not~ GnuCash.  At present, GnuCash code is slightly in use
 here, as I work to connect this to the libgnucash engine library.  So far, it
 is predominantly using direct-sql calls to reach the gnucash database.

This is an attempt to produce a web-based interface for the GnuCash program.
 Knowing that there have been several other attempts, this is based on my
 interest of hacking on GnuCash and also adding additional features, such
 as managed-bill-pay and so forth.

At this point, all of the code in this project has been developed from
 scratch.  The system can (so far) only operate on a sqlite data file.  No code was
 used from the gnucash original source codes, although I am working to use
 the gnucash library engine for accessing the underlying database.

If you are interested in an active demo, you can find it here;

https://gnucashew.lorimarksolutions.com/demo

Source Code Repository is found here;

https://github.com/lorimark/gnucashew

This project uses Wt (https://www.webtoolkit.eu/wt) as the web rendering library.

