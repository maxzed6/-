import java.util.Locale;

public class playfair {
    static char[][] matrix = new char[5][5];

    public static void initMatrix(String key){
        char[] k = key.toCharArray();
        for (int i=0; i < key.length(); i++){
            for (int j=i+1; j < key.length(); j++){
                if (k[i] == k[j]) {
                    k[i] = ' ';
                    break;
                }
            }
        }
        int n = 0;
        char a = 'a';
        for (int i=0; i < 5; i++){
            for (int j=0; j < 5; j++){
                if (n < key.length()){
                    if (k[n] == ' '){
                        n++;
                        j--;
                        continue;
                    }
                    matrix[i][j] = k[n++];
                } else {
                    if (a == 'j') {
                        a++;
                        j--;
                        continue;
                    }
                    if (a == 'i' && key.indexOf('j') != -1){
                        a++;
                        j--;
                        continue;
                    }
                    if (key.indexOf(a) == -1){
                        matrix[i][j] = a++;
                    } else {
                        j--;
                        a++;
                    }
                }
            }
        }
    }

    public static int[] find(char c){
        int[] re = new int[2];
        if (c == 'j'){
            c = 'i';
        }
        for (int i=0; i < 5; i++){
            for (int j=0; j < 5; j++){
                if (matrix[i][j] == c){
                    re[0] = i;
                    re[1] = j;
                    return re;
                }
            }
        }
        return re;
    }

    public static StringBuffer encrypt(String key, String plaintext){
        initMatrix(key);
        int[] pos1;
        int[] pos2;
        StringBuffer cipher = new StringBuffer();
        if (plaintext.length() % 2 != 0){
            plaintext += "q";
        }
        char[] p = plaintext.toCharArray();
        for (int i=0; i < plaintext.length()-1; i+=2){
            pos1 = find(p[i]);
            pos2 = find(p[i+1]);
            if (pos1[0] == pos2[0]){
                cipher.append(matrix[pos1[0]][(pos1[1]+1)%5]);
                cipher.append(matrix[pos2[0]][(pos2[1]+1)%5]);
            } else if (pos1[1] == pos2[1]){
                cipher.append(matrix[(pos1[0]+1)%5][pos1[1]]);
                cipher.append(matrix[(pos2[0]+1)%5][pos2[1]]);
            } else {
                cipher.append(matrix[pos1[0]][pos2[1]]);
                cipher.append(matrix[pos2[0]][pos1[1]]);
            }
        }
        return cipher;
    }

    public static StringBuffer decrypt(String key, String cipher){
        initMatrix(key);
        int[] pos1;
        int[] pos2;
        StringBuffer plaintext = new StringBuffer();
        char[] c = cipher.toCharArray();
        for (int i=0; i < cipher.length()-1; i+=2){
            pos1 = find(c[i]);
            pos2 = find(c[i+1]);
            if (pos1[0] == pos2[0]){
                plaintext.append(matrix[pos1[0]][(pos1[1]-1+5)%5]);
                plaintext.append(matrix[pos2[0]][(pos2[1]-1+5)%5]);
            } else if (pos1[1] == pos2[1]){
                plaintext.append(matrix[(pos1[0]-1)%5][pos1[1]]);
                plaintext.append(matrix[(pos2[0]-1)%5][pos2[1]]);
            } else {
                plaintext.append(matrix[pos1[0]][pos2[1]]);
                plaintext.append(matrix[pos2[0]][pos1[1]]);
            }
        }
        return plaintext;
    }

    public static void main(String[] args) {
        String key = "KEEEjjY";
        String plaintext = "plaintext";
        StringBuffer result;
        result = encrypt(key.toLowerCase(Locale.ROOT), plaintext.toLowerCase(Locale.ROOT));
        String plain = result.toString();
        System.out.println(result);
        result = decrypt(key.toLowerCase(Locale.ROOT), plain.toLowerCase(Locale.ROOT));
        System.out.println(result);
    }
}
