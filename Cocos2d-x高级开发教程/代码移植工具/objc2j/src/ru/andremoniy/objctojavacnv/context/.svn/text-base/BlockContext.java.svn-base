package ru.andremoniy.objctojavacnv.context;

/**
 * User: Andremoniy
 * Date: 05.10.12
 * Time: 14:21
 */
public class BlockContext extends AbstractContext {

    public ExpressionContext exprCtx;
    private BlockContext parentBlock;
    private MethodContext methodCtx;
    public boolean isBreak;
    public boolean skipBreak;

    public BlockContext(MethodContext methodContext) {
        this.methodCtx = methodContext;
    }

    public ExpressionContext newExpr() {
        exprCtx = new ExpressionContext(this);
        return exprCtx;
    }

    public MethodContext methodCtx() {
        if (parentBlock != null) return parentBlock.methodCtx();
        return methodCtx;
    }

    public BlockContext newBlock() {
        BlockContext newBlock = new BlockContext(methodCtx);
        newBlock.parentBlock = this;
        newBlock.variables.putAll(this.variables);
        return newBlock;
    }

    public BlockContext setSkipBreak() {
        this.skipBreak = true;
        return this;
    }
}
