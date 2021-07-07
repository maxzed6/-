import java.util.Locale;

public class Vigenere {
    public static StringBuffer encrypt(String key, String plaintext){
        StringBuffer cipher = new StringBuffer();
        char[] k = key.toCharArray();
        char[] p = plaintext.toCharArray();
        for (int i=0; i < plaintext.length(); i++){
            int temp = ((p[i]-'a') + (k[i%key.length()]-'a')) % 26;
            cipher.append((char)(temp+'a'));
        }
        return cipher;
    }

    public static StringBuffer decrypt(String key, String cipher){
        StringBuffer plaintext = new StringBuffer();
        char[] k = key.toCharArray();
        char[] c = cipher.toCharArray();
        for (int i=0; i < cipher.length(); i++){
            int temp = (((c[i]-'a')) - (k[i%key.length()]-'a') + 26) % 26;
            plaintext.append((char)(temp+'a'));
        }
        return plaintext;
    }

    public static void main(String[] args) {
        String key = "KEY";
        String plaintext = "plaintext";
        StringBuffer result;
        result = encrypt(key.toLowerCase(Locale.ROOT), plaintext.toLowerCase(Locale.ROOT));
        String plain = result.toString();
        System.out.println(result);
        result = decrypt(key.toLowerCase(Locale.ROOT), plain.toLowerCase(Locale.ROOT));
        System.out.println(result);
    }
}


