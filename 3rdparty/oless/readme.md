OLESS - OLE Structured Storage Utility
======================================

https://github.com/DBHeise/oless

build scripts included for both windows (build.cmd) and linux (build.sh)

Usage
=====
oless {cmd} [CommandSpecificParameters]
Available Commands:
-------------------
        list {file} [outputformat] - Enumerates the streams and storage in the OLESS file, default output format is text
        guess {file} [outputformat] - Guess the extension of the file based on the OLESS contents, default output format is text
        dump {file} {streamname} {outfile} - Extracts the given stream/storage name to an external file
Available Output Formats:
-------------------------
        xml,csv,json,txt
