import org.junit.Test;

/**
 * Created with IntelliJ IDEA.
 * User: 1
 * Date: 04.10.12
 * Time: 9:56
 * To change this template use File | Settings | File Templates.
 */
public class TestClassCreation {

    @Test
    public void testClasses() throws IllegalAccessException, InstantiationException {

        final Class x = Integer.class;
        x.newInstance();
    }
}