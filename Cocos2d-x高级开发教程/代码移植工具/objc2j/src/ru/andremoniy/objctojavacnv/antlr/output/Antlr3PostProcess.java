package ru.andremoniy.objctojavacnv.antlr.output;

import org.apache.tools.ant.Task;

import java.io.*;

public class Antlr3PostProcess extends Task {
    private File oldTarget = null;
    private String pkg = null;

    private File newTarget= null;
    private File currentInterface = null;
    private int interfaceIndex = 0;
    private int lineIndex = 0;
    private final int MAXLINEINDEX = 1000;
    private String name = null;

    public Antlr3PostProcess(){
    }

    public String getPkg() {
        return pkg;
    }

    public void setPkg(String pkg) {
        this.pkg = pkg;
    }

    public File getTarget() {
        return oldTarget;
    }

    public void setOldTarget(File oldTarget) {
        name = oldTarget.getName().substring(0,oldTarget.getName().indexOf('.'));
        this.oldTarget = oldTarget;
        newTarget = new File(oldTarget.getParentFile(),name+"_TMPANTLR3POST.java");
    }

    public void execute() {
        try{
         boolean createFile =  newTarget.createNewFile();
        moveBitSetConstantsToInterface();
        //delete the old target and rename the new target
        oldTarget.delete();
        newTarget.renameTo(oldTarget);
        } catch (Exception e){

        }
    }

    private void moveBitSetConstantsToInterface() throws IOException {
      //open the target file
        BufferedReader reader = new BufferedReader(new FileReader(oldTarget));
        BufferedWriter writerInterface = getNewInterfaceWriter();
        BufferedWriter writerNewTarget = new BufferedWriter(new FileWriter(newTarget));

        //... Loop as long as there are input lines.
        String line = null;
        while ((line=reader.readLine()) != null) {
            if(lineIndex > MAXLINEINDEX) {
                lineIndex =0;
                closeInterface(writerInterface)  ;
                writerInterface  = getNewInterfaceWriter();
            }
            if(line.startsWith("    public static final BitSet FOLLOW_")){
                lineIndex++;
                writerInterface.write(line);
                writerInterface.newLine();   // Write system dependent end of line.
            } else if (line.startsWith("public class "+name+" extends Parser")) {
                writerNewTarget.write("public class "+name+" extends Parser implements "+getInterfaceName() +" {");
                writerNewTarget.newLine();   // Write system dependent end of line.
            }
            else {
                writerNewTarget.write(line);
                writerNewTarget.newLine();   // Write system dependent end of line.
            }

        }
        closeInterface(writerInterface);
        //... Close reader and writer.
        reader.close();  // Close to unlock.
        //writerInterface.close();  // Close to unlock and flush to disk.
        writerNewTarget.close();  // Close to unlock and flush to disk.
        createInterfaceGroup();
    }


    private void createInterfaceGroup() throws IOException {
        currentInterface = new File(oldTarget.getParentFile(),getInterfaceName()+".java");
        boolean createFile =currentInterface.createNewFile();
        BufferedWriter writerInterface =  new BufferedWriter(new FileWriter(currentInterface));
        writerInterface.write("package "+pkg+";");
        writerInterface.newLine();
        writerInterface.write("import org.antlr.runtime.BitSet;");
        writerInterface.newLine();
        writerInterface.write("public interface "+getInterfaceName()+" extends ");
        writerInterface.write(getInterfaceName()+"1");
        for(int i=2;i<=interfaceIndex;i++){
            writerInterface.write(","+getInterfaceName()+i);
        }
        writerInterface.write(" {");
        writerInterface.newLine();
        writerInterface.write("}");
        writerInterface.close();

    }

    private void closeInterface(BufferedWriter writerInterface) throws IOException {
        writerInterface.newLine();
        writerInterface.write("}");
        writerInterface.close();
    }

    private BufferedWriter getNewInterfaceWriter() throws IOException {
        BufferedWriter writerInterface =  new BufferedWriter(new FileWriter(getNextInterfaceFile()));
        createInterfaceHeading(writerInterface,currentInterface.getName().substring(0,currentInterface.getName().indexOf('.')),pkg);
        return writerInterface;

    }

    private void createInterfaceHeading(BufferedWriter writerInterface,String interfaceName, String interfacePkg) throws IOException {
        writerInterface.write("package "+interfacePkg+";");
        writerInterface.newLine();
        writerInterface.write("import org.antlr.runtime.BitSet;");
        writerInterface.newLine();
        writerInterface.write("public interface "+interfaceName+" {");
        writerInterface.newLine();
    }

    private File getNextInterfaceFile() throws IOException {
        interfaceIndex++;
        currentInterface = new File(oldTarget.getParentFile(),getInterfaceName()+interfaceIndex+".java");
        boolean createFile =currentInterface.createNewFile();
        return currentInterface;
    }

    private String getInterfaceName() {
        return name+"BitSet";
    }
}