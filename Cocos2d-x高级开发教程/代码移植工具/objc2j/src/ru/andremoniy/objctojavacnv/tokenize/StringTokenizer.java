package ru.andremoniy.objctojavacnv.tokenize;

/**
 * User: Andremoniy
 * Date: 16.09.12
 * Time: 14:40
 */
public class StringTokenizer {

    private static final char[] delimiters = "`~-=+!@#$%^&*()[]{};:'\",<.>/?\\|\n\t \r".toCharArray();

    private char[] chars;
    private long length;

    private int pos;

    public StringTokenizer(String str) {
        this.chars = str.toCharArray();
        length = str.length();
    }

    public StringToken nextToken() {
        if (pos >= length) return null;
        StringToken token = new StringToken();
        do {
            char c = chars[pos];
            pos++;
            if (isDelimeter(c)) {
                // это начало строки
                if (c == '@' && token.isEmpty()) {
                    token.value += c;
                    continue;
                }
                // это начало строки
                if (c == '"' && (token.isEmpty() || token.value.equals("@"))) {
                    token.value += c;
                    token.isString = true;
                    continue;
                }
                // это конец строки
                if (c == '"' && token.isString) {
                    // проверим, что это не строка внутри другой строки:
                    token.value += c;
                    if (pos > 0 && chars[pos - 2] == '\\') {
                        continue;
                    }
                    return token;
                }
                // все что внутри строки глотаем
                if (token.isString) {
                    token.value += c;
                    continue;
                }
                if (!token.isEmpty()) {
                    pos--;
                } else {
                    token.value = "" + c;
                    token.isDelimeter = true;
                }
                return token;
            }
            token.value += c;
        } while (pos < length);
        return (token.isEmpty() ? null : token);
    }

    public static boolean isDelimeter(char c) {
        for (int i = 0; i < delimiters.length; i++) {
            if (delimiters[i] == c) return true;
        }
        return false;
    }
}
