package ru.andremoniy.objctojavacnv;

import org.antlr.runtime.RecognitionException;
import org.antlr.runtime.tree.CommonTree;
import ru.andremoniy.objctojavacnv.antlr.Preprocessor;
import ru.andremoniy.objctojavacnv.antlr.output.PreprocessorParser;
import ru.andremoniy.objctojavacnv.context.ClassContext;
import ru.andremoniy.objctojavacnv.context.ExpressionContext;
import ru.andremoniy.objctojavacnv.context.ProjectContext;

import java.util.*;

/**
 * User: Andremoniy
 * Date: 18.06.12
 * Time: 9:33
 */
public class Utils {

    private static final List<String> implementedCocoa = Arrays.asList("NSEvent");
    private static final List<String> NUMERIC_TYPES = Arrays.asList("Integer", "Double", "Short", "Long");

    static String transformObject(String obj, ClassContext classCtx, ExpressionContext exprCtx) {
        switch (obj) {
            case "_cmd":
                return "\"" + classCtx.methodCtx.methodName + "\"";
            case "NULL":
            case "nil":
            case "Nil":
                return "null";
            case "self":
                if (classCtx.methodCtx.staticFlag) {
                    return classCtx.className + (exprCtx.transformClassNames ? ".class" : "");
                } else {
                    return "this";
                }
            case "TRUE":
            case "YES":
                return "true";
            case "NO":
                return "false";
            case "static":
                return "";
//            case "NSAssert":
//                return "assert";
            case "MAX":
                return "Math.max";
            case "_CGFloatFloor":
                return "(int)Math.floor";
            case "fmodf":
                return "MathEx.fmodf";
            case ";":
                return ";\n";
        }
        if (obj.startsWith("@\"")) obj = obj.substring(1);
        obj = transformType(obj, classCtx);
        if (classCtx != null &&
                exprCtx != null &&
                exprCtx.transformClassNames &&
                classCtx.projectCtx.imports.containsKey(obj.trim())) {
            obj = obj + ".class";
        } else {
            if (classCtx != null) {
                String _import = classCtx.projectCtx.staticFields.get(obj);
                if (_import != null && !classCtx.addImports.contains(_import)) {
                    classCtx.addImports.add(_import);
                }
            }
        }
        if (exprCtx != null && exprCtx.checkForFunctionName) {
            boolean isVariable = false;
            for (String varName : exprCtx.blockCtx.variables.keySet()) {
                if (varName.equals(obj)) {
                    isVariable = true;
                    break;
                }
            }
            if (!isVariable) {
                for (String varName : exprCtx.blockCtx.methodCtx().variables.keySet()) {
                    if (varName.equals(obj)) {
                        isVariable = true;
                        break;
                    }
                }
                if (!isVariable) {
                    for (String varName : exprCtx.blockCtx.methodCtx().classCtx.variables.keySet()) {
                        if (varName.equals(obj)) {
                            isVariable = true;
                            break;
                        }
                    }
                }
            }
            if (!isVariable) {
                for (String methodName : exprCtx.blockCtx.methodCtx().classCtx.methodsInterfaces.keySet()) {
                    if (methodName.equals(obj)) {
                        return "\"" + methodName + "\"";
                    }
                }
            }
        }
        return obj;
    }

    static String transformMethodName(String name) {
        switch (name) {
            case "isEqualToString":
                return "equals";
            case "class":
                return "";
        }
        return name;
    }

    static String transformType(String type, ClassContext classCtx) {
        if (type.matches("void\\s*\\*")) return "Object";
        // отрезаем *
        if (type.endsWith("*")) type = type.substring(0, type.length() - 1);

        switch (type) {
            case "NSUInteger":
            case "NSInteger":
            case "signed":
            case "int":
            case "unsigned":
                return "Integer";
            case "id":
                return "NSObject";
            case "NSString":
                return "String";
            case "BOOL":
                return "Boolean";
            case "IBAction":
                return "void";
            case "CGFloat":
            case "doule":
            case "float":
                return "Double";
            case "long":
                return "Long";
            case "short":
                return "Short";
        }

        if (classCtx != null && !classCtx.localProcessedImports.contains(type) && !classCtx.addImports.contains(type)) {
            classCtx.addImports.add(type);
        } else if (classCtx != null && type.startsWith("NS")) {
            classCtx.addImports.add(type);
        }

        return type;
    }

