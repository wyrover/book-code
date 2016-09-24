_LNG=$(LANGUAGE)
.SUFFIXES: .inx
STAMP=stampinf

# $(OBJ_PATH)\$(O)\$(INF_NAME).inf: $(_INX)\$(INF_NAME).inx
.inx{$(OBJ_PATH)\$(O)}.inf:
    copy $(@B).inx $@
    $(STAMP)  -f $@ -a $(_BUILDARCH) -k $(KMDF_VERSION_MAJOR).$(KMDF_VERSION_MINOR) -u $(UMDF_VERSION_MAJOR).$(UMDF_VERSION_MINOR).0



