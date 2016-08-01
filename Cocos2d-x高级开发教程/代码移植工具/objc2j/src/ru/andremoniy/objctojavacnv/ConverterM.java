package ru.andremoniy.objctojavacnv;

import org.antlr.runtime.ANTLRStringStream;
import org.antlr.runtime.CharStream;
import org.antlr.runtime.CommonTokenStream;
import org.antlr.runtime.RecognitionException;
import org.antlr.runtime.tree.CommonTree;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ru.andremoniy.objctojavacnv.antlr.Preprocessor;
import ru.andremoniy.objctojavacnv.antlr.output.ObjcmLexer;
import ru.andremoniy.objctojavacnv.antlr.output.ObjcmParser;
import ru.andremoniy.objctojavacnv.context.*;

import java.io.*;
import java.nio.charset.Charset;
import java.util.*;

import static ru.andremoniy.objctojavacnv.Utils.transformObject;
import static ru.andremoniy.objctojavacnv.Utils.transformType;

/**
 * User: Andremoniy
 * Date: 12.06.12
 * Time: 1:14
 */

// package ru.andremoniy.objctojavacnv.antlr.output;

public class ConverterM {

    public static final Logger log = LoggerFactory.getLogger(ConverterM.class);

    //    private static final List<String> RESTRICTED_METHODS = Arrays.asList("release", "retain", /*"autorelease", */"dealloc"/*, "init"*/);
    private static final List<String> RESTRICTED_METHODS = Collections.EMPTY_LIST;
    private static final Map<Integer, String> OPS_NUMBER = new HashMap<Integer, String>() {{
        put(ObjcmLexer.L_MINUS, "minus");
        put(ObjcmLexer.L_PLUS, "plus");
        put(ObjcmLexer.L_DIV, "divide");
        put(ObjcmLexer.ASTERISK, "multiple");
        put(ObjcmLexer.L_PERC, "percent");
        put(ObjcmLexer.L_MORE, "more");
        put(ObjcmLexer.L_MORE_EQ, "moreEquals");
        put(ObjcmLexer.L_LESS, "less");
        put(ObjcmLexer.L_LESS_EQ, "lessEquals");
        put(ObjcmLexer.L_EQ_EQ, "equals");
        put(ObjcmLexer.L_NEQ, "notEquals");
        put(ObjcmLexer.L_EQ, "set");
        put(ObjcmLexer.L_PLUS_EQ, "setPlus");
        put(ObjcmLexer.L_MINUS_EQ, "setMinus");
        put(ObjcmLexer.L_DIV_EQ, "setDivide");
        put(ObjcmLexer.L_MULT_EQ, "setMultiple");
        put(ObjcmLexer.L_AND, "and");
        put(ObjcmLexer.L_AND_AND, "andAnd");
        put(ObjcmLexer.L_OR, "or");
        put(ObjcmLexer.L_OR_OR, "orOr");
        put(ObjcmLexer.L_AND_EQ, "setAnd");
        put(ObjcmLexer.L_OR_EQ, "setOr");
        put(ObjcmLexer.L_XOR, "xor");
        put(ObjcmLexer.L_XOR_EQ, "setXor");
    }};

    private static final List<List<Integer>> OPERATIONS_ORDER = new ArrayList<List<Integer>>() {{
        add(new ArrayList<Integer>());
        add(Arrays.asList(ObjcmLexer.L_OR_OR));
        add(Arrays.asList(ObjcmLexer.L_AND_AND));
        add(Arrays.asList(ObjcmLexer.L_OR));
        add(Arrays.asList(ObjcmLexer.L_XOR));
        add(Arrays.asList(ObjcmLexer.L_AND));
        add(Arrays.asList(ObjcmLexer.L_EQ_EQ, ObjcmLexer.L_NEQ));
        add(Arrays.asList(ObjcmLexer.L_LESS, ObjcmLexer.L_MORE, ObjcmLexer.L_LESS_EQ, ObjcmLexer.L_MORE_EQ));
        add(Arrays.asList(ObjcmLexer.L_LEFT, ObjcmLexer.L_RIGHT));
        add(Arrays.asList(ObjcmLexer.L_PLUS, ObjcmLexer.L_MINUS));
        add(Arrays.asList(ObjcmLexer.ASTERISK, ObjcmLexer.L_DIV, ObjcmLexer.L_PERC));
    }};
    private static final List<Integer> EXPR_ORDER = Arrays.asList(
            ObjcmLexer.EXPR_OR_OR, ObjcmLexer.EXPR_AND_AND, ObjcmLexer.EXPR_OR, ObjcmLexer.EXPR_XOR, ObjcmLexer.EXPR_AND, ObjcmLexer.EXPR_EQ,
            ObjcmLexer.EXPR_COND, ObjcmLexer.EXPR_MOV, ObjcmLexer.EXPR_ADD, ObjcmLexer.EXPR_MULT, ObjcmLexer.EXPR_TYPE);

    public static StringBuilder convert_m(String fileName, ProjectContext projectCtx, StringBuilder addSb) throws IOException, RecognitionException {
        projectCtx.m_counter++;
        log.info(projectCtx.m_counter + " m files processed");

        File pmfile = new File(fileName + "p");
        File mfile = pmfile.exists() ? pmfile : new File(fileName);

        final boolean categoryClass = mfile.getName().contains("+");
        final String categoryName = categoryClass ? mfile.getName().substring(mfile.getName().indexOf("+") + 1, mfile.getName().lastIndexOf(".")) : null;

        // new file with java code
        String className1 = mfile.getName().substring(0, mfile.getName().lastIndexOf("."));
        File mjfile = new File(mfile.getParent() + File.separator + className1 + ".java");
        mjfile.createNewFile();

        projectCtx.newClass(className1, categoryName);

        String packageName = mfile.getParent().substring(mfile.getParent().lastIndexOf("src") + 4).replace(File.separator, ".");
        projectCtx.classCtx.packageName = packageName;

        StringBuilder objCcode = new StringBuilder();
        try (FileInputStream fis = new FileInputStream(mfile);
             InputStreamReader isr = new InputStreamReader(fis, Charset.forName("utf-8"));
             BufferedReader br = new BufferedReader(isr)) {
            String line;
            while ((line = br.readLine()) != null) {
                objCcode.append(line).append("\n");
            }
        }

        String input = objCcode.toString();
        input = input.replace("///", "//");

        log.info("Preprocessing... ");
        input = Preprocessor.replace(input, projectCtx, fileName);
        log.info("ok");

        CharStream cs = new ANTLRStringStream(input);
        ObjcmLexer lexer = new ObjcmLexer(cs);
        CommonTokenStream tokens = new CommonTokenStream();
        tokens.setTokenSource(lexer);
        ObjcmParser parser = new ObjcmParser(tokens);

        ObjcmParser.code_return result = parser.code();

        StringBuilder sb = new StringBuilder();

        if (!categoryClass) {
            sb.append("package ").append(packageName).append(";\n\n");

            // process imports sections
            sb.append("import ru.andremoniy.jcocoa.*;\n");
            sb.append("import java.util.List;\n");
            sb.append("import java.util.Arrays;\n");
            sb.append("import java.util.ArrayList;\n");
            sb.append("import static ru.andremoniy.jcocoa.MathEx.*;\n");
            sb.append("import static ru.andremoniy.jcocoa.Utils.*;\n");
            sb.append("import static ru.andremoniy.jcocoa.NSException.*;\n");
            sb.append("import static ru.andremoniy.jcocoa.Constants.*;\n");
        }

        if (addSb == null) {
            addSb = new StringBuilder();
        }

        Utils.addOriginalImports(input, projectCtx, addSb);
        Utils.addStaticFromHeaderImports(projectCtx, sb, className1);

        StringBuilder catSb = new StringBuilder();

        if (!categoryClass) {
            sb.append(addSb);
            // идем по всем категориям в поиска import-ов

            Set<String> categoriesList = projectCtx.categories.get(mfile.getName().substring(0, mfile.getName().indexOf(".")));
            if (categoriesList != null) {
                for (String category : categoriesList) {
                    catSb.append(convert_m(category, projectCtx, addSb));
                }
            }
        }

        if (!categoryClass) {
            if (projectCtx.classCtx.localCategories.isEmpty()) {
                projectCtx.classCtx.categoryList = "null";
            } else {
                projectCtx.classCtx.categoryList = "";
                for (String category : projectCtx.classCtx.localCategories) {
                    if (!projectCtx.classCtx.categoryList.isEmpty()) projectCtx.classCtx.categoryList += ",";
                    projectCtx.classCtx.categoryList += "\"" + category + "\"";
                }
                projectCtx.classCtx.categoryList = "Arrays.asList(" + projectCtx.classCtx.categoryList + ")";
            }

            sb.append("\n");
        }
        // sb - это будет основной билдер класса. Туда будем по необходимости дописывать import-ы

        CommonTree tree = (CommonTree) result.getTree();
        CommonTree implementationTree = tree.getType() == ObjcmLexer.IMPLEMENTATION ? tree : (CommonTree) ((CommonTree) result.getTree()).getFirstChildWithType(ObjcmLexer.IMPLEMENTATION);
        if (implementationTree == null) return sb;

        StringBuilder sb2 = new StringBuilder(); // билдер класса
        if (!categoryClass) {
            m_process_implementation1(sb2, implementationTree, className1);
        }

        if (tree == implementationTree) {
            m_process_implementation2(sb2, tree, projectCtx);
        } else {
            for (Object child : tree.getChildren()) {
                CommonTree childTree = (CommonTree) child;
                switch (childTree.token.getType()) {
                    case ObjcmLexer.OPERATOR:
                        m_process_operator(sb2, childTree, projectCtx.classCtx);
                        break;
                    case ObjcmLexer.FIELD:
                        m_process_field(sb2, childTree, projectCtx.classCtx);
                        break;
                    case ObjcmLexer.INTERFACE:
                        m_process_interface(sb2, childTree, projectCtx);
                        break;
                    case ObjcmLexer.IMPLEMENTATION:
                        m_process_implementation2(sb2, childTree, projectCtx);
                        break;
                }
            }
        }

        if (categoryClass) return sb2;

        Utils.addAdditionalImports(sb, projectCtx);

        if (!projectCtx.classCtx.containsInit) {
            addInitMethod(sb2, className1);
        }
        if (!projectCtx.classCtx.containsAutoRelease) {
            addAutoReleaseMethod(sb2, className1);
        }

        sb2.append(catSb);

        sb2.append("}\n"); // end of class

        sb.append(sb2); // соединяем шапку с классом

        String javaCode = sb.toString();
        javaCode = javaCode.replaceAll(";(\\s|\n|\r)*;+", ";");

        // избавляемся от конструкций вида "this = null"
        javaCode = javaCode.replaceAll("this\\s*=\\s*null", "");
        // избавляемся от конструкций вида "this = ..."
        javaCode = javaCode.replaceAll("this\\s*=\\s*", "");

        try (FileOutputStream fos = new FileOutputStream(mjfile);
             BufferedOutputStream bos = new BufferedOutputStream(fos)) {
            bos.write(javaCode.getBytes("utf-8"));
        }

        return sb;
    }

