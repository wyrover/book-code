package ru.andremoniy.objctojavacnv.context;

import java.util.*;

/**
 * User: Andremoniy
 * Date: 05.10.12
 * Time: 9:04
 */
public class ClassContext extends AbstractContext {

    public MethodContext methodCtx;
    public ProjectContext projectCtx;

    public boolean containsInit;
    public boolean containsAutoRelease;

    // current class name
    public String className;

    // current class category name
    public String categoryName;

    // current class package name
    public String packageName;

    // локально добавленные категории
    public Set<String> localCategories = new HashSet<>();
    public String categoryList;

    public List<String> localProcessedImports = new ArrayList<>();
    public List<String> addImports = new ArrayList<>();

    public Map<String, MethodInterface> methodsInterfaces;

    public ClassContext(String className, String categoryName, ProjectContext projectCtx, Map<String, MethodInterface> methodsInterfaces) {
        this.className = className;
        this.projectCtx = projectCtx;
        this.categoryName = categoryName;
        this.methodsInterfaces = methodsInterfaces;
    }

    public MethodContext newMethod(String methodName, String methodType, boolean staticFlag, LinkedHashMap<String, String> params) {
        methodCtx = new MethodContext(methodName, methodType, staticFlag, this);
        methodCtx.variables.putAll(variables);

        if (methodName != null) {
            MethodInterface mi = new MethodInterface();
            if (params != null) {
                mi.types.addAll(params.values());
            }
            methodsInterfaces.put(methodName, mi);
        }

        return methodCtx;
    }

    public ExpressionContext newExpr() {
        return methodCtx.blockCtx.newExpr();
    }

    public ClassContext copyNoStatic() {
        return null;  //To change body of created methods use File | Settings | File Templates.
    }

    public String className() {
        if (className.contains("+")) {
            return className.substring(0, className.indexOf("+"));
        } else {
            return className;
        }
    }
}
