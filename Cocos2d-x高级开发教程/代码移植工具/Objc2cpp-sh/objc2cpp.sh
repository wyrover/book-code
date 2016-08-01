#!/usr/bin/env bash

# WARNING:
# WARNING: This script may destroy your code, make sure to backup before running.
# WARNING:

# Using Vim substitutions from this forum post
# http://www.cocos2d-x.org/boards/6/topics/6358

# Best Practice for Converting objc to c++
# http://cocos2d-x.org/projects/cocos2d-x/wiki/Rules_of_translating_objc_to_cpp

################################################################
# Find/Replace using sed to convert obj-c syntax to c++
################################################################

if [ -z "$1" ];
then
    echo "Usage: $0 [filename]"
    exit
fi

if [ ! -f $1 ];
then
    echo "File $1 doesn't exist."
    exit
else
    f="$1"
    echo "replacing objc to cpp in file $f"

    # Create a separate output file instead of overwriting
    # note: you won't be able to run multiple times to replace all nested brackets
    # comment out if you want to make substitutions "in-place"
    #cp $f "$f.new"
    #f="$f.new"

    # GENERAL
    sed -E 's/@"/"/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/import/include/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/self/this/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/YES/true/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/BOOL/bool/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/NO/false/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/nil/NULL/g' $f > "$f.tmp"; cp "$f.tmp" $f

    # MESSAGE CALLS
    sed -E 's/\[([^]\[]+)]/--*\1=--/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*([:upper:][:upper:]?.+) ([^:]*):([^:]+) [^:]*:([^:]+) [^:]*:([^:]+) [^:]*:([^:]+) [^:]*:([^:][^=]*)\=--/\1::\2(\3,\4,\5,\6,\7)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*(.+) ([^:]*):([^:]+) [^:]*:([^:]+) [^:]*:([^:]+) [^:]*:([^:]+) [^:]*:([^:][^=]*)\=--/\1->\2(\3,\4,\5,\6,\7)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*([:upper:][:upper:]?.+) ([^:]*):([^:]+) [^:]*:([^:]+) [^:]*:([^:]+) [^:]*:([^:][^=]*)\=--/\1::\2(\3,\4,\5,\6)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*(.+) ([^:]*):([^:]+) [^:]*:([^:]+) [^:]*:([^:]+) [^:]*:([^:][^=]*)\=--/\1->\2(\3,\4,\5,\6)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*([:upper:][:upper:]?.+) ([^:]*):([^:]+) [^:]*:([^:]+) [^:]*:([^:][^=]*)\=--/\1::\2(\3,\4,\5)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*(.+) ([^:]*):([^:]+) [^:]*:([^:]+) [^:]*:([^:][^=]*)\=--/\1->\2(\3,\4,\5)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*([:upper:][:upper:]?.+) ([^:]*):([^:]+) [^:]*:([^:][^=]*)\=--/\1::\2(\3,\4)/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*(.+) ([^:]*):([^:]+) [^:]*:([^:][^=]*)\=--/\1->\2(\3,\4)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*([:upper:][:upper:]?.+) ([^:]*):([^=]*)\=--/\1::\2(\3)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*(.+) ([^:]*):([^=]*)\=--/\1->\2(\3)/g ' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*([:upper:][:upper:]?.+) ([^=:]+)\=--/\1::\2()/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/--\*(.+) ([^=]*)\=--/\1->\2()/g' $f > "$f.tmp"; cp "$f.tmp" $f


    ####################################################################################
    # CLASS INTERFACE & IMPLEMENTATION
    #
    # note: this currently creates _ delimited function names so
    #
    #   @implementation IntroLayer
    #   @interface IntroLayer
    #   @interface IntroLayer : SCLayer
    #
    # becomes:
    #
    #   __COMMENT_OUT__
    #   class HelloWorld
    #   class HelloWorld : public cocos2d::CCLayer
    #
    ####################################################################################

    sed -E 's/@implementation[ ]+([^ :;{]+)/\/\/ Implementation \1/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/@interface[ ]+([^ :;{]+)[ ]*:[ ]*([^) :;{]+)/class \1 : public cocos2d::\2/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/@interface[ ]+([^ :;{]+)/class \1/g' $f > "$f.tmp"; cp "$f.tmp" $f

    ####################################################################################
    # METHOD DECLARATIONS
    #
    # note: this currently creates _ delimited function names so
    #
    #   -(CCSprite*)myMethod:(int)arg1 Another:(float)arg2 {
    #
    # becomes:
    #
    #   CCSprite* myMethod_Another(int arg1, float arg2) {
    #
    ####################################################################################

    # `sed` can only support up to 9 back references (maybe use multiple passes)
    #sed -E 's/^[ ]*-[ ]*\(([^)]+)\)[ ]*([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^ :;{]+)[ ]+([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^:;{]+)[ ]+([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^:;{]+)[ ]+([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^:;{]+)/\1 \2_\5_\8_\11(\3 \4, \6 \7, \9 \10, \12 \13)/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/^[ ]*-[ ]*\(([^)]+)\)[ ]*([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^ :;{]+)[ ]+([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^:;{]+)[ ]+([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^:;{]+)/\1 \2_\5_\8(\3 \4, \6 \7, \9 \10)/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/^[ ]*-[ ]*\(([^)]+)\)[ ]*([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^ :;{]+)[ ]+([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^:;{]+)/\1 \2_\5(\3 \4, \6 \7)/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/^[ ]*-[ ]*\(([^)]+)\)[ ]*([^ :;{]+)[ ]*:[ ]*\(([^)]+)\)[ ]*([^ :;{]+)/\1 \2(\3 \4)/g' $f > "$f.tmp"; cp "$f.tmp" $f
    sed -E 's/^[ ]*-[ ]*\(([^)]+)\)[ ]*([^ :;{]+)/\1 \2()/g' $f > "$f.tmp"; cp "$f.tmp" $f


    ####################################################################################
    # HANDLE id
    #
    # *Note: disabled by default, you may want to enable depending on porting preferences
    #
    # Sometimes id should be bool for Return Value
    # - [nil autorelease] is allowed, null->method() is ERROR
    #
    # Sometimes id is used as an ANY_OBJECT pointer
    # - id action = [CCAction action] => void* action = CCAction->action();
    #
    ####################################################################################

    # id to bool for Return Value (better practice)
    #sed -E 's/\( *id *\)/(bool)/g ' $f > "$f.tmp"; cp "$f.tmp" $f

    # id to void* for return value (direct translate)
    #sed -E 's/\( *id *\)/(void*)/g ' $f > "$f.tmp"; cp "$f.tmp" $f

    # id to void* for object pointer as param or assignment
    #sed -E 's/\(id[ ]+/(void* /g ' $f > "$f.tmp"; cp "$f.tmp" $f
    #sed -E 's/id[ ]*=/void* =/g ' $f > "$f.tmp"; cp "$f.tmp" $f




# CGFloat => GLFloat

# CGPointMake => cocos2d::CCPointMake
# CGSizeMake => cocos2d::CCSizeMake
# CGRectMake => cocos2d::CCRectMake
# CGPoint => cocos2d::CCPoint
# CGSize => cocos2d::CCSize
# CGRect => cocos2d::CCRect

# NSMutableArray => cocos2d::CCArray
# NSMutableDictionary => cocos2d::CCDictionary
# NSString => cocos2d::CCString



# REMOVE SYNTAX
# @class ...
# @end

    rm "$f.tmp"
fi