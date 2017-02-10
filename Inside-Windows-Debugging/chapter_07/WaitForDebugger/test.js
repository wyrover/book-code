WScript.Quit(main());

var g_bAttached;
function WaitForDebugger()
{
    g_bAttached = false;
    while (!g_bAttached);
}

function RunScript()
{
    WScript.Echo("Running the script...");
}

function main()
{
    WaitForDebugger();

    RunScript();
}
