WScript.Quit(main());

function DoWork()
{
    WScript.Echo("Some work...");
}

function DoMoreWork()
{
    WScript.Echo("Some more work.");
}

function main()
{
    DoWork();

    debugger;

    DoMoreWork();
}
