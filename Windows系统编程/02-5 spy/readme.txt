Monitoring Messages with SPY


SUMMARY
=======

The SPY tool demonstrates how to monitor system messages sent to a specified
window. SPY records the messages and displays them on the specified device. 
SPY is also useful in debugging, to verify that the messages you think a 
window is receiving are actually being received.

SPY demonstrates the following techniques:

  - Using a system message hook.
  - Using the WM_COPYDATA message to pass data to another application.
  - Reading and writing the registry.
  - Creating a thread.
  - Creating a DLL for the hook.

MORE INFORMATION
================

To use SPY to monitor messages, do the following:

 1. Choose the Options menu to select the following:

      - Messages: Selects the messages you wish to monitor.
      - Font: Selects the font you wish to have displayed.
      - Output: Selects the output device to which you want messages to go.

 2. Select the window whose messages you want to watch by choosing the 
    Select Window command from the Spy menu. This brings up a dialog box 
    listing the current windows to watch. The default is all windows.

 3. The Edit menu allows you to use the clipboard from the SPY window.

 4. Choose Start! to start monitoring. Once this option is chosen, the menu 
    item changes to Stop!. Choose Stop! to cease monitoring messages.
