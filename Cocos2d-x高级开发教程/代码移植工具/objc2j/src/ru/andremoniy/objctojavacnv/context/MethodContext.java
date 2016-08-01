package ru.andremoniy.objctojavacnv.context;

/**
 * User: Andremoniy
 * Date: 05.10.12
 * Time: 9:04
 */
public class MethodContext extends AbstractContext {

    public BlockContext blockCtx;
    public ClassContext classCtx;

    public boolean staticFlag;
    public String methodName;
    public boolean addReturnNull;
    public String methodType;

    public MethodContext(String methodName, String methodType, boolean staticFlag, ClassContext classCtx) {
        this.methodName = methodName;
        this.methodType = methodType;
        this.staticFlag = staticFlag;
        this.classCtx = classCtx;
    }

    public BlockContext newBlock() {
        blockCtx = new BlockContext(this);
        blockCtx.variables.putAll(this.variables);
        return blockCtx;
    }

}
