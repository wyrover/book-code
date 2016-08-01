import org.junit.Test;
import ru.andremoniy.objctojavacnv.tokenize.StringTokenizer;

import static org.testng.Assert.assertEquals;

/**
 * User: Andremoniy
 * Date: 18.09.12
 * Time: 22:50
 */
public class TestTokenizer {

    @Test
    public void testStringTokenizing() {
        String testStr = "db executeUpdate:@\"INSERT INTO ColorSchemeItemsFill (Color, colorSchemeId, TableCellBackgroundColor,Comment,Alpha) VALUES (?, ?, ?, \\\"blabla\\\", ?)\"";
        StringTokenizer st = new StringTokenizer(testStr);
        assertEquals("db", st.nextToken().value);
        assertEquals(" ", st.nextToken().value);
        assertEquals("executeUpdate", st.nextToken().value);
        assertEquals(":", st.nextToken().value);
        assertEquals("@\"INSERT INTO ColorSchemeItemsFill (Color, colorSchemeId, TableCellBackgroundColor,Comment,Alpha) VALUES (?, ?, ?, \\\"blabla\\\", ?)\"", st.nextToken().value);
    }

}
