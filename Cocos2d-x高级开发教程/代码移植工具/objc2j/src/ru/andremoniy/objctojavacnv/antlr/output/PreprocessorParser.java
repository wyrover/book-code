// $ANTLR 3.4 C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g 2012-11-04 23:26:11

package ru.andremoniy.objctojavacnv.antlr.output;


import org.antlr.runtime.*;
import java.util.Stack;
import java.util.List;
import java.util.ArrayList;

import org.antlr.runtime.tree.*;


@SuppressWarnings({"all", "warnings", "unchecked"})
public class PreprocessorParser extends Parser {
    public static final String[] tokenNames = new String[] {
        "<invalid>", "<EOR>", "<DOWN>", "<UP>", "ASTERISK", "BACKSLASH", "COMMENT", "DEFINE", "DEFINED", "DIGIT", "ELIF", "ELSE", "ENDIF", "EXC", "ID", "IF", "IFDEF", "IFNDEF", "IMPORT", "INCLUDE", "LETTER", "L_BR", "L_UBR", "MINUS", "NUMBER", "PLUS", "POINTS3", "QUOTE", "RET", "R_BR", "R_UBR", "SINGLE_COMMENT", "SPECIAL_CHARS", "T_BACKSLASH", "T_BLOCK", "T_COND_OP", "T_DEFINE", "T_EXPRESSION", "T_EX_AND", "T_EX_COND", "T_EX_DEF", "T_EX_MATH", "T_EX_NOT", "T_EX_OP", "T_EX_OR", "T_EX_SNGL", "T_ID", "T_IF_DEFINE", "T_IF_ELSE", "T_IF_MAIN", "T_IF_NOT_DEFINE", "T_IF_SIMPLE", "T_IMPORT", "T_INCLUDE", "T_IN_BRACKETS", "T_NAME", "T_PARAM", "T_PATH", "T_REPLACE", "WS", "'&&'", "','", "'.'", "'/'", "'<='", "'=='", "'>='", "'\\'#\\''", "'||'"
    };

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
    public Parser[] getDelegates() {
        return new Parser[] {};
    }

    // delegators


    public PreprocessorParser(TokenStream input) {
        this(input, new RecognizerSharedState());
    }
    public PreprocessorParser(TokenStream input, RecognizerSharedState state) {
        super(input, state);
    }

protected TreeAdaptor adaptor = new CommonTreeAdaptor();

public void setTreeAdaptor(TreeAdaptor adaptor) {
    this.adaptor = adaptor;
}
public TreeAdaptor getTreeAdaptor() {
    return adaptor;
}
    public String[] getTokenNames() { return PreprocessorParser.tokenNames; }
    public String getGrammarFileName() { return "C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g"; }


    public static class code_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "code"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:44:1: code : ( code_fragment )+ EOF ;
    public final PreprocessorParser.code_return code() throws RecognitionException {
        PreprocessorParser.code_return retval = new PreprocessorParser.code_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token EOF2=null;
        PreprocessorParser.code_fragment_return code_fragment1 =null;


        Object EOF2_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:44:6: ( ( code_fragment )+ EOF )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:44:8: ( code_fragment )+ EOF
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:44:8: ( code_fragment )+
            int cnt1=0;
            loop1:
            do {
                int alt1=2;
                int LA1_0 = input.LA(1);

                if ( ((LA1_0 >= ASTERISK && LA1_0 <= DIGIT)||(LA1_0 >= EXC && LA1_0 <= 68)) ) {
                    alt1=1;
                }


                switch (alt1) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:44:8: code_fragment
            	    {
            	    pushFollow(FOLLOW_code_fragment_in_code156);
            	    code_fragment1=code_fragment();

            	    state._fsp--;

            	    adaptor.addChild(root_0, code_fragment1.getTree());

            	    }
            	    break;

            	default :
            	    if ( cnt1 >= 1 ) break loop1;
                        EarlyExitException eee =
                            new EarlyExitException(1, input);
                        throw eee;
                }
                cnt1++;
            } while (true);


