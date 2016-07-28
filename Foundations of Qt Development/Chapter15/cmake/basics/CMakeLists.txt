#
# Copyright (c) 2006-2007, Johan Thelin
# 
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification, 
# are permitted provided that the following conditions are met:
# 
#     * Redistributions of source code must retain the above copyright notice, 
#       this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice,  
#       this list of conditions and the following disclaimer in the documentation 
#       and/or other materials provided with the distribution.
#     * Neither the name of APress nor the names of its contributors 
#       may be used to endorse or promote products derived from this software 
#       without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

PROJECT( basics )

SET( basics_SOURCES main.cpp mainwindow.cpp otherdialog.cpp preferencedialog.cpp )
SET( basics_HEADERS mainwindow.h otherdialog.h preferencedialog.h )
SET( basics_FORMS otherdialog.ui preferencedialog.ui )

FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

QT4_WRAP_CPP( basics_HEADERS_MOC ${basics_HEADERS} )
QT4_WRAP_UI( basics_FORMS_HEADERS ${basics_FORMS} )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

ADD_DEFINITIONS( ${QT_DEFINITIONS} )

ADD_EXECUTABLE( basics ${basics_SOURCES} ${basics_HEADERS_MOC} ${basics_FORMS_HEADERS} )
TARGET_LINK_LIBRARIES( basics ${QT_LIBRARIES} )
