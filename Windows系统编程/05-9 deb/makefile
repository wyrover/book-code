# -------------------------------------------------------------------------
#
#                       Microsoft Developer Support
#              Copyright (c) 1992-1997 Microsoft Corporation
#
# -------------------------------------------------------------------------
# MODULE  : MakeFile
# PURPOSE : Nmake macros for building the Windows 32-Bit Debug Event
#           Browser Application
# -------------------------------------------------------------------------

# Nmake macros for building Win32 applications

!Include <Win32.Mak>

# Macro Defines
PROJ=DEB
OBJ1=DEBMain
OBJ2=DEBDebug
OBJ3=DEBMisc
DLL1=LinkList
DLL2=ToolBar

# Target List
All: $(PROJ).Exe $(DLL1).Dll $(DLL2).Dll $(PROJ).Hlp

# Suffixes
.SUFFIXES: .Def .Hpj

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.C
.Def.Lib:
    $(implib) -def:$*.Def $*.Obj -out:$*.Lib
.Def.Exp:
    $(implib) -def:$*.Def $*.Obj -out:$*.Lib
.RC.Res:
    $(rc) $(rcvars) -r -fo $*.Res $*.RC
.Obj.Dll:
    $(link) $(ldebug) $(dlllflags) -out:$*.Dll $** $(guilibsmt)
.Res.Exe:
    $(link) $(ldebug) $(guilflags) -out:$*.Exe $** $(guilibsmt)
.Hpj.Hlp:
    -@if exist $*.Ph del $*.Ph
    $(hc) $*.Hpj

# Source Objects
$(OBJ1).Obj: $(OBJ1).C $(OBJ1).H $(OBJ2).H $(OBJ3).H $(DLL1).H $(PROJ).H
$(OBJ2).Obj: $(OBJ2).C $(OBJ2).H $(OBJ3).H $(DLL1).H $(PROJ).H
$(OBJ3).Obj: $(OBJ3).C $(OBJ3).H $(DLL1).H $(DLL2).H $(PROJ).H
$(DLL1).Obj: $(DLL1).C $(DLL1).H
$(DLL2).Obj: $(DLL2).C $(DLL2).H

# Import/Export Libraries
$(DLL1).Lib $(DLL1).Exp: $(DLL1).Obj $(DLL1).Def
$(DLL2).Lib $(DLL2).Exp: $(DLL2).Obj $(DLL2).Def

# Resources
$(PROJ).Res: $(PROJ).Rc $(PROJ).Dlg $(PROJ).H

# Dynamic Link Libraries
$(DLL1).Dll: $(DLL1).Obj $(DLL1).Exp
$(DLL2).Dll: $(DLL2).Obj $(DLL2).Exp

# Executables
$(PROJ).Exe: $(OBJ1).Obj $(OBJ2).Obj $(OBJ3).Obj $(DLL1).Lib $(DLL2).Lib $(PROJ).Res

# Help Files
$(PROJ).Hlp: $(PROJ).Hpj $(PROJ).Rtf

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul
    @del *.Dll 2> nul
    @del *.Hlp 2> nul

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    @del *.Obj 2> nul
    @del *.Lib 2> nul
    @del *.Exp 2> nul
    @del *.Res 2> nul
    @del *.Ph  2> nul
