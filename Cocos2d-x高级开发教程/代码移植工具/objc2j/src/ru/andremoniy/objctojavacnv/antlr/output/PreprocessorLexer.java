// $ANTLR 3.4 C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g 2012-11-04 23:26:11

package ru.andremoniy.objctojavacnv.antlr.output;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked"})
public class PreprocessorLexer extends Lexer {
    public static final int EOF=-1;
    public static final int T__60=60;
    public static final int T__61=61;
    public static final int T__62=62;
    public static final int T__63=63;
    public static final int T__64=64;
    public static final int T__65=65;
    public static final int T__66=66;
    public static final int T__67=67;
    public static final int T__68=68;
    public static final int ASTERISK=4;
    public static final int BACKSLASH=5;
    public static final int COMMENT=6;
    public static final int DEFINE=7;
    public static final int DEFINED=8;
    public static final int DIGIT=9;
    public static final int ELIF=10;
    public static final int ELSE=11;
    public static final int ENDIF=12;
    public static final int EXC=13;
    public static final int ID=14;
    public static final int IF=15;
    public static final int IFDEF=16;
    public static final int IFNDEF=17;
    public static final int IMPORT=18;
    public static final int INCLUDE=19;
    public static final int LETTER=20;
    public static final int L_BR=21;
    public static final int L_UBR=22;
    public static final int MINUS=23;
    public static final int NUMBER=24;
    public static final int PLUS=25;
    public static final int POINTS3=26;
    public static final int QUOTE=27;
    public static final int RET=28;
    public static final int R_BR=29;
    public static final int R_UBR=30;
    public static final int SINGLE_COMMENT=31;
    public static final int SPECIAL_CHARS=32;
    public static final int T_BACKSLASH=33;
    public static final int T_BLOCK=34;
    public static final int T_COND_OP=35;
    public static final int T_DEFINE=36;
    public static final int T_EXPRESSION=37;
    public static final int T_EX_AND=38;
    public static final int T_EX_COND=39;
    public static final int T_EX_DEF=40;
    public static final int T_EX_MATH=41;
    public static final int T_EX_NOT=42;
    public static final int T_EX_OP=43;
    public static final int T_EX_OR=44;
    public static final int T_EX_SNGL=45;
    public static final int T_ID=46;
    public static final int T_IF_DEFINE=47;
    public static final int T_IF_ELSE=48;
    public static final int T_IF_MAIN=49;
    public static final int T_IF_NOT_DEFINE=50;
    public static final int T_IF_SIMPLE=51;
    public static final int T_IMPORT=52;
    public static final int T_INCLUDE=53;
    public static final int T_IN_BRACKETS=54;
    public static final int T_NAME=55;
    public static final int T_PARAM=56;
    public static final int T_PATH=57;
    public static final int T_REPLACE=58;
    public static final int WS=59;

    // delegates
    // delegators
    public Lexer[] getDelegates() {
        return new Lexer[] {};
    }

    public PreprocessorLexer() {} 
    public PreprocessorLexer(CharStream input) {
        this(input, new RecognizerSharedState());
    }
    public PreprocessorLexer(CharStream input, RecognizerSharedState state) {
        super(input,state);
    }
    public String getGrammarFileName() { return "C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g"; }