    private static void m_process_interface(StringBuilder sb, CommonTree tree, ProjectContext projectCtx) {
        CommonTree nameTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.NAME);
        String interfaceName = Utils.getText(nameTree).trim();
        boolean sameInterface = interfaceName.equals(projectCtx.classCtx.className);
        if (!sameInterface) {
            sb.append("public static interface ").append(interfaceName).append(" {\n");
        }
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                // Это статическое поле или метод
                case ObjcmLexer.METHOD:
                    if (sameInterface) break;
                    m_process_method(sb, childTree, projectCtx.newClass(interfaceName, null), false);
                    break;
            }
        }
        if (!sameInterface) {
            sb.append("}\n");
        }
    }

    private static void m_process_operator(StringBuilder sb, CommonTree tree, ClassContext classCtx) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                // Это статическое поле или метод
                case ObjcmLexer.M_TYPE_START:
                    m_process_type_start(sb, childTree, classCtx);
                    break;
                case ObjcmLexer.INTERFACE:
                    m_process_interface(sb, childTree, classCtx.projectCtx);
                    break;
                case ObjcmLexer.STATIC:
                    m_process_static(sb, childTree, classCtx);
                    break;
                default:
                    if (childTree.getChildCount() == 0) {
                        sb.append(childTree.getText());
                    }
                    break;
            }
        }
    }

    private static void m_process_type_start(StringBuilder sb, CommonTree tree, ClassContext classCtx) {
        StringBuilder lsb = new StringBuilder();
        boolean isStatic = false;
        String name = "";
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                // Это статическое поле или метод
                case ObjcmLexer.TYPE:
                    readChildren(lsb, childTree, null, null); // there is no method, then null
                    break;
                case ObjcmLexer.NAME:
                    StringBuilder namesb = new StringBuilder();
                    readChildren(namesb, childTree, null, null); // there is no method, then null
                    lsb.append(namesb);
                    name = namesb.toString().trim();
                    break;
                case ObjcmLexer.FIELD:

                    classCtx.projectCtx.staticFields.put(name, classCtx.className);

                    CommonTree valueTree = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.VALUE);
                    if (valueTree != null) {
                        lsb.append("=");
                        lsb.append(m_process_field_value(valueTree, classCtx));
                    }
                    break;
                default:
                    if (child.toString().equals("static")) isStatic = true;
                    lsb.append(Utils.getText(childTree));
                    lsb.append(" ");
                    break;
            }
        }

        if (!isStatic && !lsb.toString().trim().isEmpty()) {
            sb.append("public static ");
        }

        sb.append(lsb);
        sb.append(";");
    }

    private static void m_process_static(StringBuilder sb, CommonTree tree, ClassContext classCtx) {
        boolean isField = false;
        String methodName = "";
        String fieldName = "";
        String methodType = "";
        boolean isStatic = false;
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.TYPE:
                    sb.append(" public ");
                    if (isStatic) sb.append("static ");
                    StringBuilder typeSb = new StringBuilder();
                    readChildren(typeSb, childTree, classCtx, null);
                    sb.append(typeSb);
                    methodType = typeSb.toString().trim();
                    break;
                case ObjcmLexer.NAME:
                    StringBuilder nameSb = new StringBuilder();
                    readChildren(nameSb, childTree, classCtx, null);
                    sb.append(nameSb);
                    if (tree.getFirstChildWithType(ObjcmLexer.METHOD) != null) {
                        methodName = nameSb.toString().trim();
                    } else {
                        fieldName = nameSb.toString().trim();
                    }
                    break;
                case ObjcmLexer.FIELD:
                    isField = true;
                    CommonTree value = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.VALUE);
                    if (value != null) {
                        sb.append("=");
                        sb.append(m_process_field_value(value, classCtx));
                    }

                    classCtx.projectCtx.staticFields.put(fieldName, classCtx.className);

                    break;
                // Это статическое поле или метод
                case ObjcmLexer.METHOD:
                    m_process_params(sb, childTree, classCtx);
                    CommonTree blockTree = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.BLOCK);
                    if (blockTree != null) {
                        m_process_block(sb, blockTree, classCtx.newMethod(methodName, methodType, isStatic, null).newBlock());
                    } else {
                        m_process_params(sb, childTree, classCtx);
                    }
                    break;
                default:
                    String text = Utils.getText(childTree).trim();
                    if (text.equals("inline")) text = "native"; // TODO ?!
                    if (text.equals("static")) {
                        isStatic = true;
                        break;
                    }
                    sb.append(text);
                    sb.append(" ");
                    break;
            }
        }
        sb.append("\n\n");
        if (isField) {
            sb.append(";\n");
        }
    }

    private static void m_process_params(StringBuilder sb, CommonTree tree, ClassContext classCtx) {
        StringBuilder lsb = new StringBuilder();
        lsb.append("(");
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.PARAM:
                    String type = Utils.getText((CommonTree) childTree.getFirstChildWithType(ObjcmLexer.TYPE)).trim();
                    String name = Utils.getText((CommonTree) childTree.getFirstChildWithType(ObjcmLexer.NAME)).trim();
                    if (lsb.length() > 1) lsb.append(", ");
                    lsb.append(transformType(type, classCtx)).append(" ").append(name);
                    break;
            }
        }
        lsb.append(")");

        sb.append(lsb);
    }

    private static void readChildren(StringBuilder sb, CommonTree tree, ClassContext classCtx, ExpressionContext exprCtx) {
        if (tree.getChildren() == null) {
            String trObj = transformObject(tree.toString(), classCtx, exprCtx);
            sb.append(trObj);
            if (!trObj.equals("this")) {
                sb.append(" ");
            }
            return;
        }
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            readChildren(sb, childTree, classCtx, exprCtx);
        }
    }

    private static void m_process_implementation1(StringBuilder sb, CommonTree tree, String _className) {
        String className = tree.getFirstChildWithType(ObjcmLexer.NAME).getChild(0).toString();

        if (className.equals(_className)) {
            sb.append("public class ").append(className).append(" extends I").append(className).append(" {\n");
        }
    }

    private static void addAutoReleaseMethod(StringBuilder sb, String className) {
        sb.append("\n");
        sb.append("\tpublic ").append(className).append(" autorelease() {\n");
        sb.append("\t\treturn this; // TODO \n");
        sb.append("\t}\n\n");
    }

    private static void addInitMethod(StringBuilder sb, String className) {
        sb.append("\n");
        sb.append("\tpublic ").append(className).append(" init() {\n");
        sb.append("\t\treturn this;\n");
        sb.append("\t}\n\n");
    }

    private static void m_process_implementation2(StringBuilder sb, CommonTree tree, ProjectContext projectCtx) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.FIELD:
                    m_process_field2(sb, childTree, projectCtx.classCtx);
                    break;
                case ObjcmLexer.METHOD:
                    m_process_method(sb, childTree, projectCtx.classCtx, false);
                    break;
                case ObjcmLexer.STATIC_METHOD:
                    m_process_method(sb, childTree, projectCtx.classCtx, true);
                    break;
                case ObjcmLexer.STATIC:
                    m_process_static(sb, childTree, projectCtx.classCtx);
                    break;
                case ObjcmLexer.TYPEDEF_STRUCT:
                    m_process_typedef_struct(sb, childTree, projectCtx.classCtx);
                    break;
            }
        }
    }

    private static void m_process_typedef_struct(StringBuilder sb, CommonTree tree, ClassContext classCtx) {
        CommonTree nameTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.NAME);
        String name = nameTree.getChild(0).toString();

        sb.append("public static class ").append(name).append(" {\n");

        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;

            switch (childTree.getType()) {
                case ObjcmLexer.STRUCT_FIELD:
                    CommonTree typeTree = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.TYPE);
                    StringBuilder typeSb = new StringBuilder();
                    readChildren(typeSb, typeTree, classCtx, null);

                    CommonTree fieldNameTree = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.NAME);
                    StringBuilder fieldNameSb = new StringBuilder();
                    readChildren(fieldNameSb, fieldNameTree, classCtx, null);

                    sb.append("public ").append(transformType(typeSb.toString(), classCtx)).append(" ").append(fieldNameSb.toString()).append(";\n");
                    break;
            }
        }

        sb.append("}\n\n");
    }

    private static void m_process_method(StringBuilder sb, CommonTree tree, ClassContext classCtx, boolean staticFlag) {
        String modifier = "";
        String type = "";
        String methodName = "";
        LinkedHashMap<String, String> params = new LinkedHashMap<>();
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.MODIFIER:
                    modifier = childTree.getChild(0).toString();
                    break;
                case ObjcmLexer.TYPE:
                    type = childTree.getChild(0).toString();
                    break;
                case ObjcmLexer.NAME:
                    methodName = childTree.getChild(0).getText().trim();
                    break;
                case ObjcmLexer.PARAM:
                    m_process_param(params, childTree, classCtx);
                    break;
            }
        }

        String methodType = transformType(type, classCtx);
        classCtx.newMethod(methodName, methodType, staticFlag, params);
        for (String paramName : params.keySet()) {
            classCtx.methodCtx.variables.put(paramName, params.get(paramName));
        }

        if (methodName.equals("init") && params.isEmpty()) classCtx.containsInit = true;
        if (methodName.equals("autoRelease") && params.isEmpty()) classCtx.containsAutoRelease = true;

        String modifier_sb = modifier.length() > 0 ? (modifier.equals("-") ? "" : "static") : "";
        if (staticFlag) modifier_sb = "static";

        // защита от неправильной перегрузки метода init():
        if (methodName.equals("init") && classCtx.containsInit) {
            type = classCtx.className; // всегда имя класса в качестве типа возвращаемого значения...
        }

        sb.append("public ").append(modifier_sb).append(" ").append(methodType).append(" ").append(classCtx.categoryName != null ? "_" + classCtx.categoryName + "_" : "").append(methodName).append("(");

        // обратный переход,т.к. static мог бысть установлен не только из static_flag
        if (modifier_sb.equals("static")) {
            staticFlag = true;
            classCtx.methodCtx.staticFlag = true;
        }
        boolean f = true;
        for (String pName : params.keySet()) {
            if (!f) {
                sb.append(", ");
            } else {
                f = false;
            }
            String paramType = params.get(pName);
            if (paramType.equals("void")) paramType = "Void";
            sb.append(paramType).append(" ").append(pName);
        }
        sb.append(") ");
        CommonTree blockTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.BLOCK);
        if (blockTree != null) {
            // есть ситуации, когда метод заканчивается оператором SWITCH, в котором не прописана конструкция default
            // в этом случае необходимо это отследить и добавить в конце "return null;"
            classCtx.methodCtx.addReturnNull = !modifier_sb.equals("void");
            m_process_block(sb, blockTree, classCtx.methodCtx.newBlock());

        } else {
            sb.append("{};\n");
        }
        sb.append("\n\n");
    }

    private static void m_process_block(StringBuilder sb, CommonTree tree, BlockContext blockCtx) {
        List children = tree.getChildren();
        boolean wasReturn = false;
        boolean fieldAccess = false;

        Integer lastComplexTreeIndex = null;
        if (blockCtx.methodCtx().addReturnNull) {
            for (int i = tree.getChildCount() - 1; i >= 0; i--) {
                if (tree.getChild(i).getChildCount() > 0) {
                    lastComplexTreeIndex = i;
                    break;
                }
            }
            blockCtx.methodCtx().addReturnNull = false; // отключаем
        }

        for (int i = 0, childrenSize = children.size(); i < childrenSize; i++) {
            Object child = children.get(i);
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                /*********************************/
                /** new block with expressions **/
                /*******************************/
                case ObjcmLexer.GOTO:
                    process_goto(sb, childTree, blockCtx.newExpr());
                    break;
                case ObjcmLexer.VARIABLE_INIT:
                    process_variable_init(sb, childTree, blockCtx.newExpr().setNeedSaveVariable());
                    break;
                case ObjcmLexer.CLASSICAL_EXPR:
                    process_classical_expr(sb, childTree, blockCtx.newExpr(), false, false);
                    break;
                case ObjcmLexer.EXPR_FULL:
                    process_expr_full(sb, childTree, blockCtx.newExpr(), true);
                    break;
                case ObjcmLexer.CLASSICAL_EXPR_2:
                    process_classical_expr(sb, childTree, blockCtx.newExpr(), true, false);
                    break;
                case ObjcmLexer.FOR_STMT:
                    process_for_stmt(sb, childTree, blockCtx.newBlock());
                    break;
                case ObjcmLexer.FOR_IN_STMT:
                case ObjcmLexer.FOR_STMT_EXPR:
                    // do nothing
                    break;
                case ObjcmLexer.STATIC_START:
                    m_process_static_start(sb, childTree, blockCtx.newBlock());
                    break;
                case ObjcmLexer.WHILE_STMT:
                    m_process_while_stmt(sb, childTree, blockCtx.newBlock());
                    break;
                case ObjcmLexer.METHOD_CALL:
                    m_process_method_call(sb, childTree, blockCtx.newExpr());
                    break;
                case ObjcmLexer.BLOCK:
                    m_process_block(sb, childTree, blockCtx.newBlock());
                    wasReturn = false;
                    break;
                case ObjcmLexer.SWITCH:
                    boolean wasDefault = m_process_switch(sb, childTree, blockCtx.newBlock());
                    if (lastComplexTreeIndex != null && lastComplexTreeIndex == i && !wasDefault) {
                        sb.append("\nreturn null;\n");
                    }
                    break;
                case ObjcmLexer.IF_STMT:
                    m_process_if_stmt(sb, childTree, blockCtx.newBlock());
                    break;
                case ObjcmLexer.SELECTOR:
                    m_process_selector(sb, childTree, blockCtx.newExpr(), "selector");
                    break;
                case ObjcmLexer.PROTOCOL:
                    m_process_selector(sb, childTree, blockCtx.newExpr(), "protocol");
                    break;
                case ObjcmLexer.BREAK:
                    blockCtx.isBreak = true;
                    if (!blockCtx.skipBreak) {
                        sb.append("break;\n");
                    }
                    break;
                case ObjcmLexer.RETURN_STMT:
                    blockCtx.isBreak = true;
                    sb.append("return ");
                    CommonTree returnStmt = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.CLASSICAL_EXPR);
                    if (returnStmt != null) {
                        sb.append("(").append(blockCtx.methodCtx().methodType).append(")(");
                        process_classical_expr(sb, returnStmt, blockCtx.newExpr(), false, false);
                        sb.append(")");
                    }
                    sb.append(";\n");
/*
                    StringBuilder rsb = new StringBuilder();
                    m_process_block(rsb, childTree, blockCtx);
                    sb.append(rsb);
*/
                    break;
                case ObjcmLexer.THROW_STMT:
                    sb.append("throw new RuntimeException(\"empty\");\n");
                    break;
                case ObjcmLexer.FIELD_ACCESS:
                    StringBuilder fasb = new StringBuilder();
                    m_process_block(fasb, childTree, blockCtx);
                    if (!fieldAccess) {
                        sb.append(".");
                    } else {
                        sb.append("\"");
                    }
                    sb.append(fasb);
                    if (fieldAccess) {
                        sb.append("\")");
                    }
                    fieldAccess = false;
                    break;
                default:
                    if (testBrackets(children, i, childrenSize, child)) break;
                    StringBuilder lsb = new StringBuilder();
                    if (childTree.getChildCount() == 0) {
                        lsb.append(transformObject(childTree.getText(), blockCtx.methodCtx().classCtx, blockCtx.newExpr()));
                    } else {
                        m_process_block(lsb, childTree, blockCtx);
                    }
                    if (lsb.toString().equals("case")) {
                        wasReturn = false;
                    }
                    switch (lsb.toString()) {
                        case "@try":
                            lsb = new StringBuilder("try");
                            break;
                        case "@catch":
                            lsb = new StringBuilder("catch");
                            break;
                        case "@finally":
                            lsb = new StringBuilder("finally");
                            break;
/*
                        case "@throw":
                            lsb = new StringBuilder("throw");
                            break;
*/
                        case "_cmd":
                            lsb = new StringBuilder("\"" + blockCtx.methodCtx().methodName + "\"");
                            break;
                    }

                    // в objective-c разрешены "break" после "return" в блоках "case"
                    // необходимо это предусмотреть
                    String prepared = lsb.toString().replaceAll(";", " ;");
                    if (wasReturn && prepared.equals("break") || prepared.startsWith("break ")) {
                        // skip
                    } else {
                        sb.append(lsb).append(" ");
                    }
                    wasReturn = prepared.equals("return") || prepared.startsWith("return ");

                    break;
            }
        }
    }

    private static void process_goto(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        readChildren(sb, tree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
    }

    private static void process_for_stmt(StringBuilder sb, CommonTree tree, BlockContext blockCtx) {
        CommonTree forStmtExpr = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.FOR_STMT_EXPR);

        // определим тип for-а:
        CommonTree forInTree = (CommonTree) forStmtExpr.getFirstChildWithType(ObjcmLexer.FOR_IN_STMT);
        boolean forIn = forInTree != null;

        if (forIn) {
            sb.append("for (");

            CommonTree typeTree = (CommonTree) forStmtExpr.getFirstChildWithType(ObjcmLexer.EXPR_FULL);
            StringBuilder typeSb = new StringBuilder();
            process_expr_full(typeSb, typeTree, blockCtx.newExpr(), true);
            String type = typeSb.toString().trim();
            type = type.substring(0, type.indexOf(" "));

            // for (Xxxx ...
            sb.append(typeSb);

            // for (Xxxx yyyy ....
            CommonTree itemTree = (CommonTree) forStmtExpr.getFirstChildWithType(ObjcmLexer.CLASSICAL_EXPR_2);
            if (itemTree != null) {
                readChildren(sb, itemTree, blockCtx.methodCtx().classCtx, blockCtx.newExpr());
            }

            sb.append(" : (List<").append(type).append(">)");

            m_process_block(sb, forInTree, blockCtx.newBlock());

            sb.append(") ");
        } else {
            // само собой
            m_process_block(sb, forStmtExpr, blockCtx.newBlock());
        }

        CommonTree lastChild = (CommonTree) tree.getChild(tree.getChildCount() - 1);
        if (lastChild.getChildCount() > 0) {
            m_process_block(sb, lastChild, blockCtx.newBlock());
        } else {
            sb.append(lastChild.getText());
        }
        sb.append("\n");
/*
        for (Object child : tree.getChildren()) {

        }
*/
    }

    private static void process_variable_init(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        boolean isVariableDeclaration = tree.getFirstChildWithType(ObjcmLexer.EXPR_FULL) != null && tree.getFirstChildWithType(ObjcmLexer.CLASSICAL_EXPR_2) != null;

        String variableType = "";

        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.getType()) {
                case ObjcmLexer.EXPR_FULL: {
                    StringBuilder efsb = new StringBuilder();
                    ExpressionContext exprCtx2 = exprCtx.newExpr();
                    exprCtx2.needSaveVariable = tree.getFirstChildWithType(ObjcmLexer.CLASSICAL_EXPR_2) == null;
                    process_expr_full(efsb, childTree, exprCtx2, true);
                    variableType = efsb.toString().trim();
                    sb.append(efsb);
                    break;
                }
                case ObjcmLexer.CLASSICAL_EXPR_2: {
                    ExpressionContext exprCtx2 = exprCtx.newExpr();
                    exprCtx2.setVariableDeclaration(isVariableDeclaration);
                    // в Objective-C часто объекту с наследованным типом присваивается объект с типом супер-класса
                    if (isVariableDeclaration && (recursiveSearchExists(childTree, ObjcmLexer.ASSIGN) || (childTree.getChildCount() == 1 && childTree.getChild(0).getChildCount() == 0))) {
                        exprCtx2.setVariableDeclarationType(variableType);
                    }
                    StringBuilder varsb = new StringBuilder();
                    process_classical_expr(varsb, childTree, exprCtx2, true, false);
                    sb.append(varsb);

                    if (isVariableDeclaration && !recursiveSearchExists(childTree, ObjcmLexer.ASSIGN)) {
                        if (!variableType.trim().equals("Class")) {
                            sb.append("= new ").append(variableType);
                            if (exprCtx2.isArrayDeclaration) {
                                if (exprCtx2.arraySizes.size() == 0) {
                                    exprCtx2.arraySizes.add("0");
                                }
                                for (String arraySize : exprCtx2.arraySizes) {
                                    sb.append("[").append(arraySize).append("]");
                                }
                            } else {
                                sb.append("(").append(needInitParam(variableType)).append(")");
                            }
                        } else {
                            sb.append("= Class.class");
                        }

                        exprCtx.blockCtx.variables.put(varsb.toString().trim(), variableType);
                    }
                    break;
                }
                default:
                    readChildren(sb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
                    break;
            }
        }
    }

    private static String needInitParam(String variableType) {
        switch (variableType) {
            case "Double":
            case "Integer":
            case "Float":
                return "0";
            case "String":
                return "";
            case "Boolean":
                return "false";
        }
        return "";
    }

    private static void process_expr_full(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx, boolean leftAssign) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            if (childTree.getType() == ObjcmLexer.CLASSICAL_EXPR) {
                process_classical_expr(sb, childTree, exprCtx, leftAssign, false);
            } else if (childTree.getType() == ObjcmLexer.COMMA) {
                sb.append(", ");
            }
        }
    }

    private static void process_expr_assign(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx, boolean doWrap) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            boolean doBracketsWrap = exprCtx.isVariableDeclaration() && !exprCtx.isArrayDeclaration && exprCtx.getVariableDeclarationType() != null;
            switch (childTree.getType()) {
                case ObjcmLexer.ARRAY_INIT:
                    process_array_init(sb, childTree, exprCtx.newExpr().setNoArrayDeclaration());
                    break;
                case ObjcmLexer.CLASSICAL_EXPR:
                    StringBuilder cesb = new StringBuilder();
                    process_classical_expr(cesb, childTree, exprCtx.newExpr().setNoArrayDeclaration().setNoNeedSaveVariable(), false, false);
                    sb.append(cesb);
                    if (doBracketsWrap) {
                        sb.append(")");
                        if (Utils.isNumericType(exprCtx.getVariableDeclarationType()) && !cesb.toString().trim().startsWith("objc_") && !isJustANumber(childTree)) {
                            sb.append(Utils.getNumberMethod(exprCtx.getVariableDeclarationType()));
                        }
                    }
                    break;
                case ObjcmLexer.ASSIGN:
                    if (doWrap) break;
                default:
                    // добавляем оператор присваивания
                    readChildren(sb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);

                    // принудительное приведение типа при инициализации объекта через присваивание
                    if (doBracketsWrap) {
                        sb.append("(");
                        if (!Utils.isNumericType(exprCtx.getVariableDeclarationType())) {
                            sb.append(exprCtx.getVariableDeclarationType()).append(")(");
                        }
                    }
                    break;
            }
        }
    }

    private static boolean isJustANumber(CommonTree tree) {
        int simpleCounter = 0;
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            if (childTree.getChildCount() > 0) {
                if (!isJustANumber(childTree)) return false;
            } else {
                if (childTree.getText().equals("-")) return false;
                if (childTree.getType() == ObjcmLexer.NUMBER) {
                    simpleCounter++;
                } else {
                    return false;
                }
            }
        }
        return simpleCounter <= 1;
    }

    private static void process_array_init(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.getType()) {
                case ObjcmLexer.ARRAY_INIT:
                    process_array_init(sb, childTree, exprCtx.newExpr().setNoArrayDeclaration());
                    break;
                case ObjcmLexer.CLASSICAL_EXPR:
                    // принудительное приведение типа при инициализации объекта через присваивание
                    if (exprCtx.isVariableDeclaration() && !exprCtx.isArrayDeclaration) {
                        sb.append("(");
                        if (!Utils.isNumericType(exprCtx.getVariableDeclarationType())) {
                            sb.append(exprCtx.getVariableDeclarationType()).append(")(");
                        }
                    }

                    StringBuilder cesb = new StringBuilder();
                    process_classical_expr(cesb, childTree, exprCtx.newExpr().setNoArrayDeclaration(), false, false);
                    sb.append(cesb);
                    if (exprCtx.isVariableDeclaration()) {
                        sb.append(")");
                        if (Utils.isNumericType(exprCtx.getVariableDeclarationType()) && !cesb.toString().trim().startsWith("objc_") && !isJustANumber(childTree)) {
                            sb.append(Utils.getNumberMethod(exprCtx.getVariableDeclarationType()));
                        }
                    }
                    break;
                default:
                    // добавляем оператор присваивания
                    readChildren(sb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);

                    break;
            }
        }
    }

    private static void process_classical_expr(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx, boolean leftAssign, boolean isIncompletePrefix) {
        boolean isIf3 = tree.getFirstChildWithType(ObjcmLexer.EXPR_QUESTION) != null;
        boolean isAssign = leftAssign || recursiveSearchExists(tree, ObjcmLexer.EXPR_ASSIGN);

        boolean doWrap = false;
        if (tree.getFirstChildWithType(ObjcmLexer.EXPR_ASSIGN) != null) {
            // проверим, если первый найденный ClassicalExpr содержит FieldAccess, то оборачиваем все выражение:
            CommonTree sef = tree;
            if (tree.getType() == ObjcmLexer.CLASSICAL_EXPR) {
                sef = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.SIMPLE_EXPR);
            } else {
                sef = (CommonTree) tree.getChild(0);
            }
            boolean isLeftSimple = !(recursiveSearchExists(sef, ObjcmLexer.FIELD_ACCESS));
            if (!isLeftSimple) {
                CommonTree assignOperator = recursiveSearchTree((CommonTree) tree.getFirstChildWithType(ObjcmLexer.EXPR_ASSIGN), ObjcmLexer.ASSIGN);
                sb.append("_").append(OPS_NUMBER.get(assignOperator.getChild(0).getType())).append("(");
                doWrap = true;
            }
        }
        int ororCounter = 0;
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;

            switch (childTree.getType()) {
                case ObjcmLexer.CLASSICAL_EXPR:
                    // только внутри скобок
                    process_classical_expr(sb, childTree, exprCtx, isAssign, false);
                    break;
                case ObjcmLexer.EXPR_ASSIGN:
                    if (doWrap) {
                        sb.append(", ");
                    }
                    process_expr_assign(sb, childTree, exprCtx.newExpr(), doWrap);
                    if (doWrap) {
                        sb.append(")");
                    }
                    break;
                case ObjcmLexer.SIMPLE_EXPR:
                    if (isIf3 && ororCounter == 0) {
                        sb.append("logic(");
                    }
                    if (recursiveSearchExists(childTree, "unsigned")) {
                        exprCtx.setVariableDeclaration(true);
                        exprCtx.setVariableDeclarationType("Integer");
                    }

                    ExpressionContext exprCtx2 = exprCtx.newExpr();
                    exprCtx2.skipObjField = doWrap;  // флаг того, что не нужно превращать модификаторы доступа в функцию objc_field
                    StringBuilder sesb = new StringBuilder();
                    process_expr(sesb, childTree, exprCtx2, 0, isAssign, isIncompletePrefix);
                    sb.append(sesb);
                    if (isIf3 && ororCounter == 0) {
                        sb.append(")");
                    }
                    ororCounter++;

                    if (tree.getFirstChildWithType(ObjcmLexer.EXPR_ASSIGN) != null && !exprCtx.isVariableDeclaration()) {
                        exprCtx.setVariableDeclarationType(exprCtx.blockCtx.variables.get(sesb.toString().trim()));
                        exprCtx.setVariableDeclaration(exprCtx.getVariableDeclarationType() != null);
                    }

                    break;
                case ObjcmLexer.EXPR_QUESTION:
                    process_expr_question(sb, childTree, exprCtx.newExpr());
                    break;
                case ObjcmLexer.COLON:
                    sb.append(": ");
                    break;
                case ObjcmLexer.INDEX:
                    if (exprCtx.isVariableDeclaration()) {
                        exprCtx.isArrayDeclaration = true;
                    }
                    process_index(sb, childTree, exprCtx.newExpr());
                    break;
                default:
                    StringBuilder defSb = new StringBuilder();
                    readChildren(defSb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
                    sb.append(defSb);
                    String objectName = defSb.toString().trim();

                    putObjectInVariables(exprCtx, objectName);
                    break;
            }
        }
    }

    private static void putObjectInVariables(ExpressionContext exprCtx, String objectName) {
        if (exprCtx.isVariableDeclaration() && exprCtx.needSaveVariable) {
            exprCtx.needSaveVariable = false;
            exprCtx.blockCtx.variables.put(objectName, exprCtx.getVariableDeclarationType());
        }
    }

    private static void process_expr_question(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.getType()) {
                case ObjcmLexer.CLASSICAL_EXPR:
                    process_classical_expr(sb, childTree, exprCtx.newExpr(), false, false);
                    break;
                case ObjcmLexer.L_QUESTION:
                    sb.append("? ");
                    break;
                case ObjcmLexer.COLON:
                    sb.append(": ");
                    break;
            }
        }
    }

    private static void process_index(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.getType()) {
                case ObjcmLexer.INDEX_NUMBER:
                    StringBuilder inSb = new StringBuilder();
                    process_classical_expr(inSb, childTree, exprCtx.newExpr(), false, false);
                    if (exprCtx.isVariableDeclaration() && exprCtx.isArrayDeclaration) {
                        // skip it:
                        exprCtx.parentCtx.arraySizes.add(inSb.toString().trim());
                    } else {
                        sb.append(inSb);
                    }
                    break;
                default:
                    readChildren(sb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
                    break;
            }
        }
    }

    private static boolean recursiveSearchExists(CommonTree tree, int type) {
        if (tree.getChildCount() == 0) return false;
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            if (childTree.getType() == type) return true;
            if (childTree.getChildCount() > 0) {
                boolean result = recursiveSearchExists(childTree, type);
                if (result) return true;
            }
        }
        return false;
    }

    private static boolean recursiveSearchExists(CommonTree tree, String value) {
        if (tree.getChildCount() == 0) return false;
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            if (childTree.getChildCount() == 0 && childTree.getText().trim().equals(value)) return true;
            if (childTree.getChildCount() > 0) {
                boolean result = recursiveSearchExists(childTree, value);
                if (result) return true;
            }
        }
        return false;
    }

    private static CommonTree recursiveSearchTree(CommonTree tree, int type) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            if (childTree.getType() == type) return childTree;
            if (childTree.getChildCount() > 0) {
                CommonTree result = recursiveSearchTree(childTree, type);
                if (result != null) return result;
            }
        }
        return null;
    }

    private static void process_expr(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx, int deep, boolean leftAssign, boolean isIncompletePrefix) {
        List<Integer> operations = OPERATIONS_ORDER.get(deep);
        int exprType = EXPR_ORDER.get(deep);
        List<Integer> operationsOrder = new ArrayList<>();
        boolean saveVarType = false;
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            if (childTree.getType() == ObjcmLexer.ASTERISK && leftAssign) {
                saveVarType = true;
                continue;
            }
            if (operations.contains(childTree.getType())) {
                operationsOrder.add(childTree.getType());
            }
        }
        int exprCounter = 0;
        int operationsCounter = 0;
        int operationsLength = operationsOrder.size();
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            if (childTree.getType() == exprType) {
                if (operationsLength > 0 && exprCounter < operationsLength) {
                    sb.append("_").append(OPS_NUMBER.get(operationsOrder.get(exprCounter))).append("(");
                }

                if (deep == EXPR_ORDER.size() - 1) {
                    StringBuilder pesb = new StringBuilder();
                    process_expr_type(pesb, childTree, exprCtx, isIncompletePrefix);
                    sb.append(pesb);

                    if (saveVarType) {
                        exprCtx = exprCtx.newExpr();
                        exprCtx.setVariableDeclaration(true);
                        exprCtx.setVariableDeclarationType(pesb.toString().trim());
                    }
                } else {
                    process_expr(sb, childTree, exprCtx, deep + 1, leftAssign, isIncompletePrefix);
                }

                if (operationsLength > 0) {
                    operationsCounter++;
                    sb.append(", ");
                }
                exprCounter++;
            } else if (childTree.getType() == ObjcmLexer.SIMPLE_EXPR) {
                process_expr(sb, childTree, exprCtx, 0, leftAssign, isIncompletePrefix);
                if (operationsLength > 0) {
                    operationsCounter = 0;
                    sb.append(")");
                }
            }
        }
    }

    private static void process_generic(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            if (childTree.toString().trim().equals("*")) continue;
            sb.append(childTree.toString()).append(" ");
        }
    }

    private static void process_type_convertion(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.getType()) {
                case ObjcmLexer.GENERIC:
                    sb.append("<");
                    process_generic(sb, childTree, exprCtx);
                    sb.append(">");
                    break;
                default:
                    readChildren(sb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
                    break;
            }
        }
    }

    private static void process_expr_type(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx, boolean isIncompletePrefix) {
        boolean isUnaryMinus = false;
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.getType()) {
                case ObjcmLexer.TYPE_CONVERTION:
                    sb.append("(");
                    process_type_convertion(sb, childTree, exprCtx.newExpr().setNoTransformClassNames());
                    sb.append(")");
                    if (isIncompletePrefix) return;
                    break;
                case ObjcmLexer.EXPR_NOT:
                    boolean isRealNot = childTree.getFirstChildWithType(ObjcmLexer.L_NOT) != null;
                    if (isRealNot) {
                        sb.append("_not(");
                    }
                    if (isUnaryMinus) {
                        sb.append("_uminus(");
                    }
                    process_expr_not(sb, childTree, exprCtx.newExpr());
                    if (isRealNot) {
                        sb.append(")");
                    }
                    if (isUnaryMinus) {
                        sb.append(")");
                    }
                    break;
                case ObjcmLexer.ASTERISK:
                case ObjcmLexer.L_AND:
                    // эту хрень пропускаем
                    break;
                default:
                    StringBuilder defSb = new StringBuilder();
                    readChildren(defSb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
                    if (defSb.toString().trim().equals("-")) {
                        isUnaryMinus = true;
                    } else {
                        sb.append(defSb);
                    }
                    break;
            }
        }
    }

    private static void process_expr_not(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        // test for Field_access:
        int fieldAccessCounter = 0;
        // сначала просто считаем кол-во операций доступа
        if (tree.getFirstChildWithType(ObjcmLexer.FIELD_ACCESS) != null) {
            for (Object child : tree.getChildren()) {
                CommonTree childTree = (CommonTree) child;
                if (childTree.getType() == ObjcmLexer.FIELD_ACCESS) {
                    fieldAccessCounter++;
                }
            }
        }
        int fieldAccessCounter2 = 0;
        if (tree.getFirstChildWithType(ObjcmLexer.FIELD_ACCESS) != null) {
            for (Object child : tree.getChildren()) {
                CommonTree childTree = (CommonTree) child;
                if (childTree.getType() == ObjcmLexer.FIELD_ACCESS) {
                    fieldAccessCounter2++;
                    if (fieldAccessCounter2 < fieldAccessCounter || !exprCtx.skipObjField) {
                        sb.append("objc_field(");
                    }
                }
            }
        }
        fieldAccessCounter2 = 0;
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.getType()) {
                case ObjcmLexer.CLASSICAL_EXPR:
                    process_classical_expr(sb, childTree, exprCtx, false, false);
                    break;
                case ObjcmLexer.L_NOT:
                    break;
                case ObjcmLexer.METHOD_CALL:
                    m_process_method_call(sb, childTree, exprCtx.newExpr());
                    break;
                case ObjcmLexer.SELECTOR:
                    m_process_selector(sb, childTree, exprCtx.newExpr(), "selector");
                    break;
                case ObjcmLexer.PROTOCOL:
                    m_process_selector(sb, childTree, exprCtx.newExpr(), "protocol");
                    break;
                case ObjcmLexer.FIELD_ACCESS:
                    fieldAccessCounter2++;
                    sb.append(", \"");
                    StringBuilder sbfa = new StringBuilder();
                    readChildren(sbfa, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
                    sb.append(sbfa.toString().trim());
                    sb.append("\"");
                    if (fieldAccessCounter2 > 0 && (fieldAccessCounter2 < fieldAccessCounter || !exprCtx.skipObjField)) {
                        sb.append(")");
                    }
                    break;
                case ObjcmLexer.FUNCTION:
                    ExpressionContext exprCtx2 = exprCtx.newExpr();
                    exprCtx2.transformClassNames = true;
                    process_classical_expr(sb, childTree, exprCtx2.checkForFunctionName(), false, false);
                    break;
                case ObjcmLexer.GENERIC:
                    sb.append("<");
                    process_generic(sb, childTree, exprCtx);
                    sb.append(">");
                    break;
//                case ObjcmLexer.STRING:
//                    sb.append("NSString(");
//                    readChildren(sb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
//                    sb.append(")");
//                    break;
                default:
                    StringBuilder defSb = new StringBuilder();
                    readChildren(defSb, childTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
                    sb.append(defSb);
                    String objectName = defSb.toString().trim();

                    putObjectInVariables(exprCtx, objectName);
                    break;
            }
        }
    }

    private static void m_process_static_start(StringBuilder sb, CommonTree tree, BlockContext blockCtx) {
        StringBuilder isb = new StringBuilder();
        String variableType = "";
        CommonTree typeTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.STATIC_TYPE);
        if (typeTree != null) {
            StringBuilder vtsb = new StringBuilder();
            readChildren(vtsb, typeTree, blockCtx.methodCtx().classCtx, blockCtx.newExpr());
            variableType = vtsb.toString().trim();
        }
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            // пропускаем только слово "static"
            if (childTree.getType() == ObjcmLexer.STATIC_PREFIX) continue;

            StringBuilder lsb = new StringBuilder();
            readChildren(lsb, childTree, blockCtx.methodCtx().classCtx, blockCtx.newExpr());
            isb.append(lsb);
            if (typeTree != null && childTree.getType() == ObjcmLexer.CLASSICAL_EXPR_2) {
                if (!recursiveSearchExists(childTree, ObjcmLexer.ASSIGN)) {
                    if (!variableType.trim().equals("Class")) {
                        isb.append("= new ").append(variableType).append("(").append(needInitParam(variableType)).append(")");
                    } else {
                        isb.append("= Class.class");
                    }
                }
            }
        }
        String line = isb.toString();
        sb.append(line);
    }

    private static void m_process_while_stmt(StringBuilder sb, CommonTree tree, BlockContext cc) {
        CommonTree exprTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.WHILE_EXPR);
        StringBuilder exprsb = new StringBuilder();
        m_process_block(exprsb, exprTree, cc);

        CommonTree bodyTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.IF_BLOCK);
        StringBuilder bodysb = new StringBuilder();
        m_process_block(bodysb, bodyTree, cc);

        sb.append("while (logic(").append(exprsb).append("))\n");
        sb.append(bodysb);
    }

    private static void m_process_selector(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx, String selector) {
        exprCtx.checkForFunctionName = false;

        CommonTree selectorValueTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.SELECTOR_VALUE);
        StringBuilder selectorValueSb = new StringBuilder();
        readChildren(selectorValueSb, selectorValueTree, exprCtx.blockCtx.methodCtx().classCtx, exprCtx);

        sb.append("_").append(selector).append("_(\"").append(selectorValueSb.toString().trim()).append("\")");
    }

    private static void m_process_if_stmt(StringBuilder sb, CommonTree tree, BlockContext cc) {
        CommonTree exprTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.IF_EXPR);
        StringBuilder exprsb = new StringBuilder();
        m_process_block(exprsb, exprTree, cc);

        CommonTree blockTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.IF_BLOCK);
        StringBuilder blocksb = new StringBuilder();
        m_process_block(blocksb, blockTree, cc);

        // logic - такой метод, который всегда возращает булевский тип
        sb.append("if (logic(").append(exprsb).append(")) ");
        sb.append(blocksb);
    }

    private static boolean m_process_switch(StringBuilder sb, CommonTree tree, BlockContext blockCtx) {
        // 1. сначала проанализируем: какого типа объект используется для ветвления
        // если это обычный enum, то конструируем обычный switch
        // если это объект иного типа, то делаем обертку "getEnum(...)"

        CommonTree switchExprTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.SWITCH_EXPRESSION);
        StringBuilder esb = new StringBuilder();
        m_process_block(esb, switchExprTree, blockCtx);
        boolean useGetEnum = !esb.toString().trim().startsWith("this.");

        CommonTree switchBodyTree = (CommonTree) tree.getFirstChildWithType(ObjcmLexer.SWITCH_BODY);

        // начинаем конструировать switch
        sb.append("{\n");
        String switchUuid = UUID.randomUUID().toString().replace("-", "_");
        sb.append("\tEnum i_").append(switchUuid).append(" = ");
        if (useGetEnum) sb.append("getEnum(");
        sb.append(esb);
        if (useGetEnum) sb.append(")");
        sb.append(";\n");

        StringBuilder bodySB;
        StringBuilder defaultBody = new StringBuilder();

        // здесь LinkedHashMap, потому что нам важно сохранить порядок
        // в котором шли case-ы и default внутри оригинального switch
        Map<String, StringBuilder> switchMap = new LinkedHashMap<>();
        Map<String, Boolean> switchMapBreak = new LinkedHashMap<>();

        for (Object child : switchBodyTree.getChildren()) {
            CommonTree childTree = (CommonTree) child;

            switch (childTree.getType()) {
                case ObjcmLexer.CASE_STMT: {
                    CommonTree caseExprTree = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.CASE_EXPR);
                    StringBuilder cesb = new StringBuilder();
                    readChildren(cesb, caseExprTree, blockCtx.methodCtx().classCtx, blockCtx.newExpr());
                    bodySB = new StringBuilder();
                    CommonTree caseBodyTree = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.CASE_BODY);
                    BlockContext blockCtx2 = blockCtx.newBlock().setSkipBreak();
                    if (caseBodyTree != null) {
                        m_process_block(bodySB, caseBodyTree, blockCtx2);
                    }
                    switchMap.put(cesb.toString(), bodySB);
                    switchMapBreak.put(cesb.toString(), blockCtx2.isBreak);
                    break;
                }
                case ObjcmLexer.DEFAULT_STMT: {
                    CommonTree blockTree = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.DEFAULT_BODY);
                    if (blockTree != null) {
                        BlockContext blockCtx2 = blockCtx.newBlock().setSkipBreak();
                        m_process_block(defaultBody, blockTree, blockCtx2);
                        switchMap.put(null, defaultBody);
                        switchMapBreak.put(null, blockCtx2.isBreak);
                    }
                    break;
                }
                default:
                    // все, что не CASE и не DEFAULT нужно перенести наверх перед switch-ом
                    readChildren(sb, childTree, blockCtx.methodCtx().classCtx, blockCtx.newExpr());
                    break;
            }
        }

        List<String> keysArray = new ArrayList<>(switchMap.keySet());
        int caseNum = 0;
        for (String caseStmt : switchMap.keySet()) {
            if (caseStmt == null) continue; // операцию DEFAULT в конец!
            if (caseNum > 0) sb.append("else ");
            sb.append("if (i_").append(switchUuid).append(".equals(").append(caseStmt).append("))");
            caseNum = processSwitchCaseBody(sb, switchMap, switchMapBreak, keysArray, caseNum, caseStmt);
        }
        if (switchMap.get(null) != null) {
            sb.append("else ");
            caseNum = processSwitchCaseBody(sb, switchMap, switchMapBreak, keysArray, caseNum, null);
        }
        sb.append("}\n");

        return switchMap.get(null) != null;
    }

    private static int processSwitchCaseBody(StringBuilder sb, Map<String, StringBuilder> switchMap, Map<String, Boolean> switchMapBreak, List<String> keysArray, int caseNum, String caseStmt) {
        sb.append(" {\n");
        StringBuilder currentBlock = switchMap.get(caseStmt);
        if (!currentBlock.toString().trim().isEmpty()) {
            sb.append(currentBlock);
        }
        if (!switchMapBreak.get(caseStmt)) {
            for (int i = keysArray.indexOf(caseStmt) + 1; i < keysArray.size(); i++) {
                StringBuilder caseCode = switchMap.get(keysArray.get(i));
                if (!caseCode.toString().trim().isEmpty()) {
                    sb.append("\n\n");
                    sb.append(caseCode);
                }
                if (switchMapBreak.containsKey(keysArray.get(i))) break;
            }
        }
        sb.append("}\n");
        caseNum++;
        return caseNum;
    }

    private static boolean testBrackets(List children, int i, int childrenSize, Object child) {
        if (child.toString().equals("[")) {
            if (i < childrenSize - 1) {
                if (((CommonTree) children.get(i + 1)).getToken().getType() == ObjcmLexer.METHOD_CALL) {
                    return true;
                }
            }
        }
        if (child.toString().equals("]")) {
            if (i > 0) {
                if (((CommonTree) children.get(i - 1)).getToken().getType() == ObjcmLexer.METHOD_CALL) {
                    return true;
                }
            }
        }
        return false;
    }

    private static boolean m_process_object(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        boolean typeConv = false;
        boolean wasMethodCall = false;
        List children = tree.getChildren();
        for (int i = 0, childrenSize = children.size(); i < childrenSize; i++) {
            Object child = children.get(i);
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.TYPE_CONVERTION:
                    sb.append("(");
                    process_type_convertion(sb, childTree, exprCtx.newExpr().setNoTransformClassNames());
                    sb.append(")");
                    break;
                case ObjcmLexer.METHOD_CALL:
                    wasMethodCall = m_process_method_call(sb, childTree, exprCtx);
                    break;
                case ObjcmLexer.FIELD_ACCESS:
                    StringBuilder fasb = new StringBuilder();
                    m_process_block(fasb, childTree, exprCtx.blockCtx.newBlock());
                    sb.append(".");
                    sb.append(fasb);
                    break;
                case ObjcmLexer.FUNCTION:
                    ExpressionContext exprCtx2 = exprCtx.newExpr();
                    exprCtx2.transformClassNames = true;
                    process_classical_expr(sb, childTree, exprCtx2.checkForFunctionName(), false, false);
                    break;
                default:
                    if (testBrackets(children, i, childrenSize, child)) break;
                    sb.append(transformObject(child.toString(), exprCtx.blockCtx.methodCtx().classCtx, exprCtx));
                    break;
            }
        }
        if (typeConv) sb.append(")");
        return wasMethodCall;
    }

    private static void m_process_message(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx, boolean isIncompletePrefix) {
        List children = tree.getChildren();
        boolean prevId = false;
        for (int i = 0, childrenSize = children.size(); i < childrenSize; i++) {
            Object child = children.get(i);
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.CLASSICAL_EXPR:
                    process_classical_expr(sb, childTree, exprCtx, false, isIncompletePrefix);
                    break;
                case ObjcmLexer.METHOD_CALL:
                    m_process_method_call(sb, childTree, exprCtx);
                    break;
                case ObjcmLexer.PREFIX:
                    break;
                case ObjcmLexer.SELECTOR:
                    m_process_selector(sb, childTree, exprCtx, "selector");
                    break;
                case ObjcmLexer.PROTOCOL:
                    m_process_selector(sb, childTree, exprCtx, "protocol");
                    break;
                case ObjcmLexer.FUNCTION:
                    ExpressionContext exprCtx2 = exprCtx.newExpr();
                    exprCtx2.transformClassNames = true;
                    process_classical_expr(sb, childTree, exprCtx2.checkForFunctionName(), false, false);
                    break;
                default:
                    if (i == 0 && child.toString().equals("[")) continue;
                    if (i == childrenSize - 1 && child.toString().equals("]")) continue;
                    // ситуация типа blabla... prefix
                    if (!children.get(childrenSize - 1).toString().equals(")") && i > 0 && i == childrenSize - 2 && prevId && child.toString().matches("\\s+")) {
                        return;
                    }
                    String obj = transformObject(child.toString(), exprCtx.blockCtx.methodCtx().classCtx, exprCtx);
                    // заменяем имя класса на "имя_класса.class"
                    if (exprCtx.blockCtx.methodCtx().classCtx.projectCtx.imports.keySet().contains(obj)) {
                        obj += ".class";
                    }
                    sb.append(obj);
                    prevId = child.toString().matches("[a-zA-Z_\\d\\.]*");
                    break;
            }
        }
    }

    private static boolean m_process_method_call(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        boolean wasMethodCall = false;
        boolean instanceCreation = false;
        String object = "";
        String message = "";
        String methodName = "";
        loop:
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.OBJECT:
                    StringBuilder lsb2 = new StringBuilder();
                    wasMethodCall = m_process_object(lsb2, childTree, exprCtx.newExpr());
                    object = lsb2.toString().trim();
                    break;
                case ObjcmLexer.METHOD_NAME:
                    String name = childTree.getChild(0).toString();
                    if (RESTRICTED_METHODS.contains(name)) {
                        break loop;
                    }
                    if (name.equals("alloc")) {
                        instanceCreation = true;
                    } else {
                        methodName = Utils.transformMethodName(name);
                        wasMethodCall = true;
                    }
                    break;
                case ObjcmLexer.MSG_LIST:
                    StringBuilder lsb = new StringBuilder();
                    m_process_msg_list(lsb, childTree, exprCtx.newExpr().setTransformClassNames().checkForFunctionName());
                    message = lsb.toString();
                    break;
            }
        }

        // Специальная защита от методов с именем "new"
        if (methodName.trim().equals("new")) {
            wasMethodCall = false;
            instanceCreation = true;
        }

        if (instanceCreation && "Class".equals(exprCtx.blockCtx.variables.get(object))) {
            methodName = "newInstance";
            wasMethodCall = true;
            instanceCreation = false;
        }

        if (wasMethodCall) {
            if (object.isEmpty()) {
                throw new RuntimeException("empty object for method call");
            }
            if (methodName.isEmpty()) {
                sb.append(object);
            } else {
                if (methodName.equals("isKindOfClass") || methodName.equals("isSubclassOfClass")) {
//                    sb.append(object).append(" instanceof ").append(message.replace(".class", ""));
                    sb.append("_instanceof(").append(object).append(", ").append(message).append(")");
                } else if (object.startsWith("NS") /*|| exprCtx.blockCtx.methodCtx().classCtx.projectCtx.imports.containsKey(object) || methodName.equals("respondsToSelector")*/) {
                    sb.append(object).append(".").append(methodName).append("(").append(message).append(")");
                } else {
                    sb.append("objc_msgSend(");
                    sb.append(fixObject(object, exprCtx.blockCtx.methodCtx().classCtx.projectCtx)).append(",").append("\"").append(methodName).append("\"");
                    sb.append(", ");

                    // список категорий
                    sb.append(exprCtx.blockCtx.methodCtx().classCtx.categoryList);

                    if (!message.isEmpty()) sb.append(", ").append(message);
                    sb.append(")");
                }
            }
        } else if (instanceCreation) {
            sb.append("new ").append(object).append("()");
        }

        return wasMethodCall || instanceCreation;
    }

    private static String fixObject(String object, ProjectContext projectCtx) {
        switch (object) {
            case "super":
                return "this";
            case "String":
                return "String.class";
        }
        if (projectCtx.imports.keySet().contains(object)) {
            return object + ".class";
        }
        return object;
    }

    private static void m_process_msg_list(StringBuilder sb, CommonTree tree, ExpressionContext exprCtx) {
        int msgCount = 0;
        List children = tree.getChildren();
        for (int i = 0, childrenSize = children.size(); i < childrenSize; i++) {
            Object child = children.get(i);
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.METHOD_MSG:
                    boolean isIncompletePrefix = i < childrenSize - 1 && ((CommonTree) children.get(i + 1)).getFirstChildWithType(ObjcmLexer.INCOMPLETE_PREFIX) != null;
                    CommonTree messageTree = (CommonTree) childTree.getFirstChildWithType(ObjcmLexer.MESSAGE);
                    if (messageTree != null) {
                        if (msgCount > 0) sb.append(", ");
                        m_process_message(sb, messageTree, exprCtx.newExpr().setTransformClassNames(), isIncompletePrefix);
                        msgCount++;
                    }
                    break;
            }
        }
    }


    private static void m_process_param(Map<String, String> params, CommonTree tree, ClassContext cc) {
        String type = "";
        String name = "";
        String generic = "";
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.TYPE:
                    type = childTree.getChild(0).toString();
                    break;
                case ObjcmLexer.NAME:
                    name = childTree.getChild(0).toString();
                    break;
            }
        }

        params.put(name, transformType(type, cc));
    }

    private static void m_process_field2(StringBuilder sb, CommonTree tree, ClassContext cc) {
        String modifier = "";
        String type = "";
        String name = "";
        String value = "";
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.MODIFIER:
                    modifier = childTree.getChild(0).toString();
                    break;
                case ObjcmLexer.TYPE:
                    type = childTree.getChild(0).toString();
                    break;
                case ObjcmLexer.NAME:
                    name = childTree.getChild(0).toString();
                    break;
                case ObjcmLexer.VALUE:
                    value = m_process_field_value(childTree, cc);
                    break;
            }
        }

        sb.append("public ").append((modifier.length() > 0 ? modifier + " " : "")).append(transformType(type, cc)).append(" ").append(name);
        if (value.length() > 0) {
            sb.append(" = ").append(value);
        }
        sb.append(";\n");
    }

    private static void m_process_field(StringBuilder sb, CommonTree tree, ClassContext classCtx) {
        String type = "";
        String name = "";
        String value = "";
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case ObjcmLexer.TYPE:
                    type = childTree.getChild(0).toString();
                    break;
                case ObjcmLexer.NAME:
                    name = childTree.getChild(0).toString().trim();
                    break;
                case ObjcmLexer.VALUE:
                    value = m_process_field_value(childTree, classCtx);
                    break;
            }
        }

        classCtx.projectCtx.staticFields.put(name, classCtx.className);

        sb.append("public static ").append(transformType(type, classCtx)).append(" ").append(name);
        if (value.length() > 0) {
            sb.append(" = ").append(value);
        }
        sb.append(";\n");
    }

    private static String m_process_field_value(CommonTree tree, ClassContext classContext) {
        StringBuilder sb = new StringBuilder();
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            classContext.newMethod(null, null, false, null);
            sb.append(transformObject(childTree.toString(), classContext, null));
        }
        return sb.toString();
    }

}
