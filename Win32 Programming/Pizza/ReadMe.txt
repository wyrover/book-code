The Skeleton application can be built with Visual C++ 2.2 or later.

Step 1.
    Run Visual C++ 2.2 or later.

Step 2.
    Select the File New menu item.

Step 3.
    From the displayed listbox, select "Project" and
    click on the OK pushbutton.

Step 4.
    In the displayed dialog box, specify the project
    name as "Skeleton", the project type as "Application"
    (not "MFC Appwizard (exe)"), and the desired subdirectory
    name (usually the same name as the project or the 
    subdirectory already containing the source files).
    Click on the "Create" pushbutton.

Step 5.
    In the displayed dialog box, click on the Add All
    pushbutton to add all the .c files residing in the
    project subdirectory to the project. Click on the
    OK pushbutton.

Step 6.
    Select the Project Settings menu item.
    
Step 7.
    In the displayed dialog box, click on the Link tab.

Step 8.
    In the Object/Library Modules edit control, append the
    library names "version.lib comctl32.lib" to the space
    character separated list of names already present.
    The default list is "kernel32.lib user32.lib gdi32.lib
    winspool.lib comdlg32.lib advapi32.lib shell32.lib
    ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib".

Step 9.
    Click on OK.

Step 10.
    Select the Project Rebuild All menu item to build the
    application.

