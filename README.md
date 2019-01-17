mfsum : subset sum solver
=========================

You have a pile of numbers and you want to know if any sub-group of
numbers in the pile sum to some target. This is tricky to do by hand
with a small group and nearly impossible to do with anything larger.

Given a set of numbers

    267 961 1153 1000 1922 493 1598 869 1766 1246

is there any sub-group that sums to `5842`?

Trying to do this by hand is a waste of time.

Prerequisites
-------------

First, make sure the required packages are installed.

    autotools autotools-dev automake autoconf libtool gcc

Second, re-build the build environment after cloning the repository.

    autoreconf --force --install

Installing
----------

    ./configure
    make
    sudo make install

This program may benefit from more agressive optimization. You can
control this when invoking the configure script.

    ./configure CFLAGS=-O3

Running
-------

mfsum only accepts positive integer input. It accepts input from either
the command line or via standard input.  The program requires three
pieces of information in order to run.

* target value
* trim value
* list of numbers

The target value and the list need no explanation but the trim number
does. It should be a number between 0 and 1 and providing this parameter
will tell the program to find a approximate solution. Fore example,
if you use 0.40, the program will find a sum that is within 40% of
the target. Using this feature is advisable when the input set is very
large. The trim value allows the program to drastically reduce running
times since it is a polynomial time algorithm while the exact mode uses
a exponential algorithm. Entering a 0 for the trim value envokes the
exact sum mode.

Examples
========

Command line input
------------------

    mfsum 5842 0 267 961 1153 1000 1922 493 1598 869 1766 1246
    5842 = 869 + 961 + 1000 + 1246 + 1766

Specifying an approximation
---------------------------

The following example will find a solution that is within 10% of the target.

    mfsum 5842 0.1 267 961 1153 1000 1922 493 1598 869 1766 1246
    5802 = 961 + 1153 + 1766 + 1922

Standard input
--------------

Invoking the program with a dash as its only argument instructs the
program to wait for standard input. In this mode, the arguments must
be provided line-by-line. The first and second lines must contain the
target and trim values, respectively. The rest of the lines should
contain the set.  This makes it easy to provide larger automated inputs
to mfsum. The input can be provided with a file as with this example.

**File input.txt**

    5009813684
    0
    212153178
    1427863024
    1366863723
    2027395864
    2129420246
    1017464478
    640366018
    1243516659
    729987169
    1039115050
    607438639
    265725058
    2008186886
    1121354568
    1316780152

**Running**

    mfsum - < input.txt
    265725058
    607438639
    640366018
    1366863723
    2129420246

Abnormal Termination
--------------------

If the program is terminating with the error "Unable to allocate X bytes"
and you are giving the program a large set of numbers, then the system
could be running out of memory. It may be necessary to increase the
memory limits enforced by the operating system. This can accomplished
with the ulimit command.

<table>
 <tr>
    <td>ulimit -c unlimited</td><td>turn on corefiles with unlimited
    size</td>
 </tr> <tr>
    <td>ulimit -n unlimited</td><td>allows an unlimited number of open
    file descriptors</td>
 </tr> <tr>
    <td>ulimit -d unlimited</td><td>sets the user data limit to
    unlimited</td>
 </tr> <tr>
    <td>ulimit -f unlimited</td><td>sets the file limit to unlimited</td>
 </tr> <tr>
    <td>ulimit -a</td><td>display the current ulimit settings</td>
 </tr>
</table>

Running `ulimit -d unlimited` sould take care of the problem until you
run out of physical memory and start swapping like crazy.

License
-------

mfsum : A Subset Sum Solver Copyright 2018 Mike Fusaro

This file is part of mfsum.

mfsum is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

mfsum is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with mfsum.  If not, see <http://www.gnu.org/licenses/>.
