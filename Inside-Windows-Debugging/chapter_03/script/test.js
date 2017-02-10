//
// Store command-line parameters and call the main function
//
var g_argv = new Array();

for (var i=0; i < WScript.Arguments.length; i++)
{
    g_argv.push(WScript.Arguments(i));
}

WScript.Quit(main(g_argv));

function main(argv)
{
    WScript.Echo("Script started at " + new Date());
    if (g_argv.length > 0)
    {
        WScript.Echo("Arguments: " + g_argv);
    }
}
