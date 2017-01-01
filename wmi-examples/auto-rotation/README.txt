My Dell U2413 monitor supports reporting its orientation, which is neat, but the
"Dell Display Manager" software that actually implements it is broken, often taking
ten or fifteen seconds to respond to an orientation change and sometimes not
doing it at all.  This application reimplements this, detecting the monitor
orientation using VCP queries and setting the orientation appropriately.  It
responds in under a second, so I'm not sure why Dell's software is so broken.

This is very simple, providing no UI except for a tray icon to shut it down
and no error reporting.  It'll only work with monitors that support reporting
orientation, and it's only been tested in Windows 7.