            EOF2=(Token)match(input,EOF,FOLLOW_EOF_in_code161); 
            EOF2_tree = 
            (Object)adaptor.create(EOF2)
            ;
            adaptor.addChild(root_0, EOF2_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "code"


    public static class smth_else_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "smth_else"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:48:1: smth_else : (~ ( DEFINE | IMPORT | IF | ELSE | ENDIF | IFDEF | IFNDEF | ELIF ) )+ ;
    public final PreprocessorParser.smth_else_return smth_else() throws RecognitionException {
        PreprocessorParser.smth_else_return retval = new PreprocessorParser.smth_else_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token set3=null;

        Object set3_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:49:2: ( (~ ( DEFINE | IMPORT | IF | ELSE | ENDIF | IFDEF | IFNDEF | ELIF ) )+ )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:49:4: (~ ( DEFINE | IMPORT | IF | ELSE | ENDIF | IFDEF | IFNDEF | ELIF ) )+
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:49:4: (~ ( DEFINE | IMPORT | IF | ELSE | ENDIF | IFDEF | IFNDEF | ELIF ) )+
            int cnt2=0;
            loop2:
            do {
                int alt2=2;
                int LA2_0 = input.LA(1);

                if ( (LA2_0==INCLUDE) ) {
                    alt2=1;
                }
                else if ( ((LA2_0 >= ASTERISK && LA2_0 <= COMMENT)||(LA2_0 >= DEFINED && LA2_0 <= DIGIT)||(LA2_0 >= EXC && LA2_0 <= ID)||(LA2_0 >= LETTER && LA2_0 <= 68)) ) {
                    alt2=1;
                }


                switch (alt2) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            	    {
            	    set3=(Token)input.LT(1);

            	    if ( (input.LA(1) >= ASTERISK && input.LA(1) <= COMMENT)||(input.LA(1) >= DEFINED && input.LA(1) <= DIGIT)||(input.LA(1) >= EXC && input.LA(1) <= ID)||(input.LA(1) >= INCLUDE && input.LA(1) <= 68) ) {
            	        input.consume();
            	        adaptor.addChild(root_0, 
            	        (Object)adaptor.create(set3)
            	        );
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    if ( cnt2 >= 1 ) break loop2;
                        EarlyExitException eee =
                            new EarlyExitException(2, input);
                        throw eee;
                }
                cnt2++;
            } while (true);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "smth_else"


    public static class code_fragment_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "code_fragment"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:50:1: code_fragment : ( define_directive | import_declaration | if_simple_check_wrapper | if_define_block_wrapper | if_not_define_block_wrapper | smth_else );
    public final PreprocessorParser.code_fragment_return code_fragment() throws RecognitionException {
        PreprocessorParser.code_fragment_return retval = new PreprocessorParser.code_fragment_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.define_directive_return define_directive4 =null;

        PreprocessorParser.import_declaration_return import_declaration5 =null;

        PreprocessorParser.if_simple_check_wrapper_return if_simple_check_wrapper6 =null;

        PreprocessorParser.if_define_block_wrapper_return if_define_block_wrapper7 =null;

        PreprocessorParser.if_not_define_block_wrapper_return if_not_define_block_wrapper8 =null;

        PreprocessorParser.smth_else_return smth_else9 =null;



        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:51:2: ( define_directive | import_declaration | if_simple_check_wrapper | if_define_block_wrapper | if_not_define_block_wrapper | smth_else )
            int alt3=6;
            alt3 = dfa3.predict(input);
            switch (alt3) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:51:4: define_directive
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_define_directive_in_code_fragment212);
                    define_directive4=define_directive();

                    state._fsp--;

                    adaptor.addChild(root_0, define_directive4.getTree());

                    }
                    break;
                case 2 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:52:4: import_declaration
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_import_declaration_in_code_fragment217);
                    import_declaration5=import_declaration();

                    state._fsp--;

                    adaptor.addChild(root_0, import_declaration5.getTree());

                    }
                    break;
                case 3 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:53:4: if_simple_check_wrapper
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_if_simple_check_wrapper_in_code_fragment222);
                    if_simple_check_wrapper6=if_simple_check_wrapper();

                    state._fsp--;

                    adaptor.addChild(root_0, if_simple_check_wrapper6.getTree());

                    }
                    break;
                case 4 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:54:4: if_define_block_wrapper
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_if_define_block_wrapper_in_code_fragment227);
                    if_define_block_wrapper7=if_define_block_wrapper();

                    state._fsp--;

                    adaptor.addChild(root_0, if_define_block_wrapper7.getTree());

                    }
                    break;
                case 5 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:55:4: if_not_define_block_wrapper
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_if_not_define_block_wrapper_in_code_fragment232);
                    if_not_define_block_wrapper8=if_not_define_block_wrapper();

                    state._fsp--;

                    adaptor.addChild(root_0, if_not_define_block_wrapper8.getTree());

                    }
                    break;
                case 6 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:56:4: smth_else
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_smth_else_in_code_fragment237);
                    smth_else9=smth_else();

                    state._fsp--;

                    adaptor.addChild(root_0, smth_else9.getTree());

                    }
                    break;

            }
            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "code_fragment"


    public static class ret_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ret"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:59:1: ret : ( RET )+ ;
    public final PreprocessorParser.ret_return ret() throws RecognitionException {
        PreprocessorParser.ret_return retval = new PreprocessorParser.ret_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token RET10=null;

        Object RET10_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:59:5: ( ( RET )+ )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:59:7: ( RET )+
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:59:7: ( RET )+
            int cnt4=0;
            loop4:
            do {
                int alt4=2;
                int LA4_0 = input.LA(1);

                if ( (LA4_0==RET) ) {
                    alt4=1;
                }


                switch (alt4) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:59:7: RET
            	    {
            	    RET10=(Token)match(input,RET,FOLLOW_RET_in_ret248); 
            	    RET10_tree = 
            	    (Object)adaptor.create(RET10)
            	    ;
            	    adaptor.addChild(root_0, RET10_tree);


            	    }
            	    break;

            	default :
            	    if ( cnt4 >= 1 ) break loop4;
                        EarlyExitException eee =
                            new EarlyExitException(4, input);
                        throw eee;
                }
                cnt4++;
            } while (true);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ret"


    public static class if_not_define_block_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "if_not_define_block_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:61:1: if_not_define_block_wrapper : if_not_define_block -> ^( T_IF_NOT_DEFINE if_not_define_block ) ;
    public final PreprocessorParser.if_not_define_block_wrapper_return if_not_define_block_wrapper() throws RecognitionException {
        PreprocessorParser.if_not_define_block_wrapper_return retval = new PreprocessorParser.if_not_define_block_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.if_not_define_block_return if_not_define_block11 =null;


        RewriteRuleSubtreeStream stream_if_not_define_block=new RewriteRuleSubtreeStream(adaptor,"rule if_not_define_block");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:62:2: ( if_not_define_block -> ^( T_IF_NOT_DEFINE if_not_define_block ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:62:4: if_not_define_block
            {
            pushFollow(FOLLOW_if_not_define_block_in_if_not_define_block_wrapper258);
            if_not_define_block11=if_not_define_block();

            state._fsp--;

            stream_if_not_define_block.add(if_not_define_block11.getTree());

            // AST REWRITE
            // elements: if_not_define_block
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 62:24: -> ^( T_IF_NOT_DEFINE if_not_define_block )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:62:27: ^( T_IF_NOT_DEFINE if_not_define_block )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_IF_NOT_DEFINE, "T_IF_NOT_DEFINE")
                , root_1);

                adaptor.addChild(root_1, stream_if_not_define_block.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "if_not_define_block_wrapper"


    public static class if_not_define_block_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "if_not_define_block"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:64:1: if_not_define_block : ( ifndef1 | ifndef2 ) ret main_code_block ( ELSE ret else_code_block )? ENDIF ;
    public final PreprocessorParser.if_not_define_block_return if_not_define_block() throws RecognitionException {
        PreprocessorParser.if_not_define_block_return retval = new PreprocessorParser.if_not_define_block_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token ELSE16=null;
        Token ENDIF19=null;
        PreprocessorParser.ifndef1_return ifndef112 =null;

        PreprocessorParser.ifndef2_return ifndef213 =null;

        PreprocessorParser.ret_return ret14 =null;

        PreprocessorParser.main_code_block_return main_code_block15 =null;

        PreprocessorParser.ret_return ret17 =null;

        PreprocessorParser.else_code_block_return else_code_block18 =null;


        Object ELSE16_tree=null;
        Object ENDIF19_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:65:2: ( ( ifndef1 | ifndef2 ) ret main_code_block ( ELSE ret else_code_block )? ENDIF )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:65:4: ( ifndef1 | ifndef2 ) ret main_code_block ( ELSE ret else_code_block )? ENDIF
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:65:4: ( ifndef1 | ifndef2 )
            int alt5=2;
            int LA5_0 = input.LA(1);

            if ( (LA5_0==IFNDEF) ) {
                alt5=1;
            }
            else if ( (LA5_0==IF) ) {
                alt5=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 5, 0, input);

                throw nvae;

            }
            switch (alt5) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:65:5: ifndef1
                    {
                    pushFollow(FOLLOW_ifndef1_in_if_not_define_block277);
                    ifndef112=ifndef1();

                    state._fsp--;

                    adaptor.addChild(root_0, ifndef112.getTree());

                    }
                    break;
                case 2 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:65:15: ifndef2
                    {
                    pushFollow(FOLLOW_ifndef2_in_if_not_define_block281);
                    ifndef213=ifndef2();

                    state._fsp--;

                    adaptor.addChild(root_0, ifndef213.getTree());

                    }
                    break;

            }


            pushFollow(FOLLOW_ret_in_if_not_define_block284);
            ret14=ret();

            state._fsp--;

            adaptor.addChild(root_0, ret14.getTree());

            pushFollow(FOLLOW_main_code_block_in_if_not_define_block288);
            main_code_block15=main_code_block();

            state._fsp--;

            adaptor.addChild(root_0, main_code_block15.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:67:3: ( ELSE ret else_code_block )?
            int alt6=2;
            int LA6_0 = input.LA(1);

            if ( (LA6_0==ELSE) ) {
                alt6=1;
            }
            switch (alt6) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:67:4: ELSE ret else_code_block
                    {
                    ELSE16=(Token)match(input,ELSE,FOLLOW_ELSE_in_if_not_define_block293); 
                    ELSE16_tree = 
                    (Object)adaptor.create(ELSE16)
                    ;
                    adaptor.addChild(root_0, ELSE16_tree);


                    pushFollow(FOLLOW_ret_in_if_not_define_block295);
                    ret17=ret();

                    state._fsp--;

                    adaptor.addChild(root_0, ret17.getTree());

                    pushFollow(FOLLOW_else_code_block_in_if_not_define_block300);
                    else_code_block18=else_code_block();

                    state._fsp--;

                    adaptor.addChild(root_0, else_code_block18.getTree());

                    }
                    break;

            }


            ENDIF19=(Token)match(input,ENDIF,FOLLOW_ENDIF_in_if_not_define_block307); 
            ENDIF19_tree = 
            (Object)adaptor.create(ENDIF19)
            ;
            adaptor.addChild(root_0, ENDIF19_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "if_not_define_block"


    public static class ifndef1_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ifndef1"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:72:1: ifndef1 : IFNDEF name ;
    public final PreprocessorParser.ifndef1_return ifndef1() throws RecognitionException {
        PreprocessorParser.ifndef1_return retval = new PreprocessorParser.ifndef1_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token IFNDEF20=null;
        PreprocessorParser.name_return name21 =null;


        Object IFNDEF20_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:72:9: ( IFNDEF name )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:72:11: IFNDEF name
            {
            root_0 = (Object)adaptor.nil();


            IFNDEF20=(Token)match(input,IFNDEF,FOLLOW_IFNDEF_in_ifndef1317); 
            IFNDEF20_tree = 
            (Object)adaptor.create(IFNDEF20)
            ;
            adaptor.addChild(root_0, IFNDEF20_tree);


            pushFollow(FOLLOW_name_in_ifndef1319);
            name21=name();

            state._fsp--;

            adaptor.addChild(root_0, name21.getTree());

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ifndef1"


    public static class ifndef2_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ifndef2"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:74:1: ifndef2 : IF EXC DEFINED L_BR name R_BR ;
    public final PreprocessorParser.ifndef2_return ifndef2() throws RecognitionException {
        PreprocessorParser.ifndef2_return retval = new PreprocessorParser.ifndef2_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token IF22=null;
        Token EXC23=null;
        Token DEFINED24=null;
        Token L_BR25=null;
        Token R_BR27=null;
        PreprocessorParser.name_return name26 =null;


        Object IF22_tree=null;
        Object EXC23_tree=null;
        Object DEFINED24_tree=null;
        Object L_BR25_tree=null;
        Object R_BR27_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:74:9: ( IF EXC DEFINED L_BR name R_BR )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:74:11: IF EXC DEFINED L_BR name R_BR
            {
            root_0 = (Object)adaptor.nil();


            IF22=(Token)match(input,IF,FOLLOW_IF_in_ifndef2327); 
            IF22_tree = 
            (Object)adaptor.create(IF22)
            ;
            adaptor.addChild(root_0, IF22_tree);


            EXC23=(Token)match(input,EXC,FOLLOW_EXC_in_ifndef2330); 
            EXC23_tree = 
            (Object)adaptor.create(EXC23)
            ;
            adaptor.addChild(root_0, EXC23_tree);


            DEFINED24=(Token)match(input,DEFINED,FOLLOW_DEFINED_in_ifndef2333); 
            DEFINED24_tree = 
            (Object)adaptor.create(DEFINED24)
            ;
            adaptor.addChild(root_0, DEFINED24_tree);


            L_BR25=(Token)match(input,L_BR,FOLLOW_L_BR_in_ifndef2335); 
            L_BR25_tree = 
            (Object)adaptor.create(L_BR25)
            ;
            adaptor.addChild(root_0, L_BR25_tree);


            pushFollow(FOLLOW_name_in_ifndef2338);
            name26=name();

            state._fsp--;

            adaptor.addChild(root_0, name26.getTree());

            R_BR27=(Token)match(input,R_BR,FOLLOW_R_BR_in_ifndef2340); 
            R_BR27_tree = 
            (Object)adaptor.create(R_BR27)
            ;
            adaptor.addChild(root_0, R_BR27_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ifndef2"


    public static class if_simple_check_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "if_simple_check_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:76:1: if_simple_check_wrapper : if_simple_check -> ^( T_IF_SIMPLE if_simple_check ) ;
    public final PreprocessorParser.if_simple_check_wrapper_return if_simple_check_wrapper() throws RecognitionException {
        PreprocessorParser.if_simple_check_wrapper_return retval = new PreprocessorParser.if_simple_check_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.if_simple_check_return if_simple_check28 =null;


        RewriteRuleSubtreeStream stream_if_simple_check=new RewriteRuleSubtreeStream(adaptor,"rule if_simple_check");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:77:2: ( if_simple_check -> ^( T_IF_SIMPLE if_simple_check ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:77:4: if_simple_check
            {
            pushFollow(FOLLOW_if_simple_check_in_if_simple_check_wrapper349);
            if_simple_check28=if_simple_check();

            state._fsp--;

            stream_if_simple_check.add(if_simple_check28.getTree());

            // AST REWRITE
            // elements: if_simple_check
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 77:20: -> ^( T_IF_SIMPLE if_simple_check )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:77:23: ^( T_IF_SIMPLE if_simple_check )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_IF_SIMPLE, "T_IF_SIMPLE")
                , root_1);

                adaptor.addChild(root_1, stream_if_simple_check.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "if_simple_check_wrapper"


    public static class if_simple_check_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "if_simple_check"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:79:1: if_simple_check : IF expression_wrapper ret ( main_code_block )? ( ELIF expression_wrapper ret else_code_block )* ( ELSE ret else_code_block )? ENDIF ;
    public final PreprocessorParser.if_simple_check_return if_simple_check() throws RecognitionException {
        PreprocessorParser.if_simple_check_return retval = new PreprocessorParser.if_simple_check_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token IF29=null;
        Token ELIF33=null;
        Token ELSE37=null;
        Token ENDIF40=null;
        PreprocessorParser.expression_wrapper_return expression_wrapper30 =null;

        PreprocessorParser.ret_return ret31 =null;

        PreprocessorParser.main_code_block_return main_code_block32 =null;

        PreprocessorParser.expression_wrapper_return expression_wrapper34 =null;

        PreprocessorParser.ret_return ret35 =null;

        PreprocessorParser.else_code_block_return else_code_block36 =null;

        PreprocessorParser.ret_return ret38 =null;

        PreprocessorParser.else_code_block_return else_code_block39 =null;


        Object IF29_tree=null;
        Object ELIF33_tree=null;
        Object ELSE37_tree=null;
        Object ENDIF40_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:80:2: ( IF expression_wrapper ret ( main_code_block )? ( ELIF expression_wrapper ret else_code_block )* ( ELSE ret else_code_block )? ENDIF )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:80:4: IF expression_wrapper ret ( main_code_block )? ( ELIF expression_wrapper ret else_code_block )* ( ELSE ret else_code_block )? ENDIF
            {
            root_0 = (Object)adaptor.nil();


            IF29=(Token)match(input,IF,FOLLOW_IF_in_if_simple_check367); 
            IF29_tree = 
            (Object)adaptor.create(IF29)
            ;
            adaptor.addChild(root_0, IF29_tree);


            pushFollow(FOLLOW_expression_wrapper_in_if_simple_check369);
            expression_wrapper30=expression_wrapper();

            state._fsp--;

            adaptor.addChild(root_0, expression_wrapper30.getTree());

            pushFollow(FOLLOW_ret_in_if_simple_check371);
            ret31=ret();

            state._fsp--;

            adaptor.addChild(root_0, ret31.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:81:3: ( main_code_block )?
            int alt7=2;
            int LA7_0 = input.LA(1);

            if ( ((LA7_0 >= ASTERISK && LA7_0 <= DIGIT)||(LA7_0 >= EXC && LA7_0 <= 68)) ) {
                alt7=1;
            }
            switch (alt7) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:81:3: main_code_block
                    {
                    pushFollow(FOLLOW_main_code_block_in_if_simple_check375);
                    main_code_block32=main_code_block();

                    state._fsp--;

                    adaptor.addChild(root_0, main_code_block32.getTree());

                    }
                    break;

            }


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:82:3: ( ELIF expression_wrapper ret else_code_block )*
            loop8:
            do {
                int alt8=2;
                int LA8_0 = input.LA(1);

                if ( (LA8_0==ELIF) ) {
                    alt8=1;
                }


                switch (alt8) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:82:4: ELIF expression_wrapper ret else_code_block
            	    {
            	    ELIF33=(Token)match(input,ELIF,FOLLOW_ELIF_in_if_simple_check381); 
            	    ELIF33_tree = 
            	    (Object)adaptor.create(ELIF33)
            	    ;
            	    adaptor.addChild(root_0, ELIF33_tree);


            	    pushFollow(FOLLOW_expression_wrapper_in_if_simple_check384);
            	    expression_wrapper34=expression_wrapper();

            	    state._fsp--;

            	    adaptor.addChild(root_0, expression_wrapper34.getTree());

            	    pushFollow(FOLLOW_ret_in_if_simple_check386);
            	    ret35=ret();

            	    state._fsp--;

            	    adaptor.addChild(root_0, ret35.getTree());

            	    pushFollow(FOLLOW_else_code_block_in_if_simple_check390);
            	    else_code_block36=else_code_block();

            	    state._fsp--;

            	    adaptor.addChild(root_0, else_code_block36.getTree());

            	    }
            	    break;

            	default :
            	    break loop8;
                }
            } while (true);


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:84:3: ( ELSE ret else_code_block )?
            int alt9=2;
            int LA9_0 = input.LA(1);

            if ( (LA9_0==ELSE) ) {
                alt9=1;
            }
            switch (alt9) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:84:4: ELSE ret else_code_block
                    {
                    ELSE37=(Token)match(input,ELSE,FOLLOW_ELSE_in_if_simple_check397); 
                    ELSE37_tree = 
                    (Object)adaptor.create(ELSE37)
                    ;
                    adaptor.addChild(root_0, ELSE37_tree);


                    pushFollow(FOLLOW_ret_in_if_simple_check399);
                    ret38=ret();

                    state._fsp--;

                    adaptor.addChild(root_0, ret38.getTree());

                    pushFollow(FOLLOW_else_code_block_in_if_simple_check404);
                    else_code_block39=else_code_block();

                    state._fsp--;

                    adaptor.addChild(root_0, else_code_block39.getTree());

                    }
                    break;

            }


            ENDIF40=(Token)match(input,ENDIF,FOLLOW_ENDIF_in_if_simple_check411); 
            ENDIF40_tree = 
            (Object)adaptor.create(ENDIF40)
            ;
            adaptor.addChild(root_0, ENDIF40_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "if_simple_check"


    public static class expression_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "expression_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:89:1: expression_wrapper : expression -> ^( T_EXPRESSION expression ) ;
    public final PreprocessorParser.expression_wrapper_return expression_wrapper() throws RecognitionException {
        PreprocessorParser.expression_wrapper_return retval = new PreprocessorParser.expression_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.expression_return expression41 =null;


        RewriteRuleSubtreeStream stream_expression=new RewriteRuleSubtreeStream(adaptor,"rule expression");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:90:2: ( expression -> ^( T_EXPRESSION expression ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:90:4: expression
            {
            pushFollow(FOLLOW_expression_in_expression_wrapper423);
            expression41=expression();

            state._fsp--;

            stream_expression.add(expression41.getTree());

            // AST REWRITE
            // elements: expression
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 90:15: -> ^( T_EXPRESSION expression )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:90:18: ^( T_EXPRESSION expression )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EXPRESSION, "T_EXPRESSION")
                , root_1);

                adaptor.addChild(root_1, stream_expression.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "expression_wrapper"


    public static class if_define_block_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "if_define_block_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:92:1: if_define_block_wrapper : if_define_block -> ^( T_IF_DEFINE if_define_block ) ;
    public final PreprocessorParser.if_define_block_wrapper_return if_define_block_wrapper() throws RecognitionException {
        PreprocessorParser.if_define_block_wrapper_return retval = new PreprocessorParser.if_define_block_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.if_define_block_return if_define_block42 =null;


        RewriteRuleSubtreeStream stream_if_define_block=new RewriteRuleSubtreeStream(adaptor,"rule if_define_block");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:93:2: ( if_define_block -> ^( T_IF_DEFINE if_define_block ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:93:4: if_define_block
            {
            pushFollow(FOLLOW_if_define_block_in_if_define_block_wrapper442);
            if_define_block42=if_define_block();

            state._fsp--;

            stream_if_define_block.add(if_define_block42.getTree());

            // AST REWRITE
            // elements: if_define_block
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 93:20: -> ^( T_IF_DEFINE if_define_block )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:93:23: ^( T_IF_DEFINE if_define_block )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_IF_DEFINE, "T_IF_DEFINE")
                , root_1);

                adaptor.addChild(root_1, stream_if_define_block.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "if_define_block_wrapper"


    public static class if_define_block_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "if_define_block"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:95:1: if_define_block : ( ifdef1 | ifdef2 ) ret ( main_code_block )? ( ELSE ret else_code_block )? ENDIF ;
    public final PreprocessorParser.if_define_block_return if_define_block() throws RecognitionException {
        PreprocessorParser.if_define_block_return retval = new PreprocessorParser.if_define_block_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token ELSE47=null;
        Token ENDIF50=null;
        PreprocessorParser.ifdef1_return ifdef143 =null;

        PreprocessorParser.ifdef2_return ifdef244 =null;

        PreprocessorParser.ret_return ret45 =null;

        PreprocessorParser.main_code_block_return main_code_block46 =null;

        PreprocessorParser.ret_return ret48 =null;

        PreprocessorParser.else_code_block_return else_code_block49 =null;


        Object ELSE47_tree=null;
        Object ENDIF50_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:96:2: ( ( ifdef1 | ifdef2 ) ret ( main_code_block )? ( ELSE ret else_code_block )? ENDIF )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:96:4: ( ifdef1 | ifdef2 ) ret ( main_code_block )? ( ELSE ret else_code_block )? ENDIF
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:96:4: ( ifdef1 | ifdef2 )
            int alt10=2;
            int LA10_0 = input.LA(1);

            if ( (LA10_0==IF) ) {
                alt10=1;
            }
            else if ( (LA10_0==IFDEF) ) {
                alt10=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 10, 0, input);

                throw nvae;

            }
            switch (alt10) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:96:5: ifdef1
                    {
                    pushFollow(FOLLOW_ifdef1_in_if_define_block462);
                    ifdef143=ifdef1();

                    state._fsp--;

                    adaptor.addChild(root_0, ifdef143.getTree());

                    }
                    break;
                case 2 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:96:14: ifdef2
                    {
                    pushFollow(FOLLOW_ifdef2_in_if_define_block466);
                    ifdef244=ifdef2();

                    state._fsp--;

                    adaptor.addChild(root_0, ifdef244.getTree());

                    }
                    break;

            }


            pushFollow(FOLLOW_ret_in_if_define_block469);
            ret45=ret();

            state._fsp--;

            adaptor.addChild(root_0, ret45.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:97:3: ( main_code_block )?
            int alt11=2;
            int LA11_0 = input.LA(1);

            if ( ((LA11_0 >= ASTERISK && LA11_0 <= DIGIT)||(LA11_0 >= EXC && LA11_0 <= 68)) ) {
                alt11=1;
            }
            switch (alt11) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:97:3: main_code_block
                    {
                    pushFollow(FOLLOW_main_code_block_in_if_define_block473);
                    main_code_block46=main_code_block();

                    state._fsp--;

                    adaptor.addChild(root_0, main_code_block46.getTree());

                    }
                    break;

            }


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:98:3: ( ELSE ret else_code_block )?
            int alt12=2;
            int LA12_0 = input.LA(1);

            if ( (LA12_0==ELSE) ) {
                alt12=1;
            }
            switch (alt12) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:98:4: ELSE ret else_code_block
                    {
                    ELSE47=(Token)match(input,ELSE,FOLLOW_ELSE_in_if_define_block479); 
                    ELSE47_tree = 
                    (Object)adaptor.create(ELSE47)
                    ;
                    adaptor.addChild(root_0, ELSE47_tree);


                    pushFollow(FOLLOW_ret_in_if_define_block481);
                    ret48=ret();

                    state._fsp--;

                    adaptor.addChild(root_0, ret48.getTree());

                    pushFollow(FOLLOW_else_code_block_in_if_define_block486);
                    else_code_block49=else_code_block();

                    state._fsp--;

                    adaptor.addChild(root_0, else_code_block49.getTree());

                    }
                    break;

            }


            ENDIF50=(Token)match(input,ENDIF,FOLLOW_ENDIF_in_if_define_block493); 
            ENDIF50_tree = 
            (Object)adaptor.create(ENDIF50)
            ;
            adaptor.addChild(root_0, ENDIF50_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "if_define_block"


    public static class ifdef1_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ifdef1"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:103:1: ifdef1 : IF DEFINED L_BR name R_BR ;
    public final PreprocessorParser.ifdef1_return ifdef1() throws RecognitionException {
        PreprocessorParser.ifdef1_return retval = new PreprocessorParser.ifdef1_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token IF51=null;
        Token DEFINED52=null;
        Token L_BR53=null;
        Token R_BR55=null;
        PreprocessorParser.name_return name54 =null;


        Object IF51_tree=null;
        Object DEFINED52_tree=null;
        Object L_BR53_tree=null;
        Object R_BR55_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:103:8: ( IF DEFINED L_BR name R_BR )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:103:10: IF DEFINED L_BR name R_BR
            {
            root_0 = (Object)adaptor.nil();


            IF51=(Token)match(input,IF,FOLLOW_IF_in_ifdef1503); 
            IF51_tree = 
            (Object)adaptor.create(IF51)
            ;
            adaptor.addChild(root_0, IF51_tree);


            DEFINED52=(Token)match(input,DEFINED,FOLLOW_DEFINED_in_ifdef1505); 
            DEFINED52_tree = 
            (Object)adaptor.create(DEFINED52)
            ;
            adaptor.addChild(root_0, DEFINED52_tree);


            L_BR53=(Token)match(input,L_BR,FOLLOW_L_BR_in_ifdef1507); 
            L_BR53_tree = 
            (Object)adaptor.create(L_BR53)
            ;
            adaptor.addChild(root_0, L_BR53_tree);


            pushFollow(FOLLOW_name_in_ifdef1509);
            name54=name();

            state._fsp--;

            adaptor.addChild(root_0, name54.getTree());

            R_BR55=(Token)match(input,R_BR,FOLLOW_R_BR_in_ifdef1511); 
            R_BR55_tree = 
            (Object)adaptor.create(R_BR55)
            ;
            adaptor.addChild(root_0, R_BR55_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ifdef1"


    public static class ifdef2_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ifdef2"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:105:1: ifdef2 : IFDEF name ;
    public final PreprocessorParser.ifdef2_return ifdef2() throws RecognitionException {
        PreprocessorParser.ifdef2_return retval = new PreprocessorParser.ifdef2_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token IFDEF56=null;
        PreprocessorParser.name_return name57 =null;


        Object IFDEF56_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:105:8: ( IFDEF name )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:105:10: IFDEF name
            {
            root_0 = (Object)adaptor.nil();


            IFDEF56=(Token)match(input,IFDEF,FOLLOW_IFDEF_in_ifdef2519); 
            IFDEF56_tree = 
            (Object)adaptor.create(IFDEF56)
            ;
            adaptor.addChild(root_0, IFDEF56_tree);


            pushFollow(FOLLOW_name_in_ifdef2522);
            name57=name();

            state._fsp--;

            adaptor.addChild(root_0, name57.getTree());

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ifdef2"


    public static class main_code_block_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "main_code_block"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:107:1: main_code_block : code_block -> ^( T_IF_MAIN code_block ) ;
    public final PreprocessorParser.main_code_block_return main_code_block() throws RecognitionException {
        PreprocessorParser.main_code_block_return retval = new PreprocessorParser.main_code_block_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.code_block_return code_block58 =null;


        RewriteRuleSubtreeStream stream_code_block=new RewriteRuleSubtreeStream(adaptor,"rule code_block");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:108:2: ( code_block -> ^( T_IF_MAIN code_block ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:108:4: code_block
            {
            pushFollow(FOLLOW_code_block_in_main_code_block531);
            code_block58=code_block();

            state._fsp--;

            stream_code_block.add(code_block58.getTree());

            // AST REWRITE
            // elements: code_block
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 108:15: -> ^( T_IF_MAIN code_block )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:108:18: ^( T_IF_MAIN code_block )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_IF_MAIN, "T_IF_MAIN")
                , root_1);

                adaptor.addChild(root_1, stream_code_block.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "main_code_block"


    public static class else_code_block_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "else_code_block"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:110:1: else_code_block : code_block -> ^( T_IF_ELSE code_block ) ;
    public final PreprocessorParser.else_code_block_return else_code_block() throws RecognitionException {
        PreprocessorParser.else_code_block_return retval = new PreprocessorParser.else_code_block_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.code_block_return code_block59 =null;


        RewriteRuleSubtreeStream stream_code_block=new RewriteRuleSubtreeStream(adaptor,"rule code_block");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:111:2: ( code_block -> ^( T_IF_ELSE code_block ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:111:4: code_block
            {
            pushFollow(FOLLOW_code_block_in_else_code_block548);
            code_block59=code_block();

            state._fsp--;

            stream_code_block.add(code_block59.getTree());

            // AST REWRITE
            // elements: code_block
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 111:15: -> ^( T_IF_ELSE code_block )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:111:18: ^( T_IF_ELSE code_block )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_IF_ELSE, "T_IF_ELSE")
                , root_1);

                adaptor.addChild(root_1, stream_code_block.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "else_code_block"


    public static class code_block_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "code_block"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:113:1: code_block : ( code_fragment )+ -> ^( T_BLOCK ( code_fragment )+ ) ;
    public final PreprocessorParser.code_block_return code_block() throws RecognitionException {
        PreprocessorParser.code_block_return retval = new PreprocessorParser.code_block_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.code_fragment_return code_fragment60 =null;


        RewriteRuleSubtreeStream stream_code_fragment=new RewriteRuleSubtreeStream(adaptor,"rule code_fragment");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:114:2: ( ( code_fragment )+ -> ^( T_BLOCK ( code_fragment )+ ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:114:4: ( code_fragment )+
            {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:114:4: ( code_fragment )+
            int cnt13=0;
            loop13:
            do {
                int alt13=2;
                int LA13_0 = input.LA(1);

                if ( ((LA13_0 >= ASTERISK && LA13_0 <= DIGIT)||(LA13_0 >= EXC && LA13_0 <= 68)) ) {
                    alt13=1;
                }


                switch (alt13) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:114:4: code_fragment
            	    {
            	    pushFollow(FOLLOW_code_fragment_in_code_block565);
            	    code_fragment60=code_fragment();

            	    state._fsp--;

            	    stream_code_fragment.add(code_fragment60.getTree());

            	    }
            	    break;

            	default :
            	    if ( cnt13 >= 1 ) break loop13;
                        EarlyExitException eee =
                            new EarlyExitException(13, input);
                        throw eee;
                }
                cnt13++;
            } while (true);


            // AST REWRITE
            // elements: code_fragment
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 114:19: -> ^( T_BLOCK ( code_fragment )+ )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:114:22: ^( T_BLOCK ( code_fragment )+ )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_BLOCK, "T_BLOCK")
                , root_1);

                if ( !(stream_code_fragment.hasNext()) ) {
                    throw new RewriteEarlyExitException();
                }
                while ( stream_code_fragment.hasNext() ) {
                    adaptor.addChild(root_1, stream_code_fragment.nextTree());

                }
                stream_code_fragment.reset();

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "code_block"


    public static class inside_block_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "inside_block"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:116:1: inside_block : (~ ( ELSE | ENDIF | ELIF ) )* ;
    public final PreprocessorParser.inside_block_return inside_block() throws RecognitionException {
        PreprocessorParser.inside_block_return retval = new PreprocessorParser.inside_block_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token set61=null;

        Object set61_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:117:2: ( (~ ( ELSE | ENDIF | ELIF ) )* )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:117:4: (~ ( ELSE | ENDIF | ELIF ) )*
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:117:4: (~ ( ELSE | ENDIF | ELIF ) )*
            loop14:
            do {
                int alt14=2;
                int LA14_0 = input.LA(1);

                if ( ((LA14_0 >= ASTERISK && LA14_0 <= DIGIT)||(LA14_0 >= EXC && LA14_0 <= 68)) ) {
                    alt14=1;
                }


                switch (alt14) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            	    {
            	    set61=(Token)input.LT(1);

            	    if ( (input.LA(1) >= ASTERISK && input.LA(1) <= DIGIT)||(input.LA(1) >= EXC && input.LA(1) <= 68) ) {
            	        input.consume();
            	        adaptor.addChild(root_0, 
            	        (Object)adaptor.create(set61)
            	        );
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }


            	    }
            	    break;

            	default :
            	    break loop14;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "inside_block"


    public static class name_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "name"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:119:1: name : ID -> ^( T_NAME ID ) ;
    public final PreprocessorParser.name_return name() throws RecognitionException {
        PreprocessorParser.name_return retval = new PreprocessorParser.name_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token ID62=null;

        Object ID62_tree=null;
        RewriteRuleTokenStream stream_ID=new RewriteRuleTokenStream(adaptor,"token ID");

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:119:6: ( ID -> ^( T_NAME ID ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:119:8: ID
            {
            ID62=(Token)match(input,ID,FOLLOW_ID_in_name605);  
            stream_ID.add(ID62);


            // AST REWRITE
            // elements: ID
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 119:11: -> ^( T_NAME ID )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:119:14: ^( T_NAME ID )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_NAME, "T_NAME")
                , root_1);

                adaptor.addChild(root_1, 
                stream_ID.nextNode()
                );

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "name"


    public static class import_declaration_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "import_declaration"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:121:1: import_declaration : ( IMPORT | INCLUDE ) import_end ;
    public final PreprocessorParser.import_declaration_return import_declaration() throws RecognitionException {
        PreprocessorParser.import_declaration_return retval = new PreprocessorParser.import_declaration_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token set63=null;
        PreprocessorParser.import_end_return import_end64 =null;


        Object set63_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:122:2: ( ( IMPORT | INCLUDE ) import_end )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:122:4: ( IMPORT | INCLUDE ) import_end
            {
            root_0 = (Object)adaptor.nil();


            set63=(Token)input.LT(1);

            if ( (input.LA(1) >= IMPORT && input.LA(1) <= INCLUDE) ) {
                input.consume();
                adaptor.addChild(root_0, 
                (Object)adaptor.create(set63)
                );
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }


            pushFollow(FOLLOW_import_end_in_import_declaration630);
            import_end64=import_end();

            state._fsp--;

            adaptor.addChild(root_0, import_end64.getTree());

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "import_declaration"


    public static class import_end_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "import_end"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:124:1: import_end : ( import_internal | import_external );
    public final PreprocessorParser.import_end_return import_end() throws RecognitionException {
        PreprocessorParser.import_end_return retval = new PreprocessorParser.import_end_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.import_internal_return import_internal65 =null;

        PreprocessorParser.import_external_return import_external66 =null;



        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:125:2: ( import_internal | import_external )
            int alt15=2;
            int LA15_0 = input.LA(1);

            if ( (LA15_0==L_UBR) ) {
                alt15=1;
            }
            else if ( (LA15_0==QUOTE) ) {
                alt15=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 15, 0, input);

                throw nvae;

            }
            switch (alt15) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:125:4: import_internal
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_import_internal_in_import_end640);
                    import_internal65=import_internal();

                    state._fsp--;

                    adaptor.addChild(root_0, import_internal65.getTree());

                    }
                    break;
                case 2 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:126:5: import_external
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_import_external_in_import_end647);
                    import_external66=import_external();

                    state._fsp--;

                    adaptor.addChild(root_0, import_external66.getTree());

                    }
                    break;

            }
            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "import_end"


    public static class import_internal_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "import_internal"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:128:1: import_internal : import_internal_string -> ^( T_IMPORT import_internal_string ) ;
    public final PreprocessorParser.import_internal_return import_internal() throws RecognitionException {
        PreprocessorParser.import_internal_return retval = new PreprocessorParser.import_internal_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.import_internal_string_return import_internal_string67 =null;


        RewriteRuleSubtreeStream stream_import_internal_string=new RewriteRuleSubtreeStream(adaptor,"rule import_internal_string");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:129:2: ( import_internal_string -> ^( T_IMPORT import_internal_string ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:129:4: import_internal_string
            {
            pushFollow(FOLLOW_import_internal_string_in_import_internal657);
            import_internal_string67=import_internal_string();

            state._fsp--;

            stream_import_internal_string.add(import_internal_string67.getTree());

            // AST REWRITE
            // elements: import_internal_string
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 129:27: -> ^( T_IMPORT import_internal_string )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:129:30: ^( T_IMPORT import_internal_string )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_IMPORT, "T_IMPORT")
                , root_1);

                adaptor.addChild(root_1, stream_import_internal_string.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "import_internal"


    public static class import_internal_string_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "import_internal_string"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:131:1: import_internal_string : L_UBR filename ( '/' filename )* R_UBR ;
    public final PreprocessorParser.import_internal_string_return import_internal_string() throws RecognitionException {
        PreprocessorParser.import_internal_string_return retval = new PreprocessorParser.import_internal_string_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token L_UBR68=null;
        Token char_literal70=null;
        Token R_UBR72=null;
        PreprocessorParser.filename_return filename69 =null;

        PreprocessorParser.filename_return filename71 =null;


        Object L_UBR68_tree=null;
        Object char_literal70_tree=null;
        Object R_UBR72_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:132:2: ( L_UBR filename ( '/' filename )* R_UBR )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:132:4: L_UBR filename ( '/' filename )* R_UBR
            {
            root_0 = (Object)adaptor.nil();


            L_UBR68=(Token)match(input,L_UBR,FOLLOW_L_UBR_in_import_internal_string675); 
            L_UBR68_tree = 
            (Object)adaptor.create(L_UBR68)
            ;
            adaptor.addChild(root_0, L_UBR68_tree);


            pushFollow(FOLLOW_filename_in_import_internal_string678);
            filename69=filename();

            state._fsp--;

            adaptor.addChild(root_0, filename69.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:132:20: ( '/' filename )*
            loop16:
            do {
                int alt16=2;
                int LA16_0 = input.LA(1);

                if ( (LA16_0==63) ) {
                    alt16=1;
                }


                switch (alt16) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:132:21: '/' filename
            	    {
            	    char_literal70=(Token)match(input,63,FOLLOW_63_in_import_internal_string681); 
            	    char_literal70_tree = 
            	    (Object)adaptor.create(char_literal70)
            	    ;
            	    adaptor.addChild(root_0, char_literal70_tree);


            	    pushFollow(FOLLOW_filename_in_import_internal_string683);
            	    filename71=filename();

            	    state._fsp--;

            	    adaptor.addChild(root_0, filename71.getTree());

            	    }
            	    break;

            	default :
            	    break loop16;
                }
            } while (true);


            R_UBR72=(Token)match(input,R_UBR,FOLLOW_R_UBR_in_import_internal_string688); 
            R_UBR72_tree = 
            (Object)adaptor.create(R_UBR72)
            ;
            adaptor.addChild(root_0, R_UBR72_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "import_internal_string"


    public static class filename_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "filename"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:134:1: filename : ID ( MINUS ID )* ( '.' ID )? ;
    public final PreprocessorParser.filename_return filename() throws RecognitionException {
        PreprocessorParser.filename_return retval = new PreprocessorParser.filename_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token ID73=null;
        Token MINUS74=null;
        Token ID75=null;
        Token char_literal76=null;
        Token ID77=null;

        Object ID73_tree=null;
        Object MINUS74_tree=null;
        Object ID75_tree=null;
        Object char_literal76_tree=null;
        Object ID77_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:134:9: ( ID ( MINUS ID )* ( '.' ID )? )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:134:11: ID ( MINUS ID )* ( '.' ID )?
            {
            root_0 = (Object)adaptor.nil();


            ID73=(Token)match(input,ID,FOLLOW_ID_in_filename696); 
            ID73_tree = 
            (Object)adaptor.create(ID73)
            ;
            adaptor.addChild(root_0, ID73_tree);


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:134:14: ( MINUS ID )*
            loop17:
            do {
                int alt17=2;
                int LA17_0 = input.LA(1);

                if ( (LA17_0==MINUS) ) {
                    alt17=1;
                }


                switch (alt17) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:134:15: MINUS ID
            	    {
            	    MINUS74=(Token)match(input,MINUS,FOLLOW_MINUS_in_filename699); 
            	    MINUS74_tree = 
            	    (Object)adaptor.create(MINUS74)
            	    ;
            	    adaptor.addChild(root_0, MINUS74_tree);


            	    ID75=(Token)match(input,ID,FOLLOW_ID_in_filename701); 
            	    ID75_tree = 
            	    (Object)adaptor.create(ID75)
            	    ;
            	    adaptor.addChild(root_0, ID75_tree);


            	    }
            	    break;

            	default :
            	    break loop17;
                }
            } while (true);


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:134:26: ( '.' ID )?
            int alt18=2;
            int LA18_0 = input.LA(1);

            if ( (LA18_0==62) ) {
                alt18=1;
            }
            switch (alt18) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:134:27: '.' ID
                    {
                    char_literal76=(Token)match(input,62,FOLLOW_62_in_filename706); 
                    char_literal76_tree = 
                    (Object)adaptor.create(char_literal76)
                    ;
                    adaptor.addChild(root_0, char_literal76_tree);


                    ID77=(Token)match(input,ID,FOLLOW_ID_in_filename708); 
                    ID77_tree = 
                    (Object)adaptor.create(ID77)
                    ;
                    adaptor.addChild(root_0, ID77_tree);


                    }
                    break;

            }


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "filename"


    public static class filename2_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "filename2"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:136:1: filename2 : ID ( ( MINUS | PLUS ) ID )* ( '.' ID )? ;
    public final PreprocessorParser.filename2_return filename2() throws RecognitionException {
        PreprocessorParser.filename2_return retval = new PreprocessorParser.filename2_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token ID78=null;
        Token set79=null;
        Token ID80=null;
        Token char_literal81=null;
        Token ID82=null;

        Object ID78_tree=null;
        Object set79_tree=null;
        Object ID80_tree=null;
        Object char_literal81_tree=null;
        Object ID82_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:137:2: ( ID ( ( MINUS | PLUS ) ID )* ( '.' ID )? )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:137:4: ID ( ( MINUS | PLUS ) ID )* ( '.' ID )?
            {
            root_0 = (Object)adaptor.nil();


            ID78=(Token)match(input,ID,FOLLOW_ID_in_filename2719); 
            ID78_tree = 
            (Object)adaptor.create(ID78)
            ;
            adaptor.addChild(root_0, ID78_tree);


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:137:7: ( ( MINUS | PLUS ) ID )*
            loop19:
            do {
                int alt19=2;
                int LA19_0 = input.LA(1);

                if ( (LA19_0==MINUS||LA19_0==PLUS) ) {
                    alt19=1;
                }


                switch (alt19) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:137:8: ( MINUS | PLUS ) ID
            	    {
            	    set79=(Token)input.LT(1);

            	    if ( input.LA(1)==MINUS||input.LA(1)==PLUS ) {
            	        input.consume();
            	        adaptor.addChild(root_0, 
            	        (Object)adaptor.create(set79)
            	        );
            	        state.errorRecovery=false;
            	    }
            	    else {
            	        MismatchedSetException mse = new MismatchedSetException(null,input);
            	        throw mse;
            	    }


            	    ID80=(Token)match(input,ID,FOLLOW_ID_in_filename2730); 
            	    ID80_tree = 
            	    (Object)adaptor.create(ID80)
            	    ;
            	    adaptor.addChild(root_0, ID80_tree);


            	    }
            	    break;

            	default :
            	    break loop19;
                }
            } while (true);


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:137:28: ( '.' ID )?
            int alt20=2;
            int LA20_0 = input.LA(1);

            if ( (LA20_0==62) ) {
                alt20=1;
            }
            switch (alt20) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:137:29: '.' ID
                    {
                    char_literal81=(Token)match(input,62,FOLLOW_62_in_filename2735); 
                    char_literal81_tree = 
                    (Object)adaptor.create(char_literal81)
                    ;
                    adaptor.addChild(root_0, char_literal81_tree);


                    ID82=(Token)match(input,ID,FOLLOW_ID_in_filename2737); 
                    ID82_tree = 
                    (Object)adaptor.create(ID82)
                    ;
                    adaptor.addChild(root_0, ID82_tree);


                    }
                    break;

            }


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "filename2"


    public static class import_external_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "import_external"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:139:1: import_external : import_external2 -> ^( T_INCLUDE import_external2 ) ;
    public final PreprocessorParser.import_external_return import_external() throws RecognitionException {
        PreprocessorParser.import_external_return retval = new PreprocessorParser.import_external_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.import_external2_return import_external283 =null;


        RewriteRuleSubtreeStream stream_import_external2=new RewriteRuleSubtreeStream(adaptor,"rule import_external2");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:140:2: ( import_external2 -> ^( T_INCLUDE import_external2 ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:140:4: import_external2
            {
            pushFollow(FOLLOW_import_external2_in_import_external748);
            import_external283=import_external2();

            state._fsp--;

            stream_import_external2.add(import_external283.getTree());

            // AST REWRITE
            // elements: import_external2
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 140:21: -> ^( T_INCLUDE import_external2 )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:140:24: ^( T_INCLUDE import_external2 )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_INCLUDE, "T_INCLUDE")
                , root_1);

                adaptor.addChild(root_1, stream_import_external2.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "import_external"


    public static class import_external2_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "import_external2"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:142:1: import_external2 : QUOTE filename2 QUOTE ;
    public final PreprocessorParser.import_external2_return import_external2() throws RecognitionException {
        PreprocessorParser.import_external2_return retval = new PreprocessorParser.import_external2_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token QUOTE84=null;
        Token QUOTE86=null;
        PreprocessorParser.filename2_return filename285 =null;


        Object QUOTE84_tree=null;
        Object QUOTE86_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:143:2: ( QUOTE filename2 QUOTE )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:143:4: QUOTE filename2 QUOTE
            {
            root_0 = (Object)adaptor.nil();


            QUOTE84=(Token)match(input,QUOTE,FOLLOW_QUOTE_in_import_external2766); 
            QUOTE84_tree = 
            (Object)adaptor.create(QUOTE84)
            ;
            adaptor.addChild(root_0, QUOTE84_tree);


            pushFollow(FOLLOW_filename2_in_import_external2768);
            filename285=filename2();

            state._fsp--;

            adaptor.addChild(root_0, filename285.getTree());

            QUOTE86=(Token)match(input,QUOTE,FOLLOW_QUOTE_in_import_external2770); 
            QUOTE86_tree = 
            (Object)adaptor.create(QUOTE86)
            ;
            adaptor.addChild(root_0, QUOTE86_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "import_external2"


    public static class define_directive_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "define_directive"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:145:1: define_directive : DEFINE id_wrapper ( mln_end )? ( replace_wrapper )? -> ^( T_DEFINE DEFINE id_wrapper ( replace_wrapper )? ) ;
    public final PreprocessorParser.define_directive_return define_directive() throws RecognitionException {
        PreprocessorParser.define_directive_return retval = new PreprocessorParser.define_directive_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token DEFINE87=null;
        PreprocessorParser.id_wrapper_return id_wrapper88 =null;

        PreprocessorParser.mln_end_return mln_end89 =null;

        PreprocessorParser.replace_wrapper_return replace_wrapper90 =null;


        Object DEFINE87_tree=null;
        RewriteRuleTokenStream stream_DEFINE=new RewriteRuleTokenStream(adaptor,"token DEFINE");
        RewriteRuleSubtreeStream stream_replace_wrapper=new RewriteRuleSubtreeStream(adaptor,"rule replace_wrapper");
        RewriteRuleSubtreeStream stream_id_wrapper=new RewriteRuleSubtreeStream(adaptor,"rule id_wrapper");
        RewriteRuleSubtreeStream stream_mln_end=new RewriteRuleSubtreeStream(adaptor,"rule mln_end");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:146:2: ( DEFINE id_wrapper ( mln_end )? ( replace_wrapper )? -> ^( T_DEFINE DEFINE id_wrapper ( replace_wrapper )? ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:146:4: DEFINE id_wrapper ( mln_end )? ( replace_wrapper )?
            {
            DEFINE87=(Token)match(input,DEFINE,FOLLOW_DEFINE_in_define_directive781);  
            stream_DEFINE.add(DEFINE87);


            pushFollow(FOLLOW_id_wrapper_in_define_directive784);
            id_wrapper88=id_wrapper();

            state._fsp--;

            stream_id_wrapper.add(id_wrapper88.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:146:24: ( mln_end )?
            int alt21=2;
            int LA21_0 = input.LA(1);

            if ( (LA21_0==BACKSLASH) ) {
                int LA21_1 = input.LA(2);

                if ( (LA21_1==RET) ) {
                    alt21=1;
                }
            }
            switch (alt21) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:146:24: mln_end
                    {
                    pushFollow(FOLLOW_mln_end_in_define_directive787);
                    mln_end89=mln_end();

                    state._fsp--;

                    stream_mln_end.add(mln_end89.getTree());

                    }
                    break;

            }


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:146:33: ( replace_wrapper )?
            int alt22=2;
            alt22 = dfa22.predict(input);
            switch (alt22) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:146:33: replace_wrapper
                    {
                    pushFollow(FOLLOW_replace_wrapper_in_define_directive790);
                    replace_wrapper90=replace_wrapper();

                    state._fsp--;

                    stream_replace_wrapper.add(replace_wrapper90.getTree());

                    }
                    break;

            }


            // AST REWRITE
            // elements: DEFINE, id_wrapper, replace_wrapper
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 146:50: -> ^( T_DEFINE DEFINE id_wrapper ( replace_wrapper )? )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:146:53: ^( T_DEFINE DEFINE id_wrapper ( replace_wrapper )? )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_DEFINE, "T_DEFINE")
                , root_1);

                adaptor.addChild(root_1, 
                stream_DEFINE.nextNode()
                );

                adaptor.addChild(root_1, stream_id_wrapper.nextTree());

                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:146:83: ( replace_wrapper )?
                if ( stream_replace_wrapper.hasNext() ) {
                    adaptor.addChild(root_1, stream_replace_wrapper.nextTree());

                }
                stream_replace_wrapper.reset();

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "define_directive"


    public static class mln_end_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "mln_end"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:149:1: mln_end : BACKSLASH ret ;
    public final PreprocessorParser.mln_end_return mln_end() throws RecognitionException {
        PreprocessorParser.mln_end_return retval = new PreprocessorParser.mln_end_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token BACKSLASH91=null;
        PreprocessorParser.ret_return ret92 =null;


        Object BACKSLASH91_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:149:9: ( BACKSLASH ret )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:149:11: BACKSLASH ret
            {
            root_0 = (Object)adaptor.nil();


            BACKSLASH91=(Token)match(input,BACKSLASH,FOLLOW_BACKSLASH_in_mln_end816); 
            BACKSLASH91_tree = 
            (Object)adaptor.create(BACKSLASH91)
            ;
            adaptor.addChild(root_0, BACKSLASH91_tree);


            pushFollow(FOLLOW_ret_in_mln_end818);
            ret92=ret();

            state._fsp--;

            adaptor.addChild(root_0, ret92.getTree());

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "mln_end"


    public static class id_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "id_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:151:1: id_wrapper : name_wrapper ( in_brackets_wrapper )? -> ^( T_ID name_wrapper ( in_brackets_wrapper )? ) ;
    public final PreprocessorParser.id_wrapper_return id_wrapper() throws RecognitionException {
        PreprocessorParser.id_wrapper_return retval = new PreprocessorParser.id_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.name_wrapper_return name_wrapper93 =null;

        PreprocessorParser.in_brackets_wrapper_return in_brackets_wrapper94 =null;


        RewriteRuleSubtreeStream stream_name_wrapper=new RewriteRuleSubtreeStream(adaptor,"rule name_wrapper");
        RewriteRuleSubtreeStream stream_in_brackets_wrapper=new RewriteRuleSubtreeStream(adaptor,"rule in_brackets_wrapper");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:152:2: ( name_wrapper ( in_brackets_wrapper )? -> ^( T_ID name_wrapper ( in_brackets_wrapper )? ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:152:4: name_wrapper ( in_brackets_wrapper )?
            {
            pushFollow(FOLLOW_name_wrapper_in_id_wrapper828);
            name_wrapper93=name_wrapper();

            state._fsp--;

            stream_name_wrapper.add(name_wrapper93.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:152:17: ( in_brackets_wrapper )?
            int alt23=2;
            alt23 = dfa23.predict(input);
            switch (alt23) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:152:17: in_brackets_wrapper
                    {
                    pushFollow(FOLLOW_in_brackets_wrapper_in_id_wrapper830);
                    in_brackets_wrapper94=in_brackets_wrapper();

                    state._fsp--;

                    stream_in_brackets_wrapper.add(in_brackets_wrapper94.getTree());

                    }
                    break;

            }


            // AST REWRITE
            // elements: in_brackets_wrapper, name_wrapper
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 152:38: -> ^( T_ID name_wrapper ( in_brackets_wrapper )? )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:152:41: ^( T_ID name_wrapper ( in_brackets_wrapper )? )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_ID, "T_ID")
                , root_1);

                adaptor.addChild(root_1, stream_name_wrapper.nextTree());

                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:152:61: ( in_brackets_wrapper )?
                if ( stream_in_brackets_wrapper.hasNext() ) {
                    adaptor.addChild(root_1, stream_in_brackets_wrapper.nextTree());

                }
                stream_in_brackets_wrapper.reset();

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "id_wrapper"


    public static class name_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "name_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:154:1: name_wrapper : ID -> ^( T_NAME ID ) ;
    public final PreprocessorParser.name_wrapper_return name_wrapper() throws RecognitionException {
        PreprocessorParser.name_wrapper_return retval = new PreprocessorParser.name_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token ID95=null;

        Object ID95_tree=null;
        RewriteRuleTokenStream stream_ID=new RewriteRuleTokenStream(adaptor,"token ID");

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:155:2: ( ID -> ^( T_NAME ID ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:155:4: ID
            {
            ID95=(Token)match(input,ID,FOLLOW_ID_in_name_wrapper853);  
            stream_ID.add(ID95);


            // AST REWRITE
            // elements: ID
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 155:7: -> ^( T_NAME ID )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:155:10: ^( T_NAME ID )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_NAME, "T_NAME")
                , root_1);

                adaptor.addChild(root_1, 
                stream_ID.nextNode()
                );

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "name_wrapper"


    public static class in_brackets_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "in_brackets_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:157:1: in_brackets_wrapper : in_brackets -> ^( T_IN_BRACKETS in_brackets ) ;
    public final PreprocessorParser.in_brackets_wrapper_return in_brackets_wrapper() throws RecognitionException {
        PreprocessorParser.in_brackets_wrapper_return retval = new PreprocessorParser.in_brackets_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.in_brackets_return in_brackets96 =null;


        RewriteRuleSubtreeStream stream_in_brackets=new RewriteRuleSubtreeStream(adaptor,"rule in_brackets");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:158:2: ( in_brackets -> ^( T_IN_BRACKETS in_brackets ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:158:4: in_brackets
            {
            pushFollow(FOLLOW_in_brackets_in_in_brackets_wrapper872);
            in_brackets96=in_brackets();

            state._fsp--;

            stream_in_brackets.add(in_brackets96.getTree());

            // AST REWRITE
            // elements: in_brackets
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 158:16: -> ^( T_IN_BRACKETS in_brackets )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:158:19: ^( T_IN_BRACKETS in_brackets )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_IN_BRACKETS, "T_IN_BRACKETS")
                , root_1);

                adaptor.addChild(root_1, stream_in_brackets.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "in_brackets_wrapper"


    public static class in_brackets_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "in_brackets"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:160:1: in_brackets : L_BR param_wrapper ( ',' param_wrapper )* R_BR ;
    public final PreprocessorParser.in_brackets_return in_brackets() throws RecognitionException {
        PreprocessorParser.in_brackets_return retval = new PreprocessorParser.in_brackets_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token L_BR97=null;
        Token char_literal99=null;
        Token R_BR101=null;
        PreprocessorParser.param_wrapper_return param_wrapper98 =null;

        PreprocessorParser.param_wrapper_return param_wrapper100 =null;


        Object L_BR97_tree=null;
        Object char_literal99_tree=null;
        Object R_BR101_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:161:2: ( L_BR param_wrapper ( ',' param_wrapper )* R_BR )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:161:4: L_BR param_wrapper ( ',' param_wrapper )* R_BR
            {
            root_0 = (Object)adaptor.nil();


            L_BR97=(Token)match(input,L_BR,FOLLOW_L_BR_in_in_brackets891); 
            L_BR97_tree = 
            (Object)adaptor.create(L_BR97)
            ;
            adaptor.addChild(root_0, L_BR97_tree);


            pushFollow(FOLLOW_param_wrapper_in_in_brackets893);
            param_wrapper98=param_wrapper();

            state._fsp--;

            adaptor.addChild(root_0, param_wrapper98.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:161:23: ( ',' param_wrapper )*
            loop24:
            do {
                int alt24=2;
                int LA24_0 = input.LA(1);

                if ( (LA24_0==61) ) {
                    alt24=1;
                }


                switch (alt24) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:161:24: ',' param_wrapper
            	    {
            	    char_literal99=(Token)match(input,61,FOLLOW_61_in_in_brackets896); 
            	    char_literal99_tree = 
            	    (Object)adaptor.create(char_literal99)
            	    ;
            	    adaptor.addChild(root_0, char_literal99_tree);


            	    pushFollow(FOLLOW_param_wrapper_in_in_brackets899);
            	    param_wrapper100=param_wrapper();

            	    state._fsp--;

            	    adaptor.addChild(root_0, param_wrapper100.getTree());

            	    }
            	    break;

            	default :
            	    break loop24;
                }
            } while (true);


            R_BR101=(Token)match(input,R_BR,FOLLOW_R_BR_in_in_brackets903); 
            R_BR101_tree = 
            (Object)adaptor.create(R_BR101)
            ;
            adaptor.addChild(root_0, R_BR101_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "in_brackets"


    public static class param_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "param_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:164:1: param_wrapper : ( param_common | POINTS3 );
    public final PreprocessorParser.param_wrapper_return param_wrapper() throws RecognitionException {
        PreprocessorParser.param_wrapper_return retval = new PreprocessorParser.param_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token POINTS3103=null;
        PreprocessorParser.param_common_return param_common102 =null;


        Object POINTS3103_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:165:2: ( param_common | POINTS3 )
            int alt25=2;
            int LA25_0 = input.LA(1);

            if ( (LA25_0==ID) ) {
                alt25=1;
            }
            else if ( (LA25_0==POINTS3) ) {
                alt25=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 25, 0, input);

                throw nvae;

            }
            switch (alt25) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:165:4: param_common
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_param_common_in_param_wrapper915);
                    param_common102=param_common();

                    state._fsp--;

                    adaptor.addChild(root_0, param_common102.getTree());

                    }
                    break;
                case 2 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:166:4: POINTS3
                    {
                    root_0 = (Object)adaptor.nil();


                    POINTS3103=(Token)match(input,POINTS3,FOLLOW_POINTS3_in_param_wrapper920); 
                    POINTS3103_tree = 
                    (Object)adaptor.create(POINTS3103)
                    ;
                    adaptor.addChild(root_0, POINTS3103_tree);


                    }
                    break;

            }
            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "param_wrapper"


    public static class param_common_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "param_common"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:169:1: param_common : ID -> ^( T_PARAM ID ) ;
    public final PreprocessorParser.param_common_return param_common() throws RecognitionException {
        PreprocessorParser.param_common_return retval = new PreprocessorParser.param_common_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token ID104=null;

        Object ID104_tree=null;
        RewriteRuleTokenStream stream_ID=new RewriteRuleTokenStream(adaptor,"token ID");

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:170:2: ( ID -> ^( T_PARAM ID ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:170:4: ID
            {
            ID104=(Token)match(input,ID,FOLLOW_ID_in_param_common932);  
            stream_ID.add(ID104);


            // AST REWRITE
            // elements: ID
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 170:7: -> ^( T_PARAM ID )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:170:10: ^( T_PARAM ID )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_PARAM, "T_PARAM")
                , root_1);

                adaptor.addChild(root_1, 
                stream_ID.nextNode()
                );

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "param_common"


    public static class replace_wrapper_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "replace_wrapper"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:172:1: replace_wrapper : replace -> ^( T_REPLACE replace ) ;
    public final PreprocessorParser.replace_wrapper_return replace_wrapper() throws RecognitionException {
        PreprocessorParser.replace_wrapper_return retval = new PreprocessorParser.replace_wrapper_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.replace_return replace105 =null;


        RewriteRuleSubtreeStream stream_replace=new RewriteRuleSubtreeStream(adaptor,"rule replace");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:173:2: ( replace -> ^( T_REPLACE replace ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:173:4: replace
            {
            pushFollow(FOLLOW_replace_in_replace_wrapper951);
            replace105=replace();

            state._fsp--;

            stream_replace.add(replace105.getTree());

            // AST REWRITE
            // elements: replace
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 173:12: -> ^( T_REPLACE replace )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:173:15: ^( T_REPLACE replace )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_REPLACE, "T_REPLACE")
                , root_1);

                adaptor.addChild(root_1, stream_replace.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "replace_wrapper"


    public static class replace_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "replace"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:175:1: replace : replace_internal ( backslash ret replace_internal )* ;
    public final PreprocessorParser.replace_return replace() throws RecognitionException {
        PreprocessorParser.replace_return retval = new PreprocessorParser.replace_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.replace_internal_return replace_internal106 =null;

        PreprocessorParser.backslash_return backslash107 =null;

        PreprocessorParser.ret_return ret108 =null;

        PreprocessorParser.replace_internal_return replace_internal109 =null;



        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:175:9: ( replace_internal ( backslash ret replace_internal )* )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:175:11: replace_internal ( backslash ret replace_internal )*
            {
            root_0 = (Object)adaptor.nil();


            pushFollow(FOLLOW_replace_internal_in_replace969);
            replace_internal106=replace_internal();

            state._fsp--;

            adaptor.addChild(root_0, replace_internal106.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:175:28: ( backslash ret replace_internal )*
            loop26:
            do {
                int alt26=2;
                alt26 = dfa26.predict(input);
                switch (alt26) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:175:29: backslash ret replace_internal
            	    {
            	    pushFollow(FOLLOW_backslash_in_replace972);
            	    backslash107=backslash();

            	    state._fsp--;

            	    adaptor.addChild(root_0, backslash107.getTree());

            	    pushFollow(FOLLOW_ret_in_replace974);
            	    ret108=ret();

            	    state._fsp--;

            	    adaptor.addChild(root_0, ret108.getTree());

            	    pushFollow(FOLLOW_replace_internal_in_replace976);
            	    replace_internal109=replace_internal();

            	    state._fsp--;

            	    adaptor.addChild(root_0, replace_internal109.getTree());

            	    }
            	    break;

            	default :
            	    break loop26;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "replace"


    public static class backslash_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "backslash"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:177:1: backslash : BACKSLASH -> ^( T_BACKSLASH BACKSLASH ) ;
    public final PreprocessorParser.backslash_return backslash() throws RecognitionException {
        PreprocessorParser.backslash_return retval = new PreprocessorParser.backslash_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token BACKSLASH110=null;

        Object BACKSLASH110_tree=null;
        RewriteRuleTokenStream stream_BACKSLASH=new RewriteRuleTokenStream(adaptor,"token BACKSLASH");

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:178:2: ( BACKSLASH -> ^( T_BACKSLASH BACKSLASH ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:178:4: BACKSLASH
            {
            BACKSLASH110=(Token)match(input,BACKSLASH,FOLLOW_BACKSLASH_in_backslash988);  
            stream_BACKSLASH.add(BACKSLASH110);


            // AST REWRITE
            // elements: BACKSLASH
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 178:14: -> ^( T_BACKSLASH BACKSLASH )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:178:17: ^( T_BACKSLASH BACKSLASH )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_BACKSLASH, "T_BACKSLASH")
                , root_1);

                adaptor.addChild(root_1, 
                stream_BACKSLASH.nextNode()
                );

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "backslash"


    public static class replace_internal_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "replace_internal"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:180:1: replace_internal : ( '\\'#\\'' | (~ ( BACKSLASH | RET ) )+ ) ;
    public final PreprocessorParser.replace_internal_return replace_internal() throws RecognitionException {
        PreprocessorParser.replace_internal_return retval = new PreprocessorParser.replace_internal_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token string_literal111=null;
        Token set112=null;

        Object string_literal111_tree=null;
        Object set112_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:181:2: ( ( '\\'#\\'' | (~ ( BACKSLASH | RET ) )+ ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:181:4: ( '\\'#\\'' | (~ ( BACKSLASH | RET ) )+ )
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:181:4: ( '\\'#\\'' | (~ ( BACKSLASH | RET ) )+ )
            int alt28=2;
            int LA28_0 = input.LA(1);

            if ( (LA28_0==67) ) {
                alt28=1;
            }
            else if ( (LA28_0==ASTERISK||(LA28_0 >= COMMENT && LA28_0 <= QUOTE)||(LA28_0 >= R_BR && LA28_0 <= 66)||LA28_0==68) ) {
                alt28=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 28, 0, input);

                throw nvae;

            }
            switch (alt28) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:181:5: '\\'#\\''
                    {
                    string_literal111=(Token)match(input,67,FOLLOW_67_in_replace_internal1007); 
                    string_literal111_tree = 
                    (Object)adaptor.create(string_literal111)
                    ;
                    adaptor.addChild(root_0, string_literal111_tree);


                    }
                    break;
                case 2 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:181:15: (~ ( BACKSLASH | RET ) )+
                    {
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:181:15: (~ ( BACKSLASH | RET ) )+
                    int cnt27=0;
                    loop27:
                    do {
                        int alt27=2;
                        alt27 = dfa27.predict(input);
                        switch (alt27) {
                    	case 1 :
                    	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
                    	    {
                    	    set112=(Token)input.LT(1);

                    	    if ( input.LA(1)==ASTERISK||(input.LA(1) >= COMMENT && input.LA(1) <= QUOTE)||(input.LA(1) >= R_BR && input.LA(1) <= 68) ) {
                    	        input.consume();
                    	        adaptor.addChild(root_0, 
                    	        (Object)adaptor.create(set112)
                    	        );
                    	        state.errorRecovery=false;
                    	    }
                    	    else {
                    	        MismatchedSetException mse = new MismatchedSetException(null,input);
                    	        throw mse;
                    	    }


                    	    }
                    	    break;

                    	default :
                    	    if ( cnt27 >= 1 ) break loop27;
                                EarlyExitException eee =
                                    new EarlyExitException(27, input);
                                throw eee;
                        }
                        cnt27++;
                    } while (true);


                    }
                    break;

            }


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "replace_internal"


    public static class expression_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "expression"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:183:1: expression : ex_or -> ^( T_EX_OR ex_or ) ;
    public final PreprocessorParser.expression_return expression() throws RecognitionException {
        PreprocessorParser.expression_return retval = new PreprocessorParser.expression_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.ex_or_return ex_or113 =null;


        RewriteRuleSubtreeStream stream_ex_or=new RewriteRuleSubtreeStream(adaptor,"rule ex_or");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:184:2: ( ex_or -> ^( T_EX_OR ex_or ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:184:4: ex_or
            {
            pushFollow(FOLLOW_ex_or_in_expression1029);
            ex_or113=ex_or();

            state._fsp--;

            stream_ex_or.add(ex_or113.getTree());

            // AST REWRITE
            // elements: ex_or
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 184:10: -> ^( T_EX_OR ex_or )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:184:13: ^( T_EX_OR ex_or )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EX_OR, "T_EX_OR")
                , root_1);

                adaptor.addChild(root_1, stream_ex_or.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "expression"


    public static class ex_or_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_or"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:186:1: ex_or : ex_and_wrp ( '||' expression_wrapper )* ;
    public final PreprocessorParser.ex_or_return ex_or() throws RecognitionException {
        PreprocessorParser.ex_or_return retval = new PreprocessorParser.ex_or_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token string_literal115=null;
        PreprocessorParser.ex_and_wrp_return ex_and_wrp114 =null;

        PreprocessorParser.expression_wrapper_return expression_wrapper116 =null;


        Object string_literal115_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:186:7: ( ex_and_wrp ( '||' expression_wrapper )* )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:186:9: ex_and_wrp ( '||' expression_wrapper )*
            {
            root_0 = (Object)adaptor.nil();


            pushFollow(FOLLOW_ex_and_wrp_in_ex_or1045);
            ex_and_wrp114=ex_and_wrp();

            state._fsp--;

            adaptor.addChild(root_0, ex_and_wrp114.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:186:20: ( '||' expression_wrapper )*
            loop29:
            do {
                int alt29=2;
                int LA29_0 = input.LA(1);

                if ( (LA29_0==68) ) {
                    alt29=1;
                }


                switch (alt29) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:186:21: '||' expression_wrapper
            	    {
            	    string_literal115=(Token)match(input,68,FOLLOW_68_in_ex_or1048); 
            	    string_literal115_tree = 
            	    (Object)adaptor.create(string_literal115)
            	    ;
            	    adaptor.addChild(root_0, string_literal115_tree);


            	    pushFollow(FOLLOW_expression_wrapper_in_ex_or1050);
            	    expression_wrapper116=expression_wrapper();

            	    state._fsp--;

            	    adaptor.addChild(root_0, expression_wrapper116.getTree());

            	    }
            	    break;

            	default :
            	    break loop29;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_or"


    public static class ex_and_wrp_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_and_wrp"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:188:1: ex_and_wrp : ex_and -> ^( T_EX_AND ex_and ) ;
    public final PreprocessorParser.ex_and_wrp_return ex_and_wrp() throws RecognitionException {
        PreprocessorParser.ex_and_wrp_return retval = new PreprocessorParser.ex_and_wrp_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.ex_and_return ex_and117 =null;


        RewriteRuleSubtreeStream stream_ex_and=new RewriteRuleSubtreeStream(adaptor,"rule ex_and");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:189:2: ( ex_and -> ^( T_EX_AND ex_and ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:189:4: ex_and
            {
            pushFollow(FOLLOW_ex_and_in_ex_and_wrp1062);
            ex_and117=ex_and();

            state._fsp--;

            stream_ex_and.add(ex_and117.getTree());

            // AST REWRITE
            // elements: ex_and
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 189:11: -> ^( T_EX_AND ex_and )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:189:14: ^( T_EX_AND ex_and )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EX_AND, "T_EX_AND")
                , root_1);

                adaptor.addChild(root_1, stream_ex_and.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_and_wrp"


    public static class ex_and_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_and"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:191:1: ex_and : ex_not_wrp ( '&&' expression_wrapper )* ;
    public final PreprocessorParser.ex_and_return ex_and() throws RecognitionException {
        PreprocessorParser.ex_and_return retval = new PreprocessorParser.ex_and_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token string_literal119=null;
        PreprocessorParser.ex_not_wrp_return ex_not_wrp118 =null;

        PreprocessorParser.expression_wrapper_return expression_wrapper120 =null;


        Object string_literal119_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:191:8: ( ex_not_wrp ( '&&' expression_wrapper )* )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:191:10: ex_not_wrp ( '&&' expression_wrapper )*
            {
            root_0 = (Object)adaptor.nil();


            pushFollow(FOLLOW_ex_not_wrp_in_ex_and1080);
            ex_not_wrp118=ex_not_wrp();

            state._fsp--;

            adaptor.addChild(root_0, ex_not_wrp118.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:191:21: ( '&&' expression_wrapper )*
            loop30:
            do {
                int alt30=2;
                int LA30_0 = input.LA(1);

                if ( (LA30_0==60) ) {
                    alt30=1;
                }


                switch (alt30) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:191:22: '&&' expression_wrapper
            	    {
            	    string_literal119=(Token)match(input,60,FOLLOW_60_in_ex_and1083); 
            	    string_literal119_tree = 
            	    (Object)adaptor.create(string_literal119)
            	    ;
            	    adaptor.addChild(root_0, string_literal119_tree);


            	    pushFollow(FOLLOW_expression_wrapper_in_ex_and1085);
            	    expression_wrapper120=expression_wrapper();

            	    state._fsp--;

            	    adaptor.addChild(root_0, expression_wrapper120.getTree());

            	    }
            	    break;

            	default :
            	    break loop30;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_and"


    public static class ex_not_wrp_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_not_wrp"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:193:1: ex_not_wrp : ex_not -> ^( T_EX_NOT ex_not ) ;
    public final PreprocessorParser.ex_not_wrp_return ex_not_wrp() throws RecognitionException {
        PreprocessorParser.ex_not_wrp_return retval = new PreprocessorParser.ex_not_wrp_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.ex_not_return ex_not121 =null;


        RewriteRuleSubtreeStream stream_ex_not=new RewriteRuleSubtreeStream(adaptor,"rule ex_not");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:194:2: ( ex_not -> ^( T_EX_NOT ex_not ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:194:4: ex_not
            {
            pushFollow(FOLLOW_ex_not_in_ex_not_wrp1096);
            ex_not121=ex_not();

            state._fsp--;

            stream_ex_not.add(ex_not121.getTree());

            // AST REWRITE
            // elements: ex_not
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 194:11: -> ^( T_EX_NOT ex_not )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:194:14: ^( T_EX_NOT ex_not )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EX_NOT, "T_EX_NOT")
                , root_1);

                adaptor.addChild(root_1, stream_ex_not.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_not_wrp"


    public static class ex_not_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_not"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:196:1: ex_not : ( EXC )? ex_cond ;
    public final PreprocessorParser.ex_not_return ex_not() throws RecognitionException {
        PreprocessorParser.ex_not_return retval = new PreprocessorParser.ex_not_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token EXC122=null;
        PreprocessorParser.ex_cond_return ex_cond123 =null;


        Object EXC122_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:196:8: ( ( EXC )? ex_cond )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:196:10: ( EXC )? ex_cond
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:196:10: ( EXC )?
            int alt31=2;
            int LA31_0 = input.LA(1);

            if ( (LA31_0==EXC) ) {
                alt31=1;
            }
            switch (alt31) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:196:10: EXC
                    {
                    EXC122=(Token)match(input,EXC,FOLLOW_EXC_in_ex_not1112); 
                    EXC122_tree = 
                    (Object)adaptor.create(EXC122)
                    ;
                    adaptor.addChild(root_0, EXC122_tree);


                    }
                    break;

            }


            pushFollow(FOLLOW_ex_cond_in_ex_not1115);
            ex_cond123=ex_cond();

            state._fsp--;

            adaptor.addChild(root_0, ex_cond123.getTree());

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_not"


    public static class ex_cond_wrp_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_cond_wrp"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:198:1: ex_cond_wrp : ex_cond -> ^( T_EX_COND ex_cond ) ;
    public final PreprocessorParser.ex_cond_wrp_return ex_cond_wrp() throws RecognitionException {
        PreprocessorParser.ex_cond_wrp_return retval = new PreprocessorParser.ex_cond_wrp_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.ex_cond_return ex_cond124 =null;


        RewriteRuleSubtreeStream stream_ex_cond=new RewriteRuleSubtreeStream(adaptor,"rule ex_cond");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:199:2: ( ex_cond -> ^( T_EX_COND ex_cond ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:199:4: ex_cond
            {
            pushFollow(FOLLOW_ex_cond_in_ex_cond_wrp1125);
            ex_cond124=ex_cond();

            state._fsp--;

            stream_ex_cond.add(ex_cond124.getTree());

            // AST REWRITE
            // elements: ex_cond
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 199:12: -> ^( T_EX_COND ex_cond )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:199:15: ^( T_EX_COND ex_cond )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EX_COND, "T_EX_COND")
                , root_1);

                adaptor.addChild(root_1, stream_ex_cond.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_cond_wrp"


    public static class ex_cond_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_cond"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:201:1: ex_cond : ex_sngl_wrp ( cond_op_wrp ex_sngl_wrp )? ;
    public final PreprocessorParser.ex_cond_return ex_cond() throws RecognitionException {
        PreprocessorParser.ex_cond_return retval = new PreprocessorParser.ex_cond_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.ex_sngl_wrp_return ex_sngl_wrp125 =null;

        PreprocessorParser.cond_op_wrp_return cond_op_wrp126 =null;

        PreprocessorParser.ex_sngl_wrp_return ex_sngl_wrp127 =null;



        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:201:9: ( ex_sngl_wrp ( cond_op_wrp ex_sngl_wrp )? )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:201:11: ex_sngl_wrp ( cond_op_wrp ex_sngl_wrp )?
            {
            root_0 = (Object)adaptor.nil();


            pushFollow(FOLLOW_ex_sngl_wrp_in_ex_cond1143);
            ex_sngl_wrp125=ex_sngl_wrp();

            state._fsp--;

            adaptor.addChild(root_0, ex_sngl_wrp125.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:201:23: ( cond_op_wrp ex_sngl_wrp )?
            int alt32=2;
            int LA32_0 = input.LA(1);

            if ( (LA32_0==L_UBR||LA32_0==R_UBR||(LA32_0 >= 64 && LA32_0 <= 66)) ) {
                alt32=1;
            }
            switch (alt32) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:201:24: cond_op_wrp ex_sngl_wrp
                    {
                    pushFollow(FOLLOW_cond_op_wrp_in_ex_cond1146);
                    cond_op_wrp126=cond_op_wrp();

                    state._fsp--;

                    adaptor.addChild(root_0, cond_op_wrp126.getTree());

                    pushFollow(FOLLOW_ex_sngl_wrp_in_ex_cond1148);
                    ex_sngl_wrp127=ex_sngl_wrp();

                    state._fsp--;

                    adaptor.addChild(root_0, ex_sngl_wrp127.getTree());

                    }
                    break;

            }


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_cond"


    public static class cond_op_wrp_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "cond_op_wrp"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:203:1: cond_op_wrp : cond_op -> ^( T_COND_OP cond_op ) ;
    public final PreprocessorParser.cond_op_wrp_return cond_op_wrp() throws RecognitionException {
        PreprocessorParser.cond_op_wrp_return retval = new PreprocessorParser.cond_op_wrp_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.cond_op_return cond_op128 =null;


        RewriteRuleSubtreeStream stream_cond_op=new RewriteRuleSubtreeStream(adaptor,"rule cond_op");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:204:2: ( cond_op -> ^( T_COND_OP cond_op ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:204:4: cond_op
            {
            pushFollow(FOLLOW_cond_op_in_cond_op_wrp1160);
            cond_op128=cond_op();

            state._fsp--;

            stream_cond_op.add(cond_op128.getTree());

            // AST REWRITE
            // elements: cond_op
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 204:12: -> ^( T_COND_OP cond_op )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:204:15: ^( T_COND_OP cond_op )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_COND_OP, "T_COND_OP")
                , root_1);

                adaptor.addChild(root_1, stream_cond_op.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "cond_op_wrp"


    public static class cond_op_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "cond_op"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:206:1: cond_op : ( '==' | '<=' | '>=' | L_UBR | R_UBR );
    public final PreprocessorParser.cond_op_return cond_op() throws RecognitionException {
        PreprocessorParser.cond_op_return retval = new PreprocessorParser.cond_op_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token set129=null;

        Object set129_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:206:9: ( '==' | '<=' | '>=' | L_UBR | R_UBR )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            {
            root_0 = (Object)adaptor.nil();


            set129=(Token)input.LT(1);

            if ( input.LA(1)==L_UBR||input.LA(1)==R_UBR||(input.LA(1) >= 64 && input.LA(1) <= 66) ) {
                input.consume();
                adaptor.addChild(root_0, 
                (Object)adaptor.create(set129)
                );
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "cond_op"


    public static class ex_sngl_wrp_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_sngl_wrp"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:208:1: ex_sngl_wrp : ex_sngl -> ^( T_EX_SNGL ex_sngl ) ;
    public final PreprocessorParser.ex_sngl_wrp_return ex_sngl_wrp() throws RecognitionException {
        PreprocessorParser.ex_sngl_wrp_return retval = new PreprocessorParser.ex_sngl_wrp_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.ex_sngl_return ex_sngl130 =null;


        RewriteRuleSubtreeStream stream_ex_sngl=new RewriteRuleSubtreeStream(adaptor,"rule ex_sngl");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:209:2: ( ex_sngl -> ^( T_EX_SNGL ex_sngl ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:209:4: ex_sngl
            {
            pushFollow(FOLLOW_ex_sngl_in_ex_sngl_wrp1203);
            ex_sngl130=ex_sngl();

            state._fsp--;

            stream_ex_sngl.add(ex_sngl130.getTree());

            // AST REWRITE
            // elements: ex_sngl
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 209:12: -> ^( T_EX_SNGL ex_sngl )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:209:15: ^( T_EX_SNGL ex_sngl )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EX_SNGL, "T_EX_SNGL")
                , root_1);

                adaptor.addChild(root_1, stream_ex_sngl.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_sngl_wrp"


    public static class ex_sngl_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "ex_sngl"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:211:1: ex_sngl : ( math_expr_wrp | defined_wrp );
    public final PreprocessorParser.ex_sngl_return ex_sngl() throws RecognitionException {
        PreprocessorParser.ex_sngl_return retval = new PreprocessorParser.ex_sngl_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.math_expr_wrp_return math_expr_wrp131 =null;

        PreprocessorParser.defined_wrp_return defined_wrp132 =null;



        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:211:9: ( math_expr_wrp | defined_wrp )
            int alt33=2;
            int LA33_0 = input.LA(1);

            if ( (LA33_0==ID||(LA33_0 >= MINUS && LA33_0 <= PLUS)) ) {
                alt33=1;
            }
            else if ( (LA33_0==DEFINED||LA33_0==L_BR) ) {
                alt33=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 33, 0, input);

                throw nvae;

            }
            switch (alt33) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:211:11: math_expr_wrp
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_math_expr_wrp_in_ex_sngl1220);
                    math_expr_wrp131=math_expr_wrp();

                    state._fsp--;

                    adaptor.addChild(root_0, math_expr_wrp131.getTree());

                    }
                    break;
                case 2 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:212:4: defined_wrp
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_defined_wrp_in_ex_sngl1226);
                    defined_wrp132=defined_wrp();

                    state._fsp--;

                    adaptor.addChild(root_0, defined_wrp132.getTree());

                    }
                    break;

            }
            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "ex_sngl"


    public static class defined_wrp_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "defined_wrp"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:214:1: defined_wrp : defined_expr -> ^( T_EX_DEF defined_expr ) ;
    public final PreprocessorParser.defined_wrp_return defined_wrp() throws RecognitionException {
        PreprocessorParser.defined_wrp_return retval = new PreprocessorParser.defined_wrp_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.defined_expr_return defined_expr133 =null;


        RewriteRuleSubtreeStream stream_defined_expr=new RewriteRuleSubtreeStream(adaptor,"rule defined_expr");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:215:2: ( defined_expr -> ^( T_EX_DEF defined_expr ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:215:4: defined_expr
            {
            pushFollow(FOLLOW_defined_expr_in_defined_wrp1236);
            defined_expr133=defined_expr();

            state._fsp--;

            stream_defined_expr.add(defined_expr133.getTree());

            // AST REWRITE
            // elements: defined_expr
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 215:17: -> ^( T_EX_DEF defined_expr )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:215:20: ^( T_EX_DEF defined_expr )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EX_DEF, "T_EX_DEF")
                , root_1);

                adaptor.addChild(root_1, stream_defined_expr.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "defined_wrp"


    public static class defined_expr_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "defined_expr"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:217:1: defined_expr : ( DEFINED )? L_BR expression_wrapper ( mln_end )? R_BR ( mln_end )? ;
    public final PreprocessorParser.defined_expr_return defined_expr() throws RecognitionException {
        PreprocessorParser.defined_expr_return retval = new PreprocessorParser.defined_expr_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token DEFINED134=null;
        Token L_BR135=null;
        Token R_BR138=null;
        PreprocessorParser.expression_wrapper_return expression_wrapper136 =null;

        PreprocessorParser.mln_end_return mln_end137 =null;

        PreprocessorParser.mln_end_return mln_end139 =null;


        Object DEFINED134_tree=null;
        Object L_BR135_tree=null;
        Object R_BR138_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:218:2: ( ( DEFINED )? L_BR expression_wrapper ( mln_end )? R_BR ( mln_end )? )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:218:4: ( DEFINED )? L_BR expression_wrapper ( mln_end )? R_BR ( mln_end )?
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:218:4: ( DEFINED )?
            int alt34=2;
            int LA34_0 = input.LA(1);

            if ( (LA34_0==DEFINED) ) {
                alt34=1;
            }
            switch (alt34) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:218:4: DEFINED
                    {
                    DEFINED134=(Token)match(input,DEFINED,FOLLOW_DEFINED_in_defined_expr1255); 
                    DEFINED134_tree = 
                    (Object)adaptor.create(DEFINED134)
                    ;
                    adaptor.addChild(root_0, DEFINED134_tree);


                    }
                    break;

            }


            L_BR135=(Token)match(input,L_BR,FOLLOW_L_BR_in_defined_expr1258); 
            L_BR135_tree = 
            (Object)adaptor.create(L_BR135)
            ;
            adaptor.addChild(root_0, L_BR135_tree);


            pushFollow(FOLLOW_expression_wrapper_in_defined_expr1260);
            expression_wrapper136=expression_wrapper();

            state._fsp--;

            adaptor.addChild(root_0, expression_wrapper136.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:218:38: ( mln_end )?
            int alt35=2;
            int LA35_0 = input.LA(1);

            if ( (LA35_0==BACKSLASH) ) {
                alt35=1;
            }
            switch (alt35) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:218:38: mln_end
                    {
                    pushFollow(FOLLOW_mln_end_in_defined_expr1263);
                    mln_end137=mln_end();

                    state._fsp--;

                    adaptor.addChild(root_0, mln_end137.getTree());

                    }
                    break;

            }


            R_BR138=(Token)match(input,R_BR,FOLLOW_R_BR_in_defined_expr1266); 
            R_BR138_tree = 
            (Object)adaptor.create(R_BR138)
            ;
            adaptor.addChild(root_0, R_BR138_tree);


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:218:53: ( mln_end )?
            int alt36=2;
            int LA36_0 = input.LA(1);

            if ( (LA36_0==BACKSLASH) ) {
                int LA36_1 = input.LA(2);

                if ( (LA36_1==RET) ) {
                    alt36=1;
                }
            }
            switch (alt36) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:218:53: mln_end
                    {
                    pushFollow(FOLLOW_mln_end_in_defined_expr1269);
                    mln_end139=mln_end();

                    state._fsp--;

                    adaptor.addChild(root_0, mln_end139.getTree());

                    }
                    break;

            }


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "defined_expr"


    public static class math_expr_wrp_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "math_expr_wrp"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:220:1: math_expr_wrp : math_expr -> ^( T_EX_MATH math_expr ) ;
    public final PreprocessorParser.math_expr_wrp_return math_expr_wrp() throws RecognitionException {
        PreprocessorParser.math_expr_wrp_return retval = new PreprocessorParser.math_expr_wrp_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.math_expr_return math_expr140 =null;


        RewriteRuleSubtreeStream stream_math_expr=new RewriteRuleSubtreeStream(adaptor,"rule math_expr");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:221:2: ( math_expr -> ^( T_EX_MATH math_expr ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:221:4: math_expr
            {
            pushFollow(FOLLOW_math_expr_in_math_expr_wrp1281);
            math_expr140=math_expr();

            state._fsp--;

            stream_math_expr.add(math_expr140.getTree());

            // AST REWRITE
            // elements: math_expr
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 221:14: -> ^( T_EX_MATH math_expr )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:221:17: ^( T_EX_MATH math_expr )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EX_MATH, "T_EX_MATH")
                , root_1);

                adaptor.addChild(root_1, stream_math_expr.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "math_expr_wrp"


    public static class math_expr_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "math_expr"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:223:1: math_expr : single_expr ( op_wrp single_expr )* ;
    public final PreprocessorParser.math_expr_return math_expr() throws RecognitionException {
        PreprocessorParser.math_expr_return retval = new PreprocessorParser.math_expr_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.single_expr_return single_expr141 =null;

        PreprocessorParser.op_wrp_return op_wrp142 =null;

        PreprocessorParser.single_expr_return single_expr143 =null;



        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:224:2: ( single_expr ( op_wrp single_expr )* )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:224:4: single_expr ( op_wrp single_expr )*
            {
            root_0 = (Object)adaptor.nil();


            pushFollow(FOLLOW_single_expr_in_math_expr1301);
            single_expr141=single_expr();

            state._fsp--;

            adaptor.addChild(root_0, single_expr141.getTree());

            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:224:16: ( op_wrp single_expr )*
            loop37:
            do {
                int alt37=2;
                int LA37_0 = input.LA(1);

                if ( (LA37_0==ASTERISK||LA37_0==MINUS||LA37_0==PLUS) ) {
                    alt37=1;
                }


                switch (alt37) {
            	case 1 :
            	    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:224:17: op_wrp single_expr
            	    {
            	    pushFollow(FOLLOW_op_wrp_in_math_expr1304);
            	    op_wrp142=op_wrp();

            	    state._fsp--;

            	    adaptor.addChild(root_0, op_wrp142.getTree());

            	    pushFollow(FOLLOW_single_expr_in_math_expr1306);
            	    single_expr143=single_expr();

            	    state._fsp--;

            	    adaptor.addChild(root_0, single_expr143.getTree());

            	    }
            	    break;

            	default :
            	    break loop37;
                }
            } while (true);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "math_expr"


    public static class op_wrp_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "op_wrp"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:226:1: op_wrp : op -> ^( T_EX_OP op ) ;
    public final PreprocessorParser.op_wrp_return op_wrp() throws RecognitionException {
        PreprocessorParser.op_wrp_return retval = new PreprocessorParser.op_wrp_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.op_return op144 =null;


        RewriteRuleSubtreeStream stream_op=new RewriteRuleSubtreeStream(adaptor,"rule op");
        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:226:8: ( op -> ^( T_EX_OP op ) )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:226:10: op
            {
            pushFollow(FOLLOW_op_in_op_wrp1317);
            op144=op();

            state._fsp--;

            stream_op.add(op144.getTree());

            // AST REWRITE
            // elements: op
            // token labels: 
            // rule labels: retval
            // token list labels: 
            // rule list labels: 
            // wildcard labels: 
            retval.tree = root_0;
            RewriteRuleSubtreeStream stream_retval=new RewriteRuleSubtreeStream(adaptor,"rule retval",retval!=null?retval.tree:null);

            root_0 = (Object)adaptor.nil();
            // 226:13: -> ^( T_EX_OP op )
            {
                // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:226:16: ^( T_EX_OP op )
                {
                Object root_1 = (Object)adaptor.nil();
                root_1 = (Object)adaptor.becomeRoot(
                (Object)adaptor.create(T_EX_OP, "T_EX_OP")
                , root_1);

                adaptor.addChild(root_1, stream_op.nextTree());

                adaptor.addChild(root_0, root_1);
                }

            }


            retval.tree = root_0;

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "op_wrp"


    public static class op_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "op"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:228:1: op : ( ASTERISK | PLUS | MINUS );
    public final PreprocessorParser.op_return op() throws RecognitionException {
        PreprocessorParser.op_return retval = new PreprocessorParser.op_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token set145=null;

        Object set145_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:228:4: ( ASTERISK | PLUS | MINUS )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
            {
            root_0 = (Object)adaptor.nil();


            set145=(Token)input.LT(1);

            if ( input.LA(1)==ASTERISK||input.LA(1)==MINUS||input.LA(1)==PLUS ) {
                input.consume();
                adaptor.addChild(root_0, 
                (Object)adaptor.create(set145)
                );
                state.errorRecovery=false;
            }
            else {
                MismatchedSetException mse = new MismatchedSetException(null,input);
                throw mse;
            }


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "op"


    public static class single_expr_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "single_expr"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:230:1: single_expr : ( PLUS | MINUS )? single_expr2 ;
    public final PreprocessorParser.single_expr_return single_expr() throws RecognitionException {
        PreprocessorParser.single_expr_return retval = new PreprocessorParser.single_expr_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token set146=null;
        PreprocessorParser.single_expr2_return single_expr2147 =null;


        Object set146_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:231:2: ( ( PLUS | MINUS )? single_expr2 )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:231:4: ( PLUS | MINUS )? single_expr2
            {
            root_0 = (Object)adaptor.nil();


            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:231:4: ( PLUS | MINUS )?
            int alt38=2;
            int LA38_0 = input.LA(1);

            if ( (LA38_0==MINUS||LA38_0==PLUS) ) {
                alt38=1;
            }
            switch (alt38) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:
                    {
                    set146=(Token)input.LT(1);

                    if ( input.LA(1)==MINUS||input.LA(1)==PLUS ) {
                        input.consume();
                        adaptor.addChild(root_0, 
                        (Object)adaptor.create(set146)
                        );
                        state.errorRecovery=false;
                    }
                    else {
                        MismatchedSetException mse = new MismatchedSetException(null,input);
                        throw mse;
                    }


                    }
                    break;

            }


            pushFollow(FOLLOW_single_expr2_in_single_expr1361);
            single_expr2147=single_expr2();

            state._fsp--;

            adaptor.addChild(root_0, single_expr2147.getTree());

            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "single_expr"


    public static class single_expr2_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "single_expr2"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:233:1: single_expr2 : ( name | const_expr );
    public final PreprocessorParser.single_expr2_return single_expr2() throws RecognitionException {
        PreprocessorParser.single_expr2_return retval = new PreprocessorParser.single_expr2_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        PreprocessorParser.name_return name148 =null;

        PreprocessorParser.const_expr_return const_expr149 =null;



        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:234:2: ( name | const_expr )
            int alt39=2;
            int LA39_0 = input.LA(1);

            if ( (LA39_0==ID) ) {
                alt39=1;
            }
            else if ( (LA39_0==NUMBER) ) {
                alt39=2;
            }
            else {
                NoViableAltException nvae =
                    new NoViableAltException("", 39, 0, input);

                throw nvae;

            }
            switch (alt39) {
                case 1 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:234:4: name
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_name_in_single_expr21371);
                    name148=name();

                    state._fsp--;

                    adaptor.addChild(root_0, name148.getTree());

                    }
                    break;
                case 2 :
                    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:235:4: const_expr
                    {
                    root_0 = (Object)adaptor.nil();


                    pushFollow(FOLLOW_const_expr_in_single_expr21376);
                    const_expr149=const_expr();

                    state._fsp--;

                    adaptor.addChild(root_0, const_expr149.getTree());

                    }
                    break;

            }
            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "single_expr2"


    public static class const_expr_return extends ParserRuleReturnScope {
        Object tree;
        public Object getTree() { return tree; }
    };


    // $ANTLR start "const_expr"
    // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:238:1: const_expr : NUMBER ;
    public final PreprocessorParser.const_expr_return const_expr() throws RecognitionException {
        PreprocessorParser.const_expr_return retval = new PreprocessorParser.const_expr_return();
        retval.start = input.LT(1);


        Object root_0 = null;

        Token NUMBER150=null;

        Object NUMBER150_tree=null;

        try {
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:239:2: ( NUMBER )
            // C:\\sandbox\\objc2j\\src\\ru\\andremoniy\\objctojavacnv\\antlr\\Preprocessor.g:239:4: NUMBER
            {
            root_0 = (Object)adaptor.nil();


            NUMBER150=(Token)match(input,NUMBER,FOLLOW_NUMBER_in_const_expr1387); 
            NUMBER150_tree = 
            (Object)adaptor.create(NUMBER150)
            ;
            adaptor.addChild(root_0, NUMBER150_tree);


            }

            retval.stop = input.LT(-1);


            retval.tree = (Object)adaptor.rulePostProcessing(root_0);
            adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop);

        }
        catch (RecognitionException re) {
            reportError(re);
            recover(input,re);
    	retval.tree = (Object)adaptor.errorNode(input, retval.start, input.LT(-1), re);

        }

        finally {
        	// do for sure before leaving
        }
        return retval;
    }
    // $ANTLR end "const_expr"

    // Delegated rules


    protected DFA3 dfa3 = new DFA3(this);
    protected DFA22 dfa22 = new DFA22(this);
    protected DFA23 dfa23 = new DFA23(this);
    protected DFA26 dfa26 = new DFA26(this);
    protected DFA27 dfa27 = new DFA27(this);
    static final String DFA3_eotS =
        "\135\uffff";
    static final String DFA3_eofS =
        "\2\uffff\1\7\5\uffff\2\7\21\uffff\1\7\13\uffff\1\7\65\uffff";
    static final String DFA3_minS =
        "\1\4\1\uffff\1\4\1\10\4\uffff\2\4\13\uffff\1\25\1\10\4\uffff\1\4"+
        "\13\uffff\1\4\13\uffff\1\10\1\25\50\uffff";
    static final String DFA3_maxS =
        "\1\104\1\uffff\1\104\1\31\4\uffff\2\104\13\uffff\1\25\1\31\4\uffff"+
        "\1\104\13\uffff\1\104\13\uffff\1\31\1\25\50\uffff";
    static final String DFA3_acceptS =
        "\1\uffff\1\1\2\uffff\1\4\1\5\1\2\1\6\17\uffff\1\3\41\uffff\4\2\13"+
        "\uffff\3\2\13\uffff\1\3\5\uffff\1\3";
    static final String DFA3_specialS =
        "\135\uffff}>";
    static final String[] DFA3_transitionS = {
            "\3\7\1\1\2\7\3\uffff\2\7\1\3\1\4\1\5\1\6\1\2\61\7",
            "",
            "\22\7\1\10\4\7\1\11\51\7",
            "\1\25\4\uffff\1\26\1\27\6\uffff\1\27\1\uffff\3\27",
            "",
            "",
            "",
            "",
            "\12\7\1\33\66\7",
            "\12\7\1\47\66\7",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\63",
            "\1\64\5\uffff\1\27\6\uffff\1\27\1\uffff\3\27",
            "",
            "",
            "",
            "",
            "\23\7\1\71\6\7\1\74\37\7\1\72\1\73\5\7",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\23\7\1\110\1\7\1\110\1\7\1\112\42\7\1\111\6\7",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\27\4\uffff\1\27\1\126\6\uffff\1\27\1\uffff\3\27",
            "\1\134",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA3_eot = DFA.unpackEncodedString(DFA3_eotS);
    static final short[] DFA3_eof = DFA.unpackEncodedString(DFA3_eofS);
    static final char[] DFA3_min = DFA.unpackEncodedStringToUnsignedChars(DFA3_minS);
    static final char[] DFA3_max = DFA.unpackEncodedStringToUnsignedChars(DFA3_maxS);
    static final short[] DFA3_accept = DFA.unpackEncodedString(DFA3_acceptS);
    static final short[] DFA3_special = DFA.unpackEncodedString(DFA3_specialS);
    static final short[][] DFA3_transition;

    static {
        int numStates = DFA3_transitionS.length;
        DFA3_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA3_transition[i] = DFA.unpackEncodedString(DFA3_transitionS[i]);
        }
    }

    class DFA3 extends DFA {

        public DFA3(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 3;
            this.eot = DFA3_eot;
            this.eof = DFA3_eof;
            this.min = DFA3_min;
            this.max = DFA3_max;
            this.accept = DFA3_accept;
            this.special = DFA3_special;
            this.transition = DFA3_transition;
        }
        public String getDescription() {
            return "50:1: code_fragment : ( define_directive | import_declaration | if_simple_check_wrapper | if_define_block_wrapper | if_not_define_block_wrapper | smth_else );";
        }
    }
    static final String DFA22_eotS =
        "\u0493\uffff";
    static final String DFA22_eofS =
        "\1\3\1\uffff\1\14\2\uffff\3\26\1\uffff\2\26\2\uffff\1\146\16\uffff"+
        "\2\146\10\uffff\1\146\4\uffff\1\146\1\u00b5\1\u00c6\11\uffff\1\u00c6"+
        "\15\uffff\1\u00ea\15\uffff\1\u00f7\4\uffff\1\u00f7\26\uffff\1\u00f7"+
        "\4\uffff\2\u00f7\1\u0141\2\u0152\1\uffff\1\u0152\15\uffff\1\u0152"+
        "\13\uffff\1\u0152\1\uffff\1\u0152\1\u01b4\1\u01c5\14\uffff\1\u01c5"+
        "\1\uffff\1\u01e9\14\uffff\1\u01fa\1\uffff\3\u01fa\14\uffff\1\u01fa"+
        "\1\uffff\3\u01fa\14\uffff\1\u01fa\1\uffff\5\u01fa\46\uffff\1\u01fa"+
        "\15\uffff\1\u0304\15\uffff\1\u0314\1\uffff\1\u0314\1\u0331\2\u0342"+
        "\14\uffff\1\u0342\1\uffff\1\u0374\14\uffff\1\u0385\1\uffff\3\u0385"+
        "\14\uffff\1\u0385\1\uffff\3\u0385\14\uffff\1\u0385\15\uffff\1\u0385"+
        "\1\uffff\5\u0385\u031c\uffff";
    static final String DFA22_minS =
        "\1\4\1\uffff\1\4\2\uffff\3\4\1\uffff\2\4\2\uffff\1\4\16\uffff\2"+
        "\4\10\uffff\1\4\4\uffff\3\4\11\uffff\1\4\15\uffff\1\4\15\uffff\1"+
        "\4\4\uffff\1\4\26\uffff\1\4\4\uffff\5\4\1\uffff\1\4\15\uffff\1\4"+
        "\13\uffff\1\4\1\uffff\3\4\14\uffff\1\4\1\uffff\1\4\14\uffff\1\4"+
        "\1\uffff\3\4\14\uffff\1\4\1\uffff\3\4\14\uffff\1\4\1\uffff\5\4\46"+
        "\uffff\1\4\15\uffff\1\4\15\uffff\1\4\1\uffff\4\4\14\uffff\1\4\1"+
        "\uffff\1\4\14\uffff\1\4\1\uffff\3\4\14\uffff\1\4\1\uffff\3\4\14"+
        "\uffff\1\4\15\uffff\1\4\1\uffff\5\4\u031c\uffff";
    static final String DFA22_maxS =
        "\1\104\1\uffff\1\104\2\uffff\3\104\1\uffff\2\104\2\uffff\1\104\16"+
        "\uffff\2\104\10\uffff\1\104\4\uffff\3\104\11\uffff\1\104\15\uffff"+
        "\1\104\15\uffff\1\104\4\uffff\1\104\26\uffff\1\104\4\uffff\5\104"+
        "\1\uffff\1\104\15\uffff\1\104\13\uffff\1\104\1\uffff\3\104\14\uffff"+
        "\1\104\1\uffff\1\104\14\uffff\1\104\1\uffff\3\104\14\uffff\1\104"+
        "\1\uffff\3\104\14\uffff\1\104\1\uffff\5\104\46\uffff\1\104\15\uffff"+
        "\1\104\15\uffff\1\104\1\uffff\4\104\14\uffff\1\104\1\uffff\1\104"+
        "\14\uffff\1\104\1\uffff\3\104\14\uffff\1\104\1\uffff\3\104\14\uffff"+
        "\1\104\15\uffff\1\104\1\uffff\5\104\u031c\uffff";
    static final String DFA22_acceptS =
        "\1\uffff\1\1\1\uffff\1\2\1\1\3\uffff\1\1\3\uffff\1\1\11\uffff\1"+
        "\1\117\uffff\1\1\116\uffff\1\1\20\uffff\1\1\43\uffff\1\1\14\uffff"+
        "\1\1\111\uffff\1\1\20\uffff\1\1\45\uffff\1\1\13\uffff\1\1\1\uffff"+
        "\4\1\1\uffff\1\1\30\uffff\1\1\1\uffff\1\1\14\uffff\5\1\14\uffff"+
        "\5\1\14\uffff\1\1\1\uffff\5\1\14\uffff\5\1\14\uffff\5\1\14\uffff"+
        "\1\1\1\uffff\2\1\14\uffff\1\1\1\uffff\4\1\14\uffff\1\1\1\uffff\5"+
        "\1\14\uffff\1\1\1\uffff\5\1\14\uffff\1\1\1\uffff\2\1\14\uffff\1"+
        "\1\1\uffff\4\1\14\uffff\1\1\1\uffff\5\1\14\uffff\1\1\1\uffff\5\1"+
        "\14\uffff\1\1\1\uffff\4\1\14\uffff\1\1\1\uffff\1\1\10\uffff\1\1"+
        "\3\uffff\1\1\1\uffff\4\1\10\uffff\1\1\3\uffff\1\1\1\uffff\4\1\14"+
        "\uffff\1\1\15\uffff\1\1\1\uffff\5\1\14\uffff\1\1\1\uffff\3\1\14"+
        "\uffff\1\1\1\uffff\2\1\14\uffff\1\1\1\uffff\1\1\14\uffff\5\1\14"+
        "\uffff\5\1\14\uffff\1\1\15\uffff\1\1\1\uffff\5\1\14\uffff\5\1\14"+
        "\uffff\5\1\14\uffff\1\1\1\uffff\2\1\14\uffff\1\1\1\uffff\4\1\14"+
        "\uffff\1\1\1\uffff\5\1\14\uffff\1\1\1\uffff\5\1\14\uffff\1\1\1\uffff"+
        "\2\1\14\uffff\1\1\1\uffff\4\1\14\uffff\1\1\1\uffff\5\1\14\uffff"+
        "\1\1\1\uffff\5\1\14\uffff\1\1\1\uffff\5\1\14\uffff\1\1\1\uffff\4"+
        "\1\14\uffff\1\1\1\uffff\1\1\10\uffff\1\1\3\uffff\1\1\1\uffff\4\1"+
        "\10\uffff\1\1\3\uffff\1\1\1\uffff\4\1\14\uffff\1\1\15\uffff\1\1"+
        "\1\uffff\5\1\1\uffff";
    static final String DFA22_specialS =
        "\u0493\uffff}>";
    static final String[] DFA22_transitionS = {
            "\1\10\1\3\1\10\1\2\2\10\1\15\1\12\1\14\2\10\1\5\1\6\1\7\1\11"+
            "\1\4\10\10\1\3\46\10\1\1\1\10",
            "",
            "\1\26\1\14\1\26\1\14\7\26\5\14\61\26",
            "",
            "",
            "\4\26\1\34\4\26\1\35\1\53\6\26\1\55\1\26\1\46\1\54\1\46\53"+
            "\26",
            "\12\26\1\67\66\26",
            "\12\26\1\105\66\26",
            "",
            "\22\26\1\123\4\26\1\130\51\26",
            "\30\26\1\146\50\26",
            "",
            "",
            "\4\146\1\167\4\146\1\157\1\165\6\146\1\170\1\146\1\164\1\166"+
            "\1\164\53\146",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\21\146\1\172\57\146",
            "\4\146\1\u0088\5\146\1\u0096\6\146\1\u0098\1\146\1\u0094\1"+
            "\u0097\1\u0094\53\146",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\146\1\u00a5\11\146\1\u00a7\54\146",
            "",
            "",
            "",
            "",
            "\1\u00b4\1\146\1\u00b5\1\146\2\u00b5\3\146\2\u00b5\5\146\2"+
            "\u00b5\1\u00b6\1\u00b4\1\u00b5\1\u00b4\4\u00b5\1\u00b6\35\u00b5"+
            "\1\u00b7\3\u00b5\3\u00b6\1\u00b5\1\u00b8",
            "\1\u00c5\1\u00b5\1\u00c6\1\u00b5\2\u00c6\3\u00b5\2\u00c6\5"+
            "\u00b5\2\u00c6\1\u00c7\1\u00c5\1\u00c6\1\u00c5\4\u00c6\1\u00c7"+
            "\35\u00c6\1\u00c8\3\u00c6\3\u00c7\1\u00c6\1\u00c9",
            "\4\u00c6\1\u00db\4\u00c6\1\u00d6\1\u00d9\6\u00c6\1\u00dc\1"+
            "\u00c6\1\u00d8\1\u00da\1\u00d8\53\u00c6",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\30\u00c6\1\u00ea\50\u00c6",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\30\u00ea\1\u00f7\50\u00ea",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u00f7\1\u0103\66\u00f7",
            "",
            "",
            "",
            "",
            "\12\u00f7\1\u0111\66\u00f7",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u00f7\1\u0123\5\u00f7\1\u0121\6\u00f7\1\u0124\1\u00f7\1"+
            "\u011f\1\u0122\1\u011f\53\u00f7",
            "",
            "",
            "",
            "",
            "\12\u00f7\1\u0131\11\u00f7\1\u0133\54\u00f7",
            "\1\u0140\1\u00f7\1\u0141\1\u00f7\2\u0141\3\u00f7\2\u0141\5"+
            "\u00f7\2\u0141\1\u0142\1\u0140\1\u0141\1\u0140\4\u0141\1\u0142"+
            "\35\u0141\1\u0143\3\u0141\3\u0142\1\u0141\1\u0144",
            "\1\u0151\1\u0141\1\u0152\1\u0141\2\u0152\3\u0141\2\u0152\5"+
            "\u0141\2\u0152\1\u0153\1\u0151\1\u0152\1\u0151\4\u0152\1\u0153"+
            "\35\u0152\1\u0154\3\u0152\3\u0153\1\u0152\1\u0155",
            "\21\u0152\1\u0162\57\u0152",
            "\4\u0152\1\u0175\4\u0152\1\u0170\1\u0173\6\u0152\1\u0176\1"+
            "\u0152\1\u0172\1\u0174\1\u0172\53\u0152",
            "",
            "\4\u0152\1\u0188\4\u0152\1\u0184\1\u0178\6\u0152\1\u0189\1"+
            "\u0152\1\u0186\1\u0187\1\u0186\53\u0152",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\21\u0152\1\u018b\57\u0152",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u0152\1\u01a4\11\u0152\1\u01a6\54\u0152",
            "",
            "\1\u01b3\1\u0152\1\u01b4\1\u0152\2\u01b4\3\u0152\2\u01b4\5"+
            "\u0152\2\u01b4\1\u01b5\1\u01b3\1\u01b4\1\u01b3\4\u01b4\1\u01b5"+
            "\35\u01b4\1\u01b6\3\u01b4\3\u01b5\1\u01b4\1\u01b7",
            "\1\u01c4\1\u01b4\1\u01c5\1\u01b4\2\u01c5\3\u01b4\2\u01c5\5"+
            "\u01b4\2\u01c5\1\u01c6\1\u01c4\1\u01c5\1\u01c4\4\u01c5\1\u01c6"+
            "\35\u01c5\1\u01c7\3\u01c5\3\u01c6\1\u01c5\1\u01c8",
            "\4\u01c5\1\u01da\4\u01c5\1\u01d5\1\u01d8\6\u01c5\1\u01db\1"+
            "\u01c5\1\u01d7\1\u01d9\1\u01d7\53\u01c5",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u01e8\1\u01c5\1\u01e9\1\u01c5\2\u01e9\3\u01c5\2\u01e9\5"+
            "\u01c5\2\u01e9\1\u01ea\1\u01e8\1\u01e9\1\u01e8\4\u01e9\1\u01ea"+
            "\35\u01e9\1\u01eb\3\u01e9\3\u01ea\1\u01e9\1\u01ec",
            "",
            "\1\u01f9\1\u01e9\1\u01fa\1\u01e9\2\u01fa\3\u01e9\2\u01fa\5"+
            "\u01e9\2\u01fa\1\u01fb\1\u01f9\1\u01fa\1\u01f9\4\u01fa\1\u01fb"+
            "\35\u01fa\1\u01fc\3\u01fa\3\u01fb\1\u01fa\1\u01fd",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u01fa\1\u020c\10\u01fa\1\u020a\1\u020d\1\u020a\53\u01fa",
            "",
            "\4\u01fa\1\u021e\5\u01fa\1\u021c\6\u01fa\1\u021f\1\u01fa\1"+
            "\u021a\1\u021d\1\u021a\53\u01fa",
            "\4\u01fa\1\u0231\4\u01fa\1\u022c\1\u022f\6\u01fa\1\u0232\1"+
            "\u01fa\1\u022e\1\u0230\1\u022e\53\u01fa",
            "\4\u01fa\1\u0244\4\u01fa\1\u023f\1\u0242\6\u01fa\1\u0245\1"+
            "\u01fa\1\u0241\1\u0243\1\u0241\53\u01fa",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u01fa\1\u0254\10\u01fa\1\u0252\1\u0255\1\u0252\53\u01fa",
            "",
            "\4\u01fa\1\u0266\5\u01fa\1\u0264\6\u01fa\1\u0267\1\u01fa\1"+
            "\u0262\1\u0265\1\u0262\53\u01fa",
            "\4\u01fa\1\u0279\4\u01fa\1\u0274\1\u0277\6\u01fa\1\u027a\1"+
            "\u01fa\1\u0276\1\u0278\1\u0276\53\u01fa",
            "\4\u01fa\1\u028c\4\u01fa\1\u0287\1\u028a\6\u01fa\1\u028d\1"+
            "\u01fa\1\u0289\1\u028b\1\u0289\53\u01fa",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u01fa\1\u029e\5\u01fa\1\u029c\6\u01fa\1\u029f\1\u01fa\1"+
            "\u029a\1\u029d\1\u029a\53\u01fa",
            "",
            "\12\u01fa\1\u02ac\11\u01fa\1\u02ae\54\u01fa",
            "\1\u02bb\1\u02b7\20\u01fa\1\u02bd\1\u02bb\1\u01fa\1\u02bb\3"+
            "\u01fa\1\u02c0\1\u02bd\35\u01fa\1\u02be\3\u01fa\3\u02bd\1\u01fa"+
            "\1\u02bf",
            "\1\u02cd\1\u02c9\20\u01fa\1\u02cf\1\u02cd\1\u01fa\1\u02cd\3"+
            "\u01fa\1\u02d2\1\u02cf\35\u01fa\1\u02d0\3\u01fa\3\u02cf\1\u01fa"+
            "\1\u02d1",
            "\21\u01fa\1\u02df\57\u01fa",
            "\4\u01fa\1\u02f2\4\u01fa\1\u02ed\1\u02f0\6\u01fa\1\u02f3\1"+
            "\u01fa\1\u02ef\1\u02f1\1\u02ef\53\u01fa",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u0304\1\u01fa\1\u0304\1\u01fa\2\u0304\3\u01fa\2\u0304\5"+
            "\u01fa\3\u0304\1\u0300\4\u0304\1\u01fa\41\u0304\1\u0302\1\u0303"+
            "\5\u0304",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u0314\1\u0304\1\u0314\1\u0304\2\u0314\3\u0304\2\u0314\5"+
            "\u0304\3\u0314\1\u0311\1\u0314\1\u0311\2\u0314\1\u0304\41\u0314"+
            "\1\u0313\6\u0314",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u0314\1\u0321\11\u0314\1\u0323\54\u0314",
            "",
            "\1\u0330\1\u0314\1\u0331\1\u0314\2\u0331\3\u0314\2\u0331\5"+
            "\u0314\2\u0331\1\u0332\1\u0330\1\u0331\1\u0330\4\u0331\1\u0332"+
            "\35\u0331\1\u0333\3\u0331\3\u0332\1\u0331\1\u0334",
            "\1\u0341\1\u0331\1\u0342\1\u0331\2\u0342\3\u0331\2\u0342\5"+
            "\u0331\2\u0342\1\u0343\1\u0341\1\u0342\1\u0341\4\u0342\1\u0343"+
            "\35\u0342\1\u0344\3\u0342\3\u0343\1\u0342\1\u0345",
            "\21\u0342\1\u0352\57\u0342",
            "\4\u0342\1\u0365\4\u0342\1\u0360\1\u0363\6\u0342\1\u0366\1"+
            "\u0342\1\u0362\1\u0364\1\u0362\53\u0342",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u0373\1\u0342\1\u0374\1\u0342\2\u0374\3\u0342\2\u0374\5"+
            "\u0342\2\u0374\1\u0375\1\u0373\1\u0374\1\u0373\4\u0374\1\u0375"+
            "\35\u0374\1\u0376\3\u0374\3\u0375\1\u0374\1\u0377",
            "",
            "\1\u0384\1\u0374\1\u0385\1\u0374\2\u0385\3\u0374\2\u0385\5"+
            "\u0374\2\u0385\1\u0386\1\u0384\1\u0385\1\u0384\4\u0385\1\u0386"+
            "\35\u0385\1\u0387\3\u0385\3\u0386\1\u0385\1\u0388",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u0385\1\u0397\10\u0385\1\u0395\1\u0398\1\u0395\53\u0385",
            "",
            "\4\u0385\1\u03a9\5\u0385\1\u03a7\6\u0385\1\u03aa\1\u0385\1"+
            "\u03a5\1\u03a8\1\u03a5\53\u0385",
            "\4\u0385\1\u03bc\4\u0385\1\u03b7\1\u03ba\6\u0385\1\u03bd\1"+
            "\u0385\1\u03b9\1\u03bb\1\u03b9\53\u0385",
            "\4\u0385\1\u03cf\4\u0385\1\u03ca\1\u03cd\6\u0385\1\u03d0\1"+
            "\u0385\1\u03cc\1\u03ce\1\u03cc\53\u0385",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u0385\1\u03df\10\u0385\1\u03dd\1\u03e0\1\u03dd\53\u0385",
            "",
            "\4\u0385\1\u03f1\5\u0385\1\u03ef\6\u0385\1\u03f2\1\u0385\1"+
            "\u03ed\1\u03f0\1\u03ed\53\u0385",
            "\4\u0385\1\u0404\4\u0385\1\u03ff\1\u0402\6\u0385\1\u0405\1"+
            "\u0385\1\u0401\1\u0403\1\u0401\53\u0385",
            "\4\u0385\1\u0417\4\u0385\1\u0412\1\u0415\6\u0385\1\u0418\1"+
            "\u0385\1\u0414\1\u0416\1\u0414\53\u0385",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u0385\1\u042a\4\u0385\1\u0425\1\u0428\6\u0385\1\u042b\1"+
            "\u0385\1\u0427\1\u0429\1\u0427\53\u0385",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u0385\1\u043c\5\u0385\1\u043a\6\u0385\1\u043d\1\u0385\1"+
            "\u0438\1\u043b\1\u0438\53\u0385",
            "",
            "\12\u0385\1\u044a\11\u0385\1\u044c\54\u0385",
            "\1\u0459\1\u0455\20\u0385\1\u045b\1\u0459\1\u0385\1\u0459\3"+
            "\u0385\1\u045e\1\u045b\35\u0385\1\u045c\3\u0385\3\u045b\1\u0385"+
            "\1\u045d",
            "\1\u046b\1\u0467\20\u0385\1\u046d\1\u046b\1\u0385\1\u046b\3"+
            "\u0385\1\u0470\1\u046d\35\u0385\1\u046e\3\u0385\3\u046d\1\u0385"+
            "\1\u046f",
            "\21\u0385\1\u047d\57\u0385",
            "\4\u0385\1\u0490\4\u0385\1\u048b\1\u048e\6\u0385\1\u0491\1"+
            "\u0385\1\u048d\1\u048f\1\u048d\53\u0385",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA22_eot = DFA.unpackEncodedString(DFA22_eotS);
    static final short[] DFA22_eof = DFA.unpackEncodedString(DFA22_eofS);
    static final char[] DFA22_min = DFA.unpackEncodedStringToUnsignedChars(DFA22_minS);
    static final char[] DFA22_max = DFA.unpackEncodedStringToUnsignedChars(DFA22_maxS);
    static final short[] DFA22_accept = DFA.unpackEncodedString(DFA22_acceptS);
    static final short[] DFA22_special = DFA.unpackEncodedString(DFA22_specialS);
    static final short[][] DFA22_transition;

    static {
        int numStates = DFA22_transitionS.length;
        DFA22_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA22_transition[i] = DFA.unpackEncodedString(DFA22_transitionS[i]);
        }
    }

    class DFA22 extends DFA {

        public DFA22(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 22;
            this.eot = DFA22_eot;
            this.eof = DFA22_eof;
            this.min = DFA22_min;
            this.max = DFA22_max;
            this.accept = DFA22_accept;
            this.special = DFA22_special;
            this.transition = DFA22_transition;
        }
        public String getDescription() {
            return "146:33: ( replace_wrapper )?";
        }
    }
    static final String DFA23_eotS =
        "\133\uffff";
    static final String DFA23_eofS =
        "\2\2\16\uffff\2\2\15\uffff\1\2\16\uffff\1\2\54\uffff";
    static final String DFA23_minS =
        "\2\4\16\uffff\2\4\15\uffff\1\4\16\uffff\1\4\54\uffff";
    static final String DFA23_maxS =
        "\2\104\16\uffff\2\104\15\uffff\1\104\16\uffff\1\104\54\uffff";
    static final String DFA23_acceptS =
        "\2\uffff\1\2\35\uffff\1\1\16\uffff\1\1\15\uffff\2\1\15\uffff\2\1"+
        "\15\uffff";
    static final String DFA23_specialS =
        "\133\uffff}>";
    static final String[] DFA23_transitionS = {
            "\21\2\1\1\57\2",
            "\12\2\1\20\13\2\1\21\52\2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\31\2\1\40\37\2\1\37\7\2",
            "\31\2\1\57\37\2\1\56\7\2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\2\1\75\13\2\1\76\52\2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\2\1\114\13\2\1\115\52\2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA23_eot = DFA.unpackEncodedString(DFA23_eotS);
    static final short[] DFA23_eof = DFA.unpackEncodedString(DFA23_eofS);
    static final char[] DFA23_min = DFA.unpackEncodedStringToUnsignedChars(DFA23_minS);
    static final char[] DFA23_max = DFA.unpackEncodedStringToUnsignedChars(DFA23_maxS);
    static final short[] DFA23_accept = DFA.unpackEncodedString(DFA23_acceptS);
    static final short[] DFA23_special = DFA.unpackEncodedString(DFA23_specialS);
    static final short[][] DFA23_transition;

    static {
        int numStates = DFA23_transitionS.length;
        DFA23_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA23_transition[i] = DFA.unpackEncodedString(DFA23_transitionS[i]);
        }
    }

    class DFA23 extends DFA {

        public DFA23(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 23;
            this.eot = DFA23_eot;
            this.eof = DFA23_eof;
            this.min = DFA23_min;
            this.max = DFA23_max;
            this.accept = DFA23_accept;
            this.special = DFA23_special;
            this.transition = DFA23_transition;
        }
        public String getDescription() {
            return "152:17: ( in_brackets_wrapper )?";
        }
    }
    static final String DFA26_eotS =
        "\u00a1\uffff";
    static final String DFA26_eofS =
        "\1\1\7\uffff\1\1\13\uffff\1\1\5\uffff\1\47\1\uffff\4\47\1\uffff"+
        "\1\163\1\uffff\1\163\1\uffff\1\1\173\uffff";
    static final String DFA26_minS =
        "\1\4\7\uffff\1\4\13\uffff\1\4\5\uffff\1\4\1\uffff\4\4\1\uffff\1"+
        "\4\1\uffff\1\4\1\uffff\1\4\173\uffff";
    static final String DFA26_maxS =
        "\1\104\7\uffff\1\104\13\uffff\1\104\5\uffff\1\104\1\uffff\4\104"+
        "\1\uffff\1\104\1\uffff\1\104\1\uffff\1\104\173\uffff";
    static final String DFA26_acceptS =
        "\1\uffff\1\2\31\uffff\1\1\4\uffff\1\1\1\uffff\1\1\1\uffff\1\1\2"+
        "\uffff\1\1\15\uffff\6\1\15\uffff\1\1\15\uffff\1\1\15\uffff\2\1\15"+
        "\uffff\1\1\14\uffff\6\1\16\uffff\14\1\1\uffff";
    static final String DFA26_specialS =
        "\u00a1\uffff}>";
    static final String[] DFA26_transitionS = {
            "\1\1\1\10\77\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\30\1\1\24\50\1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\44\1\1\1\44\1\32\2\44\1\43\1\41\1\42\2\44\1\34\1\35\1\36"+
            "\1\37\1\33\10\44\1\45\46\44\1\40\1\44",
            "",
            "",
            "",
            "",
            "",
            "\101\47",
            "",
            "\4\47\1\65\4\47\1\66\1\70\6\47\1\72\1\47\1\67\1\71\1\67\53"+
            "\47",
            "\12\47\1\110\66\47",
            "\12\47\1\126\66\47",
            "\22\47\1\144\4\47\1\145\51\47",
            "",
            "\101\163",
            "",
            "\4\163\1\u0084\4\163\1\u0080\1\u0082\6\163\1\u0085\1\163\1"+
            "\u0081\1\u0083\1\u0081\53\163",
            "",
            "\1\u009e\1\1\1\u009e\1\u0094\2\u009e\1\u009d\1\u009b\1\u009c"+
            "\2\u009e\1\u0096\1\u0097\1\u0098\1\u0099\1\u0095\10\u009e\1"+
            "\u009f\46\u009e\1\u009a\1\u009e",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA26_eot = DFA.unpackEncodedString(DFA26_eotS);
    static final short[] DFA26_eof = DFA.unpackEncodedString(DFA26_eofS);
    static final char[] DFA26_min = DFA.unpackEncodedStringToUnsignedChars(DFA26_minS);
    static final char[] DFA26_max = DFA.unpackEncodedStringToUnsignedChars(DFA26_maxS);
    static final short[] DFA26_accept = DFA.unpackEncodedString(DFA26_acceptS);
    static final short[] DFA26_special = DFA.unpackEncodedString(DFA26_specialS);
    static final short[][] DFA26_transition;

    static {
        int numStates = DFA26_transitionS.length;
        DFA26_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA26_transition[i] = DFA.unpackEncodedString(DFA26_transitionS[i]);
        }
    }

    class DFA26 extends DFA {

        public DFA26(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 26;
            this.eot = DFA26_eot;
            this.eof = DFA26_eof;
            this.min = DFA26_min;
            this.max = DFA26_max;
            this.accept = DFA26_accept;
            this.special = DFA26_special;
            this.transition = DFA26_transition;
        }
        public String getDescription() {
            return "()* loopback of 175:28: ( backslash ret replace_internal )*";
        }
    }
    static final String DFA27_eotS =
        "\u0493\uffff";
    static final String DFA27_eofS =
        "\1\1\2\uffff\1\16\1\uffff\4\16\1\uffff\1\132\1\uffff\1\132\17\uffff"+
        "\3\132\1\u00ad\2\u00be\15\uffff\1\u00de\15\uffff\1\u00eb\15\uffff"+
        "\2\u00eb\32\uffff\2\u00eb\1\u0139\3\u014a\15\uffff\1\u014a\15\uffff"+
        "\2\u014a\1\u01ac\2\u01bd\15\uffff\1\u01e1\1\u01f2\15\uffff\4\u01f2"+
        "\15\uffff\4\u01f2\15\uffff\6\u01f2\47\uffff\1\u02f8\15\uffff\1\u0308"+
        "\15\uffff\1\u0308\1\u0329\3\u033a\15\uffff\1\u036c\1\u037d\15\uffff"+
        "\4\u037d\15\uffff\4\u037d\15\uffff\1\u037d\15\uffff\6\u037d\u0328"+
        "\uffff";
    static final String DFA27_minS =
        "\1\4\2\uffff\1\4\1\uffff\4\4\1\uffff\1\4\1\uffff\1\4\17\uffff\6"+
        "\4\15\uffff\1\4\15\uffff\1\4\15\uffff\2\4\32\uffff\6\4\15\uffff"+
        "\1\4\15\uffff\5\4\15\uffff\2\4\15\uffff\4\4\15\uffff\4\4\15\uffff"+
        "\6\4\47\uffff\1\4\15\uffff\1\4\15\uffff\5\4\15\uffff\2\4\15\uffff"+
        "\4\4\15\uffff\4\4\15\uffff\1\4\15\uffff\6\4\u0328\uffff";
    static final String DFA27_maxS =
        "\1\104\2\uffff\1\104\1\uffff\4\104\1\uffff\1\104\1\uffff\1\104\17"+
        "\uffff\6\104\15\uffff\1\104\15\uffff\1\104\15\uffff\2\104\32\uffff"+
        "\6\104\15\uffff\1\104\15\uffff\5\104\15\uffff\2\104\15\uffff\4\104"+
        "\15\uffff\4\104\15\uffff\6\104\47\uffff\1\104\15\uffff\1\104\15"+
        "\uffff\5\104\15\uffff\2\104\15\uffff\4\104\15\uffff\4\104\15\uffff"+
        "\1\104\15\uffff\6\104\u0328\uffff";
    static final String DFA27_acceptS =
        "\1\uffff\1\2\2\uffff\1\1\4\uffff\1\1\1\uffff\1\1\2\uffff\1\1\113"+
        "\uffff\1\1\122\uffff\1\1\20\uffff\1\1\37\uffff\1\1\14\uffff\1\1"+
        "\115\uffff\1\1\20\uffff\1\1\55\uffff\6\1\15\uffff\1\1\15\uffff\2"+
        "\1\15\uffff\5\1\14\uffff\5\1\14\uffff\6\1\15\uffff\5\1\14\uffff"+
        "\5\1\14\uffff\3\1\15\uffff\5\1\15\uffff\6\1\15\uffff\6\1\15\uffff"+
        "\3\1\15\uffff\5\1\15\uffff\6\1\15\uffff\6\1\15\uffff\5\1\15\uffff"+
        "\2\1\15\uffff\6\1\14\uffff\6\1\14\uffff\1\1\15\uffff\6\1\15\uffff"+
        "\4\1\15\uffff\3\1\15\uffff\2\1\15\uffff\5\1\14\uffff\5\1\14\uffff"+
        "\1\1\15\uffff\6\1\15\uffff\5\1\14\uffff\5\1\14\uffff\3\1\15\uffff"+
        "\5\1\15\uffff\6\1\15\uffff\6\1\15\uffff\3\1\15\uffff\5\1\15\uffff"+
        "\6\1\15\uffff\6\1\15\uffff\6\1\15\uffff\5\1\15\uffff\2\1\15\uffff"+
        "\6\1\14\uffff\6\1\14\uffff\1\1\15\uffff\6\1\15\uffff";
    static final String DFA27_specialS =
        "\u0493\uffff}>";
    static final String[] DFA27_transitionS = {
            "\1\11\1\1\1\11\1\3\2\11\1\14\1\12\1\13\2\11\1\5\1\6\1\7\1\10"+
            "\1\4\10\11\1\1\50\11",
            "",
            "",
            "\101\16",
            "",
            "\4\16\1\34\4\16\1\35\1\37\6\16\1\41\1\16\1\36\1\40\1\36\53"+
            "\16",
            "\12\16\1\57\66\16",
            "\12\16\1\75\66\16",
            "\22\16\1\113\4\16\1\114\51\16",
            "",
            "\101\132",
            "",
            "\4\132\1\153\4\132\1\147\1\151\6\132\1\154\1\132\1\150\1\152"+
            "\1\150\53\132",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\21\132\1\172\57\132",
            "\4\132\1\u0088\5\132\1\u008a\6\132\1\u008c\1\132\1\u0089\1"+
            "\u008b\1\u0089\53\132",
            "\12\132\1\u009a\11\132\1\u009b\54\132",
            "\1\u00a9\21\u00ad\1\u00aa\1\u00a9\1\u00ad\1\u00a9\4\u00ad\1"+
            "\u00aa\35\u00ad\1\u00ab\3\u00ad\3\u00aa\1\u00ad\1\u00ac",
            "\1\u00ba\21\u00be\1\u00bb\1\u00ba\1\u00be\1\u00ba\4\u00be\1"+
            "\u00bb\35\u00be\1\u00bc\3\u00be\3\u00bb\1\u00be\1\u00bd",
            "\4\u00be\1\u00cf\4\u00be\1\u00cb\1\u00cd\6\u00be\1\u00d0\1"+
            "\u00be\1\u00cc\1\u00ce\1\u00cc\53\u00be",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\101\u00de",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\101\u00eb",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u00eb\1\u00f8\66\u00eb",
            "\12\u00eb\1\u0106\66\u00eb",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u00eb\1\u0117\5\u00eb\1\u0115\6\u00eb\1\u0118\1\u00eb\1"+
            "\u0114\1\u0116\1\u0114\53\u00eb",
            "\12\u00eb\1\u0126\11\u00eb\1\u0127\54\u00eb",
            "\1\u0135\21\u0139\1\u0136\1\u0135\1\u0139\1\u0135\4\u0139\1"+
            "\u0136\35\u0139\1\u0137\3\u0139\3\u0136\1\u0139\1\u0138",
            "\1\u0146\21\u014a\1\u0147\1\u0146\1\u014a\1\u0146\4\u014a\1"+
            "\u0147\35\u014a\1\u0148\3\u014a\3\u0147\1\u014a\1\u0149",
            "\21\u014a\1\u0157\57\u014a",
            "\4\u014a\1\u0169\4\u014a\1\u0165\1\u0167\6\u014a\1\u016a\1"+
            "\u014a\1\u0166\1\u0168\1\u0166\53\u014a",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u014a\1\u017c\4\u014a\1\u0179\1\u0178\6\u014a\1\u017d\1"+
            "\u014a\1\u017a\1\u017b\1\u017a\53\u014a",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\21\u014a\1\u018b\57\u014a",
            "\12\u014a\1\u0199\11\u014a\1\u019a\54\u014a",
            "\1\u01a8\21\u01ac\1\u01a9\1\u01a8\1\u01ac\1\u01a8\4\u01ac\1"+
            "\u01a9\35\u01ac\1\u01aa\3\u01ac\3\u01a9\1\u01ac\1\u01ab",
            "\1\u01b9\21\u01bd\1\u01ba\1\u01b9\1\u01bd\1\u01b9\4\u01bd\1"+
            "\u01ba\35\u01bd\1\u01bb\3\u01bd\3\u01ba\1\u01bd\1\u01bc",
            "\4\u01bd\1\u01ce\4\u01bd\1\u01ca\1\u01cc\6\u01bd\1\u01cf\1"+
            "\u01bd\1\u01cb\1\u01cd\1\u01cb\53\u01bd",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u01dd\21\u01e1\1\u01de\1\u01dd\1\u01e1\1\u01dd\4\u01e1\1"+
            "\u01de\35\u01e1\1\u01df\3\u01e1\3\u01de\1\u01e1\1\u01e0",
            "\1\u01ee\21\u01f2\1\u01ef\1\u01ee\1\u01f2\1\u01ee\4\u01f2\1"+
            "\u01ef\35\u01f2\1\u01f0\3\u01f2\3\u01ef\1\u01f2\1\u01f1",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u01f2\1\u0200\10\u01f2\1\u01ff\1\u0201\1\u01ff\53\u01f2",
            "\4\u01f2\1\u0212\5\u01f2\1\u0210\6\u01f2\1\u0213\1\u01f2\1"+
            "\u020f\1\u0211\1\u020f\53\u01f2",
            "\4\u01f2\1\u0225\4\u01f2\1\u0221\1\u0223\6\u01f2\1\u0226\1"+
            "\u01f2\1\u0222\1\u0224\1\u0222\53\u01f2",
            "\4\u01f2\1\u0238\4\u01f2\1\u0234\1\u0236\6\u01f2\1\u0239\1"+
            "\u01f2\1\u0235\1\u0237\1\u0235\53\u01f2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u01f2\1\u0248\10\u01f2\1\u0247\1\u0249\1\u0247\53\u01f2",
            "\4\u01f2\1\u025a\5\u01f2\1\u0258\6\u01f2\1\u025b\1\u01f2\1"+
            "\u0257\1\u0259\1\u0257\53\u01f2",
            "\4\u01f2\1\u026d\4\u01f2\1\u0269\1\u026b\6\u01f2\1\u026e\1"+
            "\u01f2\1\u026a\1\u026c\1\u026a\53\u01f2",
            "\4\u01f2\1\u0280\4\u01f2\1\u027c\1\u027e\6\u01f2\1\u0281\1"+
            "\u01f2\1\u027d\1\u027f\1\u027d\53\u01f2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u01f2\1\u0292\5\u01f2\1\u0290\6\u01f2\1\u0293\1\u01f2\1"+
            "\u028f\1\u0291\1\u028f\53\u01f2",
            "\12\u01f2\1\u02a1\11\u01f2\1\u02a2\54\u01f2",
            "\1\u02b0\1\u02b4\20\u01f2\1\u02b1\1\u02b0\1\u01f2\1\u02b0\3"+
            "\u01f2\1\u02b5\1\u02b1\35\u01f2\1\u02b2\3\u01f2\3\u02b1\1\u01f2"+
            "\1\u02b3",
            "\1\u02c2\1\u02c6\20\u01f2\1\u02c3\1\u02c2\1\u01f2\1\u02c2\3"+
            "\u01f2\1\u02c7\1\u02c3\35\u01f2\1\u02c4\3\u01f2\3\u02c3\1\u01f2"+
            "\1\u02c5",
            "\21\u01f2\1\u02d4\57\u01f2",
            "\4\u01f2\1\u02e6\4\u01f2\1\u02e2\1\u02e4\6\u01f2\1\u02e7\1"+
            "\u01f2\1\u02e3\1\u02e5\1\u02e3\53\u01f2",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\23\u02f8\1\u02f5\46\u02f8\1\u02f6\1\u02f7\5\u02f8",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\23\u0308\1\u0306\1\u0308\1\u0306\44\u0308\1\u0307\6\u0308",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u0308\1\u0316\11\u0308\1\u0317\54\u0308",
            "\1\u0325\21\u0329\1\u0326\1\u0325\1\u0329\1\u0325\4\u0329\1"+
            "\u0326\35\u0329\1\u0327\3\u0329\3\u0326\1\u0329\1\u0328",
            "\1\u0336\21\u033a\1\u0337\1\u0336\1\u033a\1\u0336\4\u033a\1"+
            "\u0337\35\u033a\1\u0338\3\u033a\3\u0337\1\u033a\1\u0339",
            "\21\u033a\1\u0347\57\u033a",
            "\4\u033a\1\u0359\4\u033a\1\u0355\1\u0357\6\u033a\1\u035a\1"+
            "\u033a\1\u0356\1\u0358\1\u0356\53\u033a",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\1\u0368\21\u036c\1\u0369\1\u0368\1\u036c\1\u0368\4\u036c\1"+
            "\u0369\35\u036c\1\u036a\3\u036c\3\u0369\1\u036c\1\u036b",
            "\1\u0379\21\u037d\1\u037a\1\u0379\1\u037d\1\u0379\4\u037d\1"+
            "\u037a\35\u037d\1\u037b\3\u037d\3\u037a\1\u037d\1\u037c",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u037d\1\u038b\10\u037d\1\u038a\1\u038c\1\u038a\53\u037d",
            "\4\u037d\1\u039d\5\u037d\1\u039b\6\u037d\1\u039e\1\u037d\1"+
            "\u039a\1\u039c\1\u039a\53\u037d",
            "\4\u037d\1\u03b0\4\u037d\1\u03ac\1\u03ae\6\u037d\1\u03b1\1"+
            "\u037d\1\u03ad\1\u03af\1\u03ad\53\u037d",
            "\4\u037d\1\u03c3\4\u037d\1\u03bf\1\u03c1\6\u037d\1\u03c4\1"+
            "\u037d\1\u03c0\1\u03c2\1\u03c0\53\u037d",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\12\u037d\1\u03d3\10\u037d\1\u03d2\1\u03d4\1\u03d2\53\u037d",
            "\4\u037d\1\u03e5\5\u037d\1\u03e3\6\u037d\1\u03e6\1\u037d\1"+
            "\u03e2\1\u03e4\1\u03e2\53\u037d",
            "\4\u037d\1\u03f8\4\u037d\1\u03f4\1\u03f6\6\u037d\1\u03f9\1"+
            "\u037d\1\u03f5\1\u03f7\1\u03f5\53\u037d",
            "\4\u037d\1\u040b\4\u037d\1\u0407\1\u0409\6\u037d\1\u040c\1"+
            "\u037d\1\u0408\1\u040a\1\u0408\53\u037d",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u037d\1\u041e\4\u037d\1\u041a\1\u041c\6\u037d\1\u041f\1"+
            "\u037d\1\u041b\1\u041d\1\u041b\53\u037d",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "\4\u037d\1\u0430\5\u037d\1\u042e\6\u037d\1\u0431\1\u037d\1"+
            "\u042d\1\u042f\1\u042d\53\u037d",
            "\12\u037d\1\u043f\11\u037d\1\u0440\54\u037d",
            "\1\u044e\1\u0452\20\u037d\1\u044f\1\u044e\1\u037d\1\u044e\3"+
            "\u037d\1\u0453\1\u044f\35\u037d\1\u0450\3\u037d\3\u044f\1\u037d"+
            "\1\u0451",
            "\1\u0460\1\u0464\20\u037d\1\u0461\1\u0460\1\u037d\1\u0460\3"+
            "\u037d\1\u0465\1\u0461\35\u037d\1\u0462\3\u037d\3\u0461\1\u037d"+
            "\1\u0463",
            "\21\u037d\1\u0472\57\u037d",
            "\4\u037d\1\u0484\4\u037d\1\u0480\1\u0482\6\u037d\1\u0485\1"+
            "\u037d\1\u0481\1\u0483\1\u0481\53\u037d",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            ""
    };

    static final short[] DFA27_eot = DFA.unpackEncodedString(DFA27_eotS);
    static final short[] DFA27_eof = DFA.unpackEncodedString(DFA27_eofS);
    static final char[] DFA27_min = DFA.unpackEncodedStringToUnsignedChars(DFA27_minS);
    static final char[] DFA27_max = DFA.unpackEncodedStringToUnsignedChars(DFA27_maxS);
    static final short[] DFA27_accept = DFA.unpackEncodedString(DFA27_acceptS);
    static final short[] DFA27_special = DFA.unpackEncodedString(DFA27_specialS);
    static final short[][] DFA27_transition;

    static {
        int numStates = DFA27_transitionS.length;
        DFA27_transition = new short[numStates][];
        for (int i=0; i<numStates; i++) {
            DFA27_transition[i] = DFA.unpackEncodedString(DFA27_transitionS[i]);
        }
    }

    class DFA27 extends DFA {

        public DFA27(BaseRecognizer recognizer) {
            this.recognizer = recognizer;
            this.decisionNumber = 27;
            this.eot = DFA27_eot;
            this.eof = DFA27_eof;
            this.min = DFA27_min;
            this.max = DFA27_max;
            this.accept = DFA27_accept;
            this.special = DFA27_special;
            this.transition = DFA27_transition;
        }
        public String getDescription() {
            return "()+ loopback of 181:15: (~ ( BACKSLASH | RET ) )+";
        }
    }
 

    public static final BitSet FOLLOW_code_fragment_in_code156 = new BitSet(new long[]{0xFFFFFFFFFFFFE3F0L,0x000000000000001FL});
    public static final BitSet FOLLOW_EOF_in_code161 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_define_directive_in_code_fragment212 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_import_declaration_in_code_fragment217 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_if_simple_check_wrapper_in_code_fragment222 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_if_define_block_wrapper_in_code_fragment227 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_if_not_define_block_wrapper_in_code_fragment232 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_smth_else_in_code_fragment237 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_RET_in_ret248 = new BitSet(new long[]{0x0000000010000002L});
    public static final BitSet FOLLOW_if_not_define_block_in_if_not_define_block_wrapper258 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ifndef1_in_if_not_define_block277 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ifndef2_in_if_not_define_block281 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_if_not_define_block284 = new BitSet(new long[]{0xFFFFFFFFFFFFE3F0L,0x000000000000001FL});
    public static final BitSet FOLLOW_main_code_block_in_if_not_define_block288 = new BitSet(new long[]{0x0000000000001800L});
    public static final BitSet FOLLOW_ELSE_in_if_not_define_block293 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_if_not_define_block295 = new BitSet(new long[]{0xFFFFFFFFFFFFE3F0L,0x000000000000001FL});
    public static final BitSet FOLLOW_else_code_block_in_if_not_define_block300 = new BitSet(new long[]{0x0000000000001000L});
    public static final BitSet FOLLOW_ENDIF_in_if_not_define_block307 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IFNDEF_in_ifndef1317 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_name_in_ifndef1319 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IF_in_ifndef2327 = new BitSet(new long[]{0x0000000000002000L});
    public static final BitSet FOLLOW_EXC_in_ifndef2330 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_DEFINED_in_ifndef2333 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_L_BR_in_ifndef2335 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_name_in_ifndef2338 = new BitSet(new long[]{0x0000000020000000L});
    public static final BitSet FOLLOW_R_BR_in_ifndef2340 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_if_simple_check_in_if_simple_check_wrapper349 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IF_in_if_simple_check367 = new BitSet(new long[]{0x0000000003A06100L});
    public static final BitSet FOLLOW_expression_wrapper_in_if_simple_check369 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_if_simple_check371 = new BitSet(new long[]{0xFFFFFFFFFFFFFFF0L,0x000000000000001FL});
    public static final BitSet FOLLOW_main_code_block_in_if_simple_check375 = new BitSet(new long[]{0x0000000000001C00L});
    public static final BitSet FOLLOW_ELIF_in_if_simple_check381 = new BitSet(new long[]{0x0000000003A06100L});
    public static final BitSet FOLLOW_expression_wrapper_in_if_simple_check384 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_if_simple_check386 = new BitSet(new long[]{0xFFFFFFFFFFFFE3F0L,0x000000000000001FL});
    public static final BitSet FOLLOW_else_code_block_in_if_simple_check390 = new BitSet(new long[]{0x0000000000001C00L});
    public static final BitSet FOLLOW_ELSE_in_if_simple_check397 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_if_simple_check399 = new BitSet(new long[]{0xFFFFFFFFFFFFE3F0L,0x000000000000001FL});
    public static final BitSet FOLLOW_else_code_block_in_if_simple_check404 = new BitSet(new long[]{0x0000000000001000L});
    public static final BitSet FOLLOW_ENDIF_in_if_simple_check411 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_expression_in_expression_wrapper423 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_if_define_block_in_if_define_block_wrapper442 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ifdef1_in_if_define_block462 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ifdef2_in_if_define_block466 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_if_define_block469 = new BitSet(new long[]{0xFFFFFFFFFFFFFBF0L,0x000000000000001FL});
    public static final BitSet FOLLOW_main_code_block_in_if_define_block473 = new BitSet(new long[]{0x0000000000001800L});
    public static final BitSet FOLLOW_ELSE_in_if_define_block479 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_if_define_block481 = new BitSet(new long[]{0xFFFFFFFFFFFFE3F0L,0x000000000000001FL});
    public static final BitSet FOLLOW_else_code_block_in_if_define_block486 = new BitSet(new long[]{0x0000000000001000L});
    public static final BitSet FOLLOW_ENDIF_in_if_define_block493 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IF_in_ifdef1503 = new BitSet(new long[]{0x0000000000000100L});
    public static final BitSet FOLLOW_DEFINED_in_ifdef1505 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_L_BR_in_ifdef1507 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_name_in_ifdef1509 = new BitSet(new long[]{0x0000000020000000L});
    public static final BitSet FOLLOW_R_BR_in_ifdef1511 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_IFDEF_in_ifdef2519 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_name_in_ifdef2522 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_code_block_in_main_code_block531 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_code_block_in_else_code_block548 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_code_fragment_in_code_block565 = new BitSet(new long[]{0xFFFFFFFFFFFFE3F2L,0x000000000000001FL});
    public static final BitSet FOLLOW_ID_in_name605 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_set_in_import_declaration622 = new BitSet(new long[]{0x0000000008400000L});
    public static final BitSet FOLLOW_import_end_in_import_declaration630 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_import_internal_in_import_end640 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_import_external_in_import_end647 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_import_internal_string_in_import_internal657 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_L_UBR_in_import_internal_string675 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_filename_in_import_internal_string678 = new BitSet(new long[]{0x8000000040000000L});
    public static final BitSet FOLLOW_63_in_import_internal_string681 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_filename_in_import_internal_string683 = new BitSet(new long[]{0x8000000040000000L});
    public static final BitSet FOLLOW_R_UBR_in_import_internal_string688 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_filename696 = new BitSet(new long[]{0x4000000000800002L});
    public static final BitSet FOLLOW_MINUS_in_filename699 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_ID_in_filename701 = new BitSet(new long[]{0x4000000000800002L});
    public static final BitSet FOLLOW_62_in_filename706 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_ID_in_filename708 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_filename2719 = new BitSet(new long[]{0x4000000002800002L});
    public static final BitSet FOLLOW_set_in_filename2722 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_ID_in_filename2730 = new BitSet(new long[]{0x4000000002800002L});
    public static final BitSet FOLLOW_62_in_filename2735 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_ID_in_filename2737 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_import_external2_in_import_external748 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_QUOTE_in_import_external2766 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_filename2_in_import_external2768 = new BitSet(new long[]{0x0000000008000000L});
    public static final BitSet FOLLOW_QUOTE_in_import_external2770 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DEFINE_in_define_directive781 = new BitSet(new long[]{0x0000000000004000L});
    public static final BitSet FOLLOW_id_wrapper_in_define_directive784 = new BitSet(new long[]{0xFFFFFFFFEFFFFFF2L,0x000000000000001FL});
    public static final BitSet FOLLOW_mln_end_in_define_directive787 = new BitSet(new long[]{0xFFFFFFFFEFFFFFD2L,0x000000000000001FL});
    public static final BitSet FOLLOW_replace_wrapper_in_define_directive790 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_BACKSLASH_in_mln_end816 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_mln_end818 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_wrapper_in_id_wrapper828 = new BitSet(new long[]{0x0000000000200002L});
    public static final BitSet FOLLOW_in_brackets_wrapper_in_id_wrapper830 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_name_wrapper853 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_in_brackets_in_in_brackets_wrapper872 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_L_BR_in_in_brackets891 = new BitSet(new long[]{0x0000000004004000L});
    public static final BitSet FOLLOW_param_wrapper_in_in_brackets893 = new BitSet(new long[]{0x2000000020000000L});
    public static final BitSet FOLLOW_61_in_in_brackets896 = new BitSet(new long[]{0x0000000004004000L});
    public static final BitSet FOLLOW_param_wrapper_in_in_brackets899 = new BitSet(new long[]{0x2000000020000000L});
    public static final BitSet FOLLOW_R_BR_in_in_brackets903 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_param_common_in_param_wrapper915 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_POINTS3_in_param_wrapper920 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ID_in_param_common932 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_replace_in_replace_wrapper951 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_replace_internal_in_replace969 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_backslash_in_replace972 = new BitSet(new long[]{0x0000000010000000L});
    public static final BitSet FOLLOW_ret_in_replace974 = new BitSet(new long[]{0xFFFFFFFFEFFFFFD0L,0x000000000000001FL});
    public static final BitSet FOLLOW_replace_internal_in_replace976 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_BACKSLASH_in_backslash988 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_67_in_replace_internal1007 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ex_or_in_expression1029 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ex_and_wrp_in_ex_or1045 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000010L});
    public static final BitSet FOLLOW_68_in_ex_or1048 = new BitSet(new long[]{0x0000000003A06100L});
    public static final BitSet FOLLOW_expression_wrapper_in_ex_or1050 = new BitSet(new long[]{0x0000000000000002L,0x0000000000000010L});
    public static final BitSet FOLLOW_ex_and_in_ex_and_wrp1062 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ex_not_wrp_in_ex_and1080 = new BitSet(new long[]{0x1000000000000002L});
    public static final BitSet FOLLOW_60_in_ex_and1083 = new BitSet(new long[]{0x0000000003A06100L});
    public static final BitSet FOLLOW_expression_wrapper_in_ex_and1085 = new BitSet(new long[]{0x1000000000000002L});
    public static final BitSet FOLLOW_ex_not_in_ex_not_wrp1096 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_EXC_in_ex_not1112 = new BitSet(new long[]{0x0000000003A04100L});
    public static final BitSet FOLLOW_ex_cond_in_ex_not1115 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ex_cond_in_ex_cond_wrp1125 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ex_sngl_wrp_in_ex_cond1143 = new BitSet(new long[]{0x0000000040400002L,0x0000000000000007L});
    public static final BitSet FOLLOW_cond_op_wrp_in_ex_cond1146 = new BitSet(new long[]{0x0000000003A04100L});
    public static final BitSet FOLLOW_ex_sngl_wrp_in_ex_cond1148 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_cond_op_in_cond_op_wrp1160 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_ex_sngl_in_ex_sngl_wrp1203 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_math_expr_wrp_in_ex_sngl1220 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_defined_wrp_in_ex_sngl1226 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_defined_expr_in_defined_wrp1236 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_DEFINED_in_defined_expr1255 = new BitSet(new long[]{0x0000000000200000L});
    public static final BitSet FOLLOW_L_BR_in_defined_expr1258 = new BitSet(new long[]{0x0000000003A06100L});
    public static final BitSet FOLLOW_expression_wrapper_in_defined_expr1260 = new BitSet(new long[]{0x0000000020000020L});
    public static final BitSet FOLLOW_mln_end_in_defined_expr1263 = new BitSet(new long[]{0x0000000020000000L});
    public static final BitSet FOLLOW_R_BR_in_defined_expr1266 = new BitSet(new long[]{0x0000000000000022L});
    public static final BitSet FOLLOW_mln_end_in_defined_expr1269 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_math_expr_in_math_expr_wrp1281 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_single_expr_in_math_expr1301 = new BitSet(new long[]{0x0000000002800012L});
    public static final BitSet FOLLOW_op_wrp_in_math_expr1304 = new BitSet(new long[]{0x0000000003804000L});
    public static final BitSet FOLLOW_single_expr_in_math_expr1306 = new BitSet(new long[]{0x0000000002800012L});
    public static final BitSet FOLLOW_op_in_op_wrp1317 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_single_expr2_in_single_expr1361 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_name_in_single_expr21371 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_const_expr_in_single_expr21376 = new BitSet(new long[]{0x0000000000000002L});
    public static final BitSet FOLLOW_NUMBER_in_const_expr1387 = new BitSet(new long[]{0x0000000000000002L});

}