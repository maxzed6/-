import java.util.Locale;

public class AffineCipher {
    public static boolean check(String key){
        if (key.indexOf('x') < 0 || key.indexOf('+') < 0){
            return false;
        }
        int n1 = key.indexOf('x');
        int n2 = key.indexOf('+');
        String tempA = key.substring(0, n1);
        String tempB = key.substring(n2+1, key.length());
        char[] a = tempA.toCharArray();
        char[] b = tempB.toCharArray();
        for (int i = 0; i < a.length; i++){
            if ((int)a[i] < '0' || (int)a[i] > '9'){
                return false;
            }
        }
        for (int i = 0; i < b.length; i++){
            if ((int)b[i] < '0' || (int)b[i] > '9'){
                return false;
            }
        }
        return true;
    }

    public static StringBuffer encrypt(String key, String plaintext){//ax+b
        StringBuffer cipher = new StringBuffer();
        char[] p = plaintext.toCharArray();
        int n1 = key.indexOf('x');
        int n2 = key.indexOf('+');
        String tempA = key.substring(0, n1);
        String tempB = key.substring(n2+1, key.length());
        int a = Integer.parseInt(tempA);
        int b = Integer.parseInt(tempB);
        for (int i=0; i<plaintext.length(); i++){
            int temp = (a * (p[i]-'a') + b) % 26;
            cipher.append((char)(temp+'a'));
        }
        return cipher;
    }

    static int x,y;

    public static void exGCD(int a, int b){//a mod b
        if (b == 0){
            x = 1;
            y = 0;
            return;
        }
       exGCD(b, a % b);//ax+by=bx'+(a%b)y'
        int t = x;
        x = y;
        y = t - a / b * x;
    }

    public static StringBuffer decrypt(String key, String cipher){
        StringBuffer plaintext = new StringBuffer();
        char[] k = key.toCharArray();
        char[] p = cipher.toCharArray();
        int n1 = key.indexOf('x');
        int n2 = key.indexOf('+');
        String tempA = key.substring(0, n1);
        String tempB = key.substring(n2+1, key.length());
        int a = Integer.parseInt(tempA);
        int b = Integer.parseInt(tempB);
        exGCD(a, 26);
        for (int i=0; i < cipher.length(); i++){
            int temp = (x * ((p[i]-'a') - b)) % 26;
            if (temp < 0) {
                temp += 26;
            }
            plaintext.append((char)(temp+'a'));
        }
        return plaintext;
    }

    public static void main(String[] args) {
        String key = "5x+90";
        System.out.println(check(key));
        String plaintext = "affinecipher";
        StringBuffer result = encrypt(key.toLowerCase(Locale.ROOT), plaintext.toLowerCase(Locale.ROOT));
        String plain = result.toString();
        System.out.println(result);
        result = decrypt(key.toLowerCase(Locale.ROOT), plain.toLowerCase(Locale.ROOT));
        System.out.println(result);
    }
}