    // $ANTLR start "T__60"
    public final void mT__60() throws RecognitionException {
        try {
            int _type = T__60;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:6:7: ( '&&' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:6:9: '&&'
            {
            match("&&"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__60"

    // $ANTLR start "T__61"
    public final void mT__61() throws RecognitionException {
        try {
            int _type = T__61;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:7:7: ( ',' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:7:9: ','
            {
            match(','); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__61"

    // $ANTLR start "T__62"
    public final void mT__62() throws RecognitionException {
        try {
            int _type = T__62;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:8:7: ( '.' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:8:9: '.'
            {
            match('.'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__62"

    // $ANTLR start "T__63"
    public final void mT__63() throws RecognitionException {
        try {
            int _type = T__63;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:9:7: ( '/' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:9:9: '/'
            {
            match('/'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__63"

    // $ANTLR start "T__64"
    public final void mT__64() throws RecognitionException {
        try {
            int _type = T__64;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:10:7: ( '<=' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:10:9: '<='
            {
            match("<="); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__64"

    // $ANTLR start "T__65"
    public final void mT__65() throws RecognitionException {
        try {
            int _type = T__65;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:11:7: ( '==' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:11:9: '=='
            {
            match("=="); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__65"

    // $ANTLR start "T__66"
    public final void mT__66() throws RecognitionException {
        try {
            int _type = T__66;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:12:7: ( '>=' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:12:9: '>='
            {
            match(">="); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__66"

    // $ANTLR start "T__67"
    public final void mT__67() throws RecognitionException {
        try {
            int _type = T__67;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:13:7: ( '\\'#\\'' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:13:9: '\\'#\\''
            {
            match("'#'"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__67"

    // $ANTLR start "T__68"
    public final void mT__68() throws RecognitionException {
        try {
            int _type = T__68;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:14:7: ( '||' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:14:9: '||'
            {
            match("||"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "T__68"

    // $ANTLR start "COMMENT"
    public final void mCOMMENT() throws RecognitionException {
        try {
            int _type = COMMENT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:241:9: ( '/*' ( options {greedy=false; } : . )* '*/' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:241:12: '/*' ( options {greedy=false; } : . )* '*/'
            {
            match("/*"); 



            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:241:17: ( options {greedy=false; } : . )*
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( (LA1_0=='*') ) {
                    int LA1_1 = input.LA(2);

                    if ( (LA1_1=='/') ) {
                        alt1=2;
                    }
                    else if ( ((LA1_1 >= '\u0000' && LA1_1 <= '.')||(LA1_1 >= '0' && LA1_1 <= '\uFFFF')) ) {
                        alt1=1;
                    }


                }
                else if ( ((LA1_0 >= '\u0000' && LA1_0 <= ')')||(LA1_0 >= '+' && LA1_0 <= '\uFFFF')) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:241:45: .
            	    {
            	    matchAny(); 

            	    }
            	    break;

            	default :
            	    break loop1;
                }
            } while (true);


            match("*/"); 



              _channel = HIDDEN;  

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "COMMENT"

    // $ANTLR start "SINGLE_COMMENT"
    public final void mSINGLE_COMMENT() throws RecognitionException {
        try {
            int _type = SINGLE_COMMENT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:244:2: ( '//' (~ ( '\\n' | '\\r' ) )* )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:244:4: '//' (~ ( '\\n' | '\\r' ) )*
            {
            match("//"); 



            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:244:9: (~ ( '\\n' | '\\r' ) )*
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( ((LA2_0 >= '\u0000' && LA2_0 <= '\t')||(LA2_0 >= '\u000B' && LA2_0 <= '\f')||(LA2_0 >= '\u000E' && LA2_0 <= '\uFFFF')) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            	    {
            	    if ( (input.LA(1) >= '\u0000' && input.LA(1) <= '\t')||(input.LA(1) >= '\u000B' && input.LA(1) <= '\f')||(input.LA(1) >= '\u000E' && input.LA(1) <= '\uFFFF') ) {
            	        input.consume();
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        recover(mse);
            	        throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    break loop2;
                }
            } while (true);


              _channel = HIDDEN;  

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "SINGLE_COMMENT"

    // $ANTLR start "IFNDEF"
    public final void mIFNDEF() throws RecognitionException {
        try {
            int _type = IFNDEF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:246:8: ( '#ifndef' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:246:10: '#ifndef'
            {
            match("#ifndef"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "IFNDEF"

    // $ANTLR start "IF"
    public final void mIF() throws RecognitionException {
        try {
            int _type = IF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:247:4: ( '#if' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:247:6: '#if'
            {
            match("#if"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "IF"

    // $ANTLR start "ELIF"
    public final void mELIF() throws RecognitionException {
        try {
            int _type = ELIF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:248:6: ( '#elif' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:248:8: '#elif'
            {
            match("#elif"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "ELIF"

    // $ANTLR start "IFDEF"
    public final void mIFDEF() throws RecognitionException {
        try {
            int _type = IFDEF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:249:7: ( '#ifdef' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:249:9: '#ifdef'
            {
            match("#ifdef"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "IFDEF"

    // $ANTLR start "ELSE"
    public final void mELSE() throws RecognitionException {
        try {
            int _type = ELSE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:250:6: ( '#else' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:250:8: '#else'
            {
            match("#else"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "ELSE"

    // $ANTLR start "ENDIF"
    public final void mENDIF() throws RecognitionException {
        try {
            int _type = ENDIF;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:251:7: ( '#endif' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:251:9: '#endif'
            {
            match("#endif"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "ENDIF"

    // $ANTLR start "DEFINE"
    public final void mDEFINE() throws RecognitionException {
        try {
            int _type = DEFINE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:252:8: ( '#define' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:252:10: '#define'
            {
            match("#define"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "DEFINE"

    // $ANTLR start "INCLUDE"
    public final void mINCLUDE() throws RecognitionException {
        try {
            int _type = INCLUDE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:253:9: ( '#include' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:253:11: '#include'
            {
            match("#include"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "INCLUDE"

    // $ANTLR start "IMPORT"
    public final void mIMPORT() throws RecognitionException {
        try {
            int _type = IMPORT;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:254:8: ( '#import' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:254:10: '#import'
            {
            match("#import"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "IMPORT"

    // $ANTLR start "DEFINED"
    public final void mDEFINED() throws RecognitionException {
        try {
            int _type = DEFINED;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:255:9: ( 'defined' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:255:11: 'defined'
            {
            match("defined"); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "DEFINED"

    // $ANTLR start "BACKSLASH"
    public final void mBACKSLASH() throws RecognitionException {
        try {
            int _type = BACKSLASH;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:258:2: ( '\\\\' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:258:4: '\\\\'
            {
            match('\\'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "BACKSLASH"

    // $ANTLR start "POINTS3"
    public final void mPOINTS3() throws RecognitionException {
        try {
            int _type = POINTS3;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:259:9: ( '...' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:259:11: '...'
            {
            match("..."); 



            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "POINTS3"

    // $ANTLR start "EXC"
    public final void mEXC() throws RecognitionException {
        try {
            int _type = EXC;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:261:5: ( '!' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:261:7: '!'
            {
            match('!'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "EXC"

    // $ANTLR start "PLUS"
    public final void mPLUS() throws RecognitionException {
        try {
            int _type = PLUS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:262:6: ( '+' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:262:8: '+'
            {
            match('+'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "PLUS"

    // $ANTLR start "MINUS"
    public final void mMINUS() throws RecognitionException {
        try {
            int _type = MINUS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:263:7: ( '-' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:263:9: '-'
            {
            match('-'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "MINUS"

    // $ANTLR start "L_BR"
    public final void mL_BR() throws RecognitionException {
        try {
            int _type = L_BR;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:264:6: ( '(' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:264:8: '('
            {
            match('('); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "L_BR"

    // $ANTLR start "R_BR"
    public final void mR_BR() throws RecognitionException {
        try {
            int _type = R_BR;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:265:6: ( ')' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:265:8: ')'
            {
            match(')'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "R_BR"

    // $ANTLR start "L_UBR"
    public final void mL_UBR() throws RecognitionException {
        try {
            int _type = L_UBR;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:266:7: ( '<' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:266:9: '<'
            {
            match('<'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "L_UBR"

    // $ANTLR start "R_UBR"
    public final void mR_UBR() throws RecognitionException {
        try {
            int _type = R_UBR;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:267:7: ( '>' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:267:9: '>'
            {
            match('>'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "R_UBR"

    // $ANTLR start "ASTERISK"
    public final void mASTERISK() throws RecognitionException {
        try {
            int _type = ASTERISK;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:268:9: ( '*' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:268:11: '*'
            {
            match('*'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "ASTERISK"

    // $ANTLR start "QUOTE"
    public final void mQUOTE() throws RecognitionException {
        try {
            int _type = QUOTE;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:269:7: ( '\"' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:269:9: '\"'
            {
            match('\"'); 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "QUOTE"

    // $ANTLR start "RET"
    public final void mRET() throws RecognitionException {
        try {
            int _type = RET;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:271:5: ( '\\r' | '\\n' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            {
            if ( input.LA(1)=='\n'||input.LA(1)=='\r' ) {
                input.consume();
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;
            }


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "RET"

    // $ANTLR start "ID"
    public final void mID() throws RecognitionException {
        try {
            int _type = ID;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:273:4: ( LETTER ( DIGIT | LETTER )* )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:273:6: LETTER ( DIGIT | LETTER )*
            {
            mLETTER(); 


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:273:13: ( DIGIT | LETTER )*
            loop3:
            do {
                int alt3=2;
                int LA3_0 = input.LA(1);

                if ( ((LA3_0 >= '0' && LA3_0 <= '9')||(LA3_0 >= 'A' && LA3_0 <= 'Z')||LA3_0=='_'||(LA3_0 >= 'a' && LA3_0 <= 'z')) ) {
                    alt3=1;
                }


                switch (alt3) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            	    {
            	    if ( (input.LA(1) >= '0' && input.LA(1) <= '9')||(input.LA(1) >= 'A' && input.LA(1) <= 'Z')||input.LA(1)=='_'||(input.LA(1) >= 'a' && input.LA(1) <= 'z') ) {
            	        input.consume();
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        recover(mse);
            	        throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    break loop3;
                }
            } while (true);


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "ID"

    // $ANTLR start "NUMBER"
    public final void mNUMBER() throws RecognitionException {
        try {
            int _type = NUMBER;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:275:9: ( DIGIT ( DIGIT | '.' )* ( 'L' )? )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:275:12: DIGIT ( DIGIT | '.' )* ( 'L' )?
            {
            mDIGIT(); 


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:275:18: ( DIGIT | '.' )*
            loop4:
            do {
                int alt4=2;
                int LA4_0 = input.LA(1);

                if ( (LA4_0=='.'||(LA4_0 >= '0' && LA4_0 <= '9')) ) {
                    alt4=1;
                }


                switch (alt4) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            	    {
            	    if ( input.LA(1)=='.'||(input.LA(1) >= '0' && input.LA(1) <= '9') ) {
            	        input.consume();
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        recover(mse);
            	        throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    break loop4;
                }
            } while (true);


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:275:33: ( 'L' )?
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0=='L') ) {
                alt5=1;
            }
            switch (alt5) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:275:33: 'L'
                    {
                    match('L'); 

                    }
                    break;

            }


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "NUMBER"

    // $ANTLR start "SPECIAL_CHARS"
    public final void mSPECIAL_CHARS() throws RecognitionException {
        try {
            int _type = SPECIAL_CHARS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:283:2: ( '%' | '?' | '|' | '&' | ';' | '[' | ']' | '{' | '}' | '@' | ':' | '/' | '#' | '=' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            {
            if ( input.LA(1)=='#'||(input.LA(1) >= '%' && input.LA(1) <= '&')||input.LA(1)=='/'||(input.LA(1) >= ':' && input.LA(1) <= ';')||input.LA(1)=='='||(input.LA(1) >= '?' && input.LA(1) <= '@')||input.LA(1)=='['||input.LA(1)==']'||(input.LA(1) >= '{' && input.LA(1) <= '}') ) {
                input.consume();
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;
            }


            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "SPECIAL_CHARS"

    // $ANTLR start "WS"
    public final void mWS() throws RecognitionException {
        try {
            int _type = WS;
            int _channel = DEFAULT_TOKEN_CHANNEL;
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:285:6: ( ( '\\u000C' | ' ' | '\\t' )+ )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:285:8: ( '\\u000C' | ' ' | '\\t' )+
            {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:285:8: ( '\\u000C' | ' ' | '\\t' )+
            int cnt6=0;
            loop6:
            do {
                int alt6=2;
                int LA6_0 = input.LA(1);

                if ( (LA6_0=='\t'||LA6_0=='\f'||LA6_0==' ') ) {
                    alt6=1;
                }


                switch (alt6) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            	    {
            	    if ( input.LA(1)=='\t'||input.LA(1)=='\f'||input.LA(1)==' ' ) {
            	        input.consume();
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        recover(mse);
            	        throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    if ( cnt6 >= 1 ) break loop6;
                        EarlyExitException eee =
                            new EarlyExitException(6, input);
                        throw eee;
                }
                cnt6++;
            } while (true);


             _channel = HIDDEN; 

            }

            state.type = _type;
            state.channel = _channel;
        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "WS"

    // $ANTLR start "DIGIT"
    public final void mDIGIT() throws RecognitionException {
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:287:17: ( '0' .. '9' )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            {
            if ( (input.LA(1) >= '0' && input.LA(1) <= '9') ) {
                input.consume();
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;
            }


            }


        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "DIGIT"

    // $ANTLR start "LETTER"
    public final void mLETTER() throws RecognitionException {
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:288:17: ( ( 'a' .. 'z' | 'A' .. 'Z' | '_' ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            {
            if ( (input.LA(1) >= 'A' && input.LA(1) <= 'Z')||input.LA(1)=='_'||(input.LA(1) >= 'a' && input.LA(1) <= 'z') ) {
                input.consume();
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                recover(mse);
                throw mse;
            }


            }


        }
        finally {
        	// do for sure before leaving
        }
    }
    // $ANTLR end "LETTER"

    public void mTokens() throws RecognitionException {
        // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:8: ( T__60 | T__61 | T__62 | T__63 | T__64 | T__65 | T__66 | T__67 | T__68 | COMMENT | SINGLE_COMMENT | IFNDEF | IF | ELIF | IFDEF | ELSE | ENDIF | DEFINE | INCLUDE | IMPORT | DEFINED | BACKSLASH | POINTS3 | EXC | PLUS | MINUS | L_BR | R_BR | L_UBR | R_UBR | ASTERISK | QUOTE | RET | ID | NUMBER | SPECIAL_CHARS | WS )
        int alt7=37;
        switch ( input.LA(1) ) {
        case '&':
            {
            int LA7_1 = input.LA(2);

            if ( (LA7_1=='&') ) {
                alt7=1;
            }
            else {
                alt7=36;
            }
            }
            break;
        case ',':
            {
            alt7=2;
            }
            break;
        case '.':
            {
            int LA7_3 = input.LA(2);

            if ( (LA7_3=='.') ) {
                alt7=23;
            }
            else {
                alt7=3;
            }
            }
            break;
        case '/':
            {
            switch ( input.LA(2) ) {
            case '*':
                {
                alt7=10;
                }
                break;
            case '/':
                {
                alt7=11;
                }
                break;
            default:
                alt7=4;
            }

            }
            break;
        case '<':
            {
            int LA7_5 = input.LA(2);

            if ( (LA7_5=='=') ) {
                alt7=5;
            }
            else {
                alt7=29;
            }
            }
            break;
        case '=':
            {
            int LA7_6 = input.LA(2);

            if ( (LA7_6=='=') ) {
                alt7=6;
            }
            else {
                alt7=36;
            }
            }
            break;
        case '>':
            {
            int LA7_7 = input.LA(2);

            if ( (LA7_7=='=') ) {
                alt7=7;
            }
            else {
                alt7=30;
            }
            }
            break;
        case '\'':
            {
            alt7=8;
            }
            break;
        case '|':
            {
            int LA7_9 = input.LA(2);

            if ( (LA7_9=='|') ) {
                alt7=9;
            }
            else {
                alt7=36;
            }
            }
            break;
        case '#':
            {
            switch ( input.LA(2) ) {
            case 'i':
                {
                switch ( input.LA(3) ) {
                case 'f':
                    {
                    switch ( input.LA(4) ) {
                    case 'n':
                        {
                        alt7=12;
                        }
                        break;
                    case 'd':
                        {
                        alt7=15;
                        }
                        break;
                    default:
                        alt7=13;
                    }

                    }
                    break;
                case 'n':
                    {
                    alt7=19;
                    }
                    break;
                case 'm':
                    {
                    alt7=20;
                    }
                    break;
                default:
                    NoViableAltException nvae =
                        new NoViableAltException("", 7, 37, input);

                    throw nvae;

                }

                }
                break;
            case 'e':
                {
                int LA7_38 = input.LA(3);

                if ( (LA7_38=='l') ) {
                    int LA7_44 = input.LA(4);

                    if ( (LA7_44=='i') ) {
                        alt7=14;
                    }
                    else if ( (LA7_44=='s') ) {
                        alt7=16;
                    }
                    else {
                        NoViableAltException nvae =
                            new NoViableAltException("", 7, 44, input);

                        throw nvae;

                    }
                }
                else if ( (LA7_38=='n') ) {
                    alt7=17;
                }
                else {
                    NoViableAltException nvae =
                        new NoViableAltException("", 7, 38, input);

                    throw nvae;

                }
                }
                break;
            case 'd':
                {
                alt7=18;
                }
                break;
            default:
                alt7=36;
            }

            }
            break;
        case 'd':
            {
            int LA7_11 = input.LA(2);

            if ( (LA7_11=='e') ) {
                int LA7_40 = input.LA(3);

                if ( (LA7_40=='f') ) {
                    int LA7_46 = input.LA(4);

                    if ( (LA7_46=='i') ) {
                        int LA7_52 = input.LA(5);

                        if ( (LA7_52=='n') ) {
                            int LA7_53 = input.LA(6);

                            if ( (LA7_53=='e') ) {
                                int LA7_54 = input.LA(7);

                                if ( (LA7_54=='d') ) {
                                    int LA7_55 = input.LA(8);

                                    if ( ((LA7_55 >= '0' && LA7_55 <= '9')||(LA7_55 >= 'A' && LA7_55 <= 'Z')||LA7_55=='_'||(LA7_55 >= 'a' && LA7_55 <= 'z')) ) {
                                        alt7=34;
                                    }
                                    else {
                                        alt7=21;
                                    }
                                }
                                else {
                                    alt7=34;
                                }
                            }
                            else {
                                alt7=34;
                            }
                        }
                        else {
                            alt7=34;
                        }
                    }
                    else {
                        alt7=34;
                    }
                }
                else {
                    alt7=34;
                }
            }
            else {
                alt7=34;
            }
            }
            break;
        case '\\':
            {
            alt7=22;
            }
            break;
        case '!':
            {
            alt7=24;
            }
            break;
        case '+':
            {
            alt7=25;
            }
            break;
        case '-':
            {
            alt7=26;
            }
            break;
        case '(':
            {
            alt7=27;
            }
            break;
        case ')':
            {
            alt7=28;
            }
            break;
        case '*':
            {
            alt7=31;
            }
            break;
        case '\"':
            {
            alt7=32;
            }
            break;
        case '\n':
        case '\r':
            {
            alt7=33;
            }
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
        case '_':
        case 'a':
        case 'b':
        case 'c':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            {
            alt7=34;
            }
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            {
            alt7=35;
            }
            break;
        case '%':
        case ':':
        case ';':
        case '?':
        case '@':
        case '[':
        case ']':
        case '{':
        case '}':
            {
            alt7=36;
            }
            break;
        case '\t':
        case '\f':
        case ' ':
            {
            alt7=37;
            }
            break;
        default:
            NoViableAltException nvae =
                new NoViableAltException("", 7, 0, input);

            throw nvae;

        }

        switch (alt7) {
            case 1 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:10: T__60
                {
                mT__60(); 


                }
                break;
            case 2 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:16: T__61
                {
                mT__61(); 


                }
                break;
            case 3 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:22: T__62
                {
                mT__62(); 


                }
                break;
            case 4 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:28: T__63
                {
                mT__63(); 


                }
                break;
            case 5 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:34: T__64
                {
                mT__64(); 


                }
                break;
            case 6 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:40: T__65
                {
                mT__65(); 


                }
                break;
            case 7 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:46: T__66
                {
                mT__66(); 


                }
                break;
            case 8 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:52: T__67
                {
                mT__67(); 


                }
                break;
            case 9 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:58: T__68
                {
                mT__68(); 


                }
                break;
            case 10 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:64: COMMENT
                {
                mCOMMENT(); 


                }
                break;
            case 11 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:72: SINGLE_COMMENT
                {
                mSINGLE_COMMENT(); 


                }
                break;
            case 12 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:87: IFNDEF
                {
                mIFNDEF(); 


                }
                break;
            case 13 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:94: IF
                {
                mIF(); 


                }
                break;
            case 14 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:97: ELIF
                {
                mELIF(); 


                }
                break;
            case 15 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:102: IFDEF
                {
                mIFDEF(); 


                }
                break;
            case 16 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:108: ELSE
                {
                mELSE(); 


                }
                break;
            case 17 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:113: ENDIF
                {
                mENDIF(); 


                }
                break;
            case 18 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:119: DEFINE
                {
                mDEFINE(); 


                }
                break;
            case 19 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:126: INCLUDE
                {
                mINCLUDE(); 


                }
                break;
            case 20 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:134: IMPORT
                {
                mIMPORT(); 


                }
                break;
            case 21 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:141: DEFINED
                {
                mDEFINED(); 


                }
                break;
            case 22 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:149: BACKSLASH
                {
                mBACKSLASH(); 


                }
                break;
            case 23 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:159: POINTS3
                {
                mPOINTS3(); 


                }
                break;
            case 24 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:167: EXC
                {
                mEXC(); 


                }
                break;
            case 25 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:171: PLUS
                {
                mPLUS(); 


                }
                break;
            case 26 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:176: MINUS
                {
                mMINUS(); 


                }
                break;
            case 27 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:182: L_BR
                {
                mL_BR(); 


                }
                break;
            case 28 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:187: R_BR
                {
                mR_BR(); 


                }
                break;
            case 29 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:192: L_UBR
                {
                mL_UBR(); 


                }
                break;
            case 30 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:198: R_UBR
                {
                mR_UBR(); 


                }
                break;
            case 31 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:204: ASTERISK
                {
                mASTERISK(); 


                }
                break;
            case 32 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:213: QUOTE
                {
                mQUOTE(); 


                }
                break;
            case 33 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:219: RET
                {
                mRET(); 


                }
                break;
            case 34 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:223: ID
                {
                mID(); 


                }
                break;
            case 35 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:226: NUMBER
                {
                mNUMBER(); 


                }
                break;
            case 36 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:233: SPECIAL_CHARS
                {
                mSPECIAL_CHARS(); 


                }
                break;
            case 37 :
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:1:247: WS
                {
                mWS(); 


                }
                break;

        }

    }


 

}