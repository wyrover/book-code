import org.junit.Test;
import ru.andremoniy.objctojavacnv.antlr.Macros;
import ru.andremoniy.objctojavacnv.antlr.Preprocessor;
import ru.andremoniy.objctojavacnv.context.ProjectContext;

import java.util.Arrays;

import static junit.framework.Assert.assertEquals;

/**
 * User: Andremoniy
 * Date: 19.09.12
 * Time: 8:49
 */
public class TestPreprocessor {

    @Test
    public void testReplace() {
        String code = "- (DKLayer*)\t\tobjectInLayersAtIndex:(NSUInteger) layerIndex\n" +
                "{\n" +
                "\tNSAssert1( layerIndex < [self countOfLayers], @\"bad layer index %ld (overrange)\", (long)layerIndex);\n" +
                "\t\n" +
                "\treturn [[self layers] objectAtIndex:layerIndex];\n" +
                "}\n";


        Macros macros = new Macros();
        macros.setName("NSAssert1");
        macros.getParams().add("condition");
        macros.getParams().add("desc");
        macros.getParams().add("arg1");
        macros.setFileName("blabla");
        macros.setReplace("_NSAssertBody((condition), (desc), (arg1), 0, 0, 0, 0)");

        ProjectContext ctx = new ProjectContext();
        ctx.macrosMap.put("NSAssert1", Arrays.asList(macros));
        String replaced = Preprocessor.replace(code, ctx, "test");

        assertEquals("- (DKLayer*)\t\tobjectInLayersAtIndex:(NSUInteger) layerIndex\n" +
                        "{\n" +
                        "\t_NSAssertBody((layerIndex < [self countOfLayers]), (@\"bad layer index %ld (overrange)\"), ((long)layerIndex), 0, 0, 0, 0);\n" +
                        "\t\n" +
                        "\treturn [[self layers] objectAtIndex:layerIndex];\n" +
                        "}\n", replaced);
    }

}