    public static void addAdditionalImports(StringBuilder sb, ProjectContext projectCtx) {
        Set<String> nsImports = new HashSet<>();
        for (String addImport : projectCtx.classCtx.addImports) {
            String _addImport = addImport;
            if (addImport.contains(".")) {
                _addImport = addImport.substring(0, addImport.indexOf("."));
            }
            String classPath = projectCtx.imports.get(_addImport);
            if (classPath == null && _addImport.startsWith("I")) {
                _addImport = _addImport.substring(1);
                classPath = projectCtx.imports.get(_addImport);
            }
            if (classPath != null) {
                // reverse replace classname to appropriate
                if (!_addImport.equals(addImport)) {
                    classPath = classPath.substring(0, classPath.lastIndexOf(".") + 1) + addImport;
                }
                if (!classPath.contains("+")) {
                    sb.append("import ").append(classPath).append(";\n");
                    sb.append("import static ").append(classPath).append(".*;\n");
                    addNSHeaderImport(sb, classPath);
                }
                addStaticFromHeaderImports(projectCtx, sb, classPath.substring(classPath.lastIndexOf(".") + 1));
            } else if (addImport.startsWith("NS")) {
                if (!nsImports.contains(addImport) && implementedCocoa.contains(addImport)) {
                    sb.append("import static ru.andremoniy.jcocoa.I").append(addImport).append(".*;\n");
//                    sb.append("import ru.andremoniy.jcocoa.I").append(addImport).append(".*;\n");
                    nsImports.add(addImport);
                }
            }
        }
        sb.append("\n");
    }

    private static void addNSHeaderImport(StringBuilder sb, String classPath) {
        String importClassName = classPath.substring(classPath.lastIndexOf(".") + 1);
        if (importClassName.startsWith("NS")) {
            String interfaceName = classPath.substring(0, classPath.lastIndexOf(".")) + ".I" + importClassName;
            sb.append("import ").append(interfaceName).append(";\n");
            sb.append("import static ").append(interfaceName).append(".*;\n");
        }
    }

    public static void addStaticFromHeaderImports(ProjectContext ctx, StringBuilder sb, String className) {
        List<String> enums = ctx.headerEnums.get("I" + className);
        if (enums != null) {
            String headerPath = ctx.imports.get(className);
            headerPath = headerPath.substring(0, headerPath.length() - className.length()) + "I" + className;
            if (headerPath.contains("+")) {
                headerPath = headerPath.substring(0, headerPath.indexOf("+"));
            }
            for (String _enum : enums) {
                sb.append("import static ").append(headerPath).append(".").append(_enum).append(".*;\n");
            }
        }
    }

    public static String getText(CommonTree tree) {
        if (tree == null) return "";
        if (tree.getChildCount() == 0) return tree.getText();
        StringBuilder sb = new StringBuilder();
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            sb.append(getText(childTree)).append(" ");
        }
        return sb.toString();
    }

    /**
     * add original imports from file
     *
     * @param input file code source
     * @param sb    java code string builder
     */
    public static void addOriginalImports(String input, ProjectContext projectCtx, StringBuilder sb) throws RecognitionException {
        Map<String, String> imports = projectCtx.imports;
        PreprocessorParser.code_return result = Preprocessor.getResult(input);
        for (Object child : ((CommonTree) result.getTree()).getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case PreprocessorParser.T_INCLUDE:
                    String path = getText(childTree);
                    String className = path.substring(1, path.length() - 3);
                    String classPath = imports.get(className);
                    if (classPath != null) {
                        if (className.contains("+")) {
                            // сохраняем инфу о добавленных категориях, но в import не добавляем
                            projectCtx.classCtx.localCategories.add(className);
                        } else {
                            sb.append("import ").append(classPath).append(";\n");
                            sb.append("import static ").append(classPath).append(".*;\n");
                            addNSHeaderImport(sb, classPath);
                        }
                        addStaticFromHeaderImports(projectCtx, sb, className);
                        projectCtx.classCtx.localProcessedImports.add(className);
                    }
                    break;
            }
        }

    }

    public static boolean isNumericType(String type) {
        if (type == null) return false;
        type = type.trim();
        return (NUMERIC_TYPES.contains(type));
    }

    public static String getNumberMethod(String type) {
        type = type.trim();
        switch (type) {
            case "Integer":
                return ".intValue()";
            case "Long":
                return ".longValue()";
            case "Double":
                return ".doubleValue()";
            case "Byte":
                return ".byteValue()";
            case "Float":
                return ".floatValue()";
        }
        return "";
    }

}