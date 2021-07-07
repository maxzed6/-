import java.util.Locale;

public class Permutation {
    public static boolean check(String key){
        char[] k = key.toCharArray();
        for (int i = 0; i < key.length(); i++){
            if (k[i] < '0' || k[i] > '9'){
                return false;
            }
        }
        return true;
    }

    public static StringBuffer encrypt(String key, String plaintext){
        StringBuffer cipher = new StringBuffer();
        char[] k = key.toCharArray();
        char[] p = plaintext.toCharArray();
        for (int i=0; i < plaintext.length(); i++){
            cipher.append(p[k[i%key.length()]-1-'0'+((i/key.length())*key.length())]);
        }
        return cipher;
    }

    public static char[] decrypt(String key, String cipher){
        char[] plaintext = new char[cipher.length()];
        char[] k = key.toCharArray();
        char[] c = cipher.toCharArray();
        for (int i=0; i < cipher.length(); i++){
            plaintext[k[i%key.length()]-1-'0'+((i/key.length())*key.length())] = c[i];
        }
        return plaintext;
    }

    public static void main(String[] args) {
        String key = "132";
        System.out.println(check(key));
        String plaintext = "plaintext";
        StringBuffer result;
        result = encrypt(key.toLowerCase(Locale.ROOT), plaintext.toLowerCase(Locale.ROOT));
        String plain = result.toString();
        System.out.println(result);
        char[] result2 = decrypt(key.toLowerCase(Locale.ROOT), plain.toLowerCase(Locale.ROOT));
        System.out.println(result2);
    }
}
