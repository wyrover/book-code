package ru.andremoniy.objctojavacnv.context;

import java.util.ArrayList;
import java.util.List;

/**
 * User: Andremoniy
 * Date: 05.10.12
 * Time: 9:04
 */
public class ExpressionContext {

    public boolean needSaveVariable;
    boolean isVariableDeclaration;

    public ExpressionContext parentCtx;
    String variableDeclarationType;
    public BlockContext blockCtx;
    public boolean skipObjField;
    public boolean transformClassNames;
    public boolean isArrayDeclaration;
    public List<String> arraySizes = new ArrayList<>();
    public boolean checkForFunctionName;

    public ExpressionContext setNeedSaveVariable() {
        needSaveVariable = true;
        return this;
    }

    public ExpressionContext(BlockContext blockCtx) {
        this.blockCtx = blockCtx;
    }

    public ExpressionContext checkForFunctionName() {
        checkForFunctionName = true;
        return this;
    }

    public ExpressionContext newExpr() {
        ExpressionContext newExpr = new ExpressionContext(blockCtx);
        newExpr.parentCtx = this;
        newExpr.transformClassNames = this.transformClassNames;
        newExpr.skipObjField = this.skipObjField;
        newExpr.needSaveVariable = this.needSaveVariable;
        newExpr.isArrayDeclaration = this.isArrayDeclaration;
        newExpr.isVariableDeclaration = this.isVariableDeclaration;
        newExpr.variableDeclarationType = this.variableDeclarationType;
        newExpr.checkForFunctionName = checkForFunctionName;
        return newExpr;
    }

    public ExpressionContext setTransformClassNames() {
        this.transformClassNames = true;
        return this;
    }

    public ExpressionContext setNoTransformClassNames() {
        this.transformClassNames = false;
        return this;
    }

    public ExpressionContext setNoArrayDeclaration() {
        this.isArrayDeclaration = false;
        return this;
    }

    public ExpressionContext setNoNeedSaveVariable() {
        this.needSaveVariable = false;
        return this;
    }

    public String getVariableDeclarationType() {
        return variableDeclarationType;
    }

    public void setVariableDeclarationType(String variableDeclarationType) {
        this.variableDeclarationType = variableDeclarationType;
        if (this.parentCtx != null) {
            parentCtx.setVariableDeclarationType(variableDeclarationType);
        }
    }

    public boolean isVariableDeclaration() {
        return isVariableDeclaration;
    }

    public void setVariableDeclaration(boolean variableDeclaration) {
        isVariableDeclaration = variableDeclaration;
        if (this.parentCtx != null) {
            parentCtx.setVariableDeclaration(variableDeclaration);
        }
    }
}
