#ifndef COMMAND_FILE_H
#define COMMAND_FILE_H

enum{
	play_ramp,
	play_sine,
	play_saw,
	play_file
};

void CommandProcess();
void InitializeCommandVaribles();
bool pars_cmd(char * g_cmdBuf, char ** g_cmdName, char **g_cmdPar);

// ¿ØÖÆÃüÁî
bool quit(char* par);
bool play(char* par);
bool stop(char* par);
bool record(char* par);
bool setASIOPar(char* par);
bool help(char*);

#endif