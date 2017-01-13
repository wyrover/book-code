a::Pr:
Private
a::Pu:
Public
a::E:
Else
a::D:
Dim
a::If:
If $end$ Then
End If
a::Function:
Function $end$ ( )
End Function
a::Sub:
Sub $end$ ( )
End Sub
a::Do:
Do While $end$
Loop
a::For:
For $end$ To 
Next
a:File header::
'*************************************************************************
'	created:	$DATE$
'	created:	$DAY$:$MONTH$:$YEAR$   $HOUR$:$MINUTE$
'	filename: 	$FILE$
'	file path:	$FILE_PATH$
'	file base:	$FILE_BASE$
'	file ext:	$FILE_EXT$
'	author:		$1$
'	
'	purpose:	$end$
'*************************************************************************


readme:
VA Snippet used for suggestions in class definitions.
Delete this item to restore the default upon next use.

a:SuggestionsForType class::
Public
Private
Protected


readme:
VA Snippet used for suggestions of type Boolean.
Delete this item to restore the default upon next use.

a:SuggestionsForType Boolean::
True
False


readme:
VA Snippet used for refactoring.
Delete this item to restore the default upon next use.

a:Refactor Create Implementation::
$SymbolPrivileges$ Sub $SymbolName$( $ParameterList$ )
	$end$$MethodBody$
End Sub


readme:
VA Snippet used for refactoring.
Delete this item to restore the default upon next use.

a:Refactor Document Method::
 
'//////////////////////////////////////////////////
' Method:    $SymbolName$
' FullName:  $SymbolContext$
' Access:    $SymbolVirtual$$SymbolPrivileges$$SymbolStatic$
' Returns:   $SymbolType$
' Parameter: $MethodArg$
'//////////////////////////////////////////////////


readme:
VA Snippet used for refactoring.
Delete this item to restore the default upon next use.

a:Refactor Encapsulate Field::
	$end$Public Property $GeneratedPropertyName$Property() As $SymbolType$
		Get
			Return $SymbolName$
		End Get
		Set (ByVal value As $SymbolType$)
			$SymbolName$ = value
		End Set
	End Property


readme:
VA Snippet used for refactoring.
Delete this item to restore the default upon next use.

a:Refactor Extract Method::

$end$$SymbolPrivileges$ Sub $SymbolName$( $ParameterList$ )
	$MethodBody$
End Sub


readme:
VA Snippet used by Surround With #region.
Delete this item to restore the default upon next use.

a:#region (VA):#r:
#Region "$end$"
$selected$
#End Region


readme:
Delete this item to restore the default when the IDE starts.

a:(...)::
($selected$)
