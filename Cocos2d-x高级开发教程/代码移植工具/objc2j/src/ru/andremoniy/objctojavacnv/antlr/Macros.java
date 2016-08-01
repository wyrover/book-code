package ru.andremoniy.objctojavacnv.antlr;

import org.antlr.runtime.tree.CommonTree;
import ru.andremoniy.objctojavacnv.antlr.output.PreprocessorParser;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

/**
 * User: Andremoniy
 * Date: 09.07.12
 * Time: 9:45
 */
public class Macros {

    private String name;
    private List<String> params = new ArrayList<>();
    private Pattern pattern;
    private String fileName;

    private String replace;

    private boolean simpleReplace;

    public Macros() {
    }

    public Macros(String name, String replace) {
        this.name = name;
        this.replace = replace;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public List<String> getParams() {
        return params;
    }

    public void setParams(List<String> params) {
        this.params = params;
    }

    public String getReplace() {
        return replace;
    }

    public void setReplace(String replace) {
        this.replace = replace;
    }

    public Pattern getPattern() {
        return pattern;
    }

    public void setPattern(Pattern pattern) {
        this.pattern = pattern;
    }

    public String getFileName() {
        return fileName;
    }

    public static Macros build(CommonTree tree, String fileName) {
        Macros macros = new Macros();
        macros.fileName = fileName;

        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case PreprocessorParser.T_ID:
                    processId(macros, childTree);
                    break;
                case PreprocessorParser.T_REPLACE:
                    processReplace(macros, childTree);
                    break;
            }
        }

        if (macros.replace == null) macros.replace = "";

        return macros;
    }

    private static void processReplace(Macros macros, CommonTree tree) {
        StringBuilder replace = new StringBuilder();
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case PreprocessorParser.T_BACKSLASH:
                    replace.append("//#%#%");
                    break;
                default:
                    replace.append(child.toString()).append(" ");
                    break;
            }
        }
        macros.replace = replace.toString();
    }

    private static void processId(Macros macros, CommonTree tree) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case PreprocessorParser.T_NAME:
                    macros.name = childTree.getChild(0).toString();
                    break;
                case PreprocessorParser.T_IN_BRACKETS:
                    processParams(macros, childTree);
                    break;
            }
        }
    }

    private static void processParams(Macros macros, CommonTree tree) {
        for (Object child : tree.getChildren()) {
            CommonTree childTree = (CommonTree) child;
            switch (childTree.token.getType()) {
                case PreprocessorParser.T_PARAM:
                    macros.params.add(childTree.getChild(0).toString());
                    break;
                default:
                    if (childTree.getChildCount() == 0 && childTree.getText().equals("...")) {
                        macros.params.add("...");
                    }
                    break;
            }
        }
    }

    public static Macros getMacros(String name, List<Macros> macrosList) {
        for (Macros macros : macrosList) {
            if (macros.getName().equalsIgnoreCase(name)) return macros;
        }
        return null;
    }

    public boolean isSimpleReplace() {
        return simpleReplace;
    }

    public void setSimpleReplace(boolean simpleReplace) {
        this.simpleReplace = simpleReplace;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Macros macros = (Macros) o;

        if (name != null ? !name.equals(macros.name) : macros.name != null) return false;
        if (params != null ? !params.equals(macros.params) : macros.params != null) return false;
        if (replace != null ? !replace.equals(macros.replace) : macros.replace != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = name != null ? name.hashCode() : 0;
        result = 31 * result + (params != null ? params.hashCode() : 0);
        result = 31 * result + (replace != null ? replace.hashCode() : 0);
        return result;
    }
}
