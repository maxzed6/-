import java.math.BigInteger;
import java.util.Arrays;

public class ECC {
    static int a = 2;
    static BigInteger p = BigInteger.valueOf(2).pow(192).subtract(BigInteger.valueOf(2).pow(64)).subtract(BigInteger.valueOf(1));
//    static BigInteger b = new BigInteger("64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1", 16);
    static BigInteger x = new BigInteger("188DA80EB03090F67CBf20EB43A18800F4FF0AFD82FF1012", 16);
    static BigInteger y = new BigInteger("07192B95FFC8DA78631011ED6B24CDD573F977A11E794811", 16);
//    static BigInteger n = new BigInteger("FFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831", 16);
    static BigInteger[] G = {x, y};
//    static int h = 1;

    public static BigInteger[][] toBigIntegerArray(String cipher){
        String[] c = cipher.split("]");
        cipher = cipher.replaceAll("]\\[", ",");
        String[] cSplit = cipher.split(",");
        BigInteger[][] result = new BigInteger[c.length][4];
        int j = 0, i = 0;
        for (; j < c.length; j++){
            for (int k = 0; k < 4; k++){
                cSplit[i] = cSplit[i].replace('[', ' ');
                cSplit[i] = cSplit[i].replace(']', ' ');
                cSplit[i] = cSplit[i].strip();
                result[j][k] = new BigInteger(cSplit[i++]);
            }
        }
        return result;
    }

    public static BigInteger[] add(BigInteger[] point1, BigInteger[] point2){
        BigInteger lamda;
        BigInteger[] point3 = new BigInteger[2];
        if (point1[0].equals(point2[0]) && point1[1].equals(point2[1])){
            lamda = ((point1[0].pow(2)).multiply(BigInteger.valueOf(3)).add(BigInteger.valueOf(a))).multiply(point1[1].multiply(BigInteger.valueOf(2)).modPow(new BigInteger("-1"), p)).mod(p);
            point3[0] = lamda.pow(2).subtract(point1[0].multiply(new BigInteger("2"))).mod(p);
            point3[1] = lamda.multiply(point1[0].subtract(point3[0])).subtract(point1[1]).mod(p);
        } else if (point1[0].equals(point2[0]) && point1[1].equals(point2[1].negate())){
            return null;
        } else {
            lamda = (point2[1].subtract(point1[1])).multiply((point2[0].subtract(point1[0])).modPow(new BigInteger("-1"), p)).mod(p);
            point3[0] = lamda.pow(2).subtract(point1[0]).subtract(point2[0]).mod(p);
            point3[1] = lamda.multiply(point1[0].subtract(point3[0])).subtract(point1[1]).mod(p);
        }
        return point3;
    }

    public static BigInteger[] generatePublic(long privateKey){
        BigInteger[] pointA = G;
        for (long i = 0; i < privateKey; i++)
        {
            if (pointA != null) {
                pointA = add(pointA, G);
            } else {
                System.out.println("failed! choose another key");
                return null;
            }
        }
        return pointA;
    }

    public static BigInteger[][] encrypt(String plaintext, BigInteger[] publicKey){
        BigInteger[][] cipher = new BigInteger[plaintext.length()][4];
        char[] p = plaintext.toCharArray();
        BigInteger[] pointm = G;
        BigInteger[] tempG = G;
        long r = (long)(1+Math.random()*100000);
//        long r = 10;
        if (r != 0) {
            BigInteger[] tempA = publicKey;
            for (int k = 0; k < r; k++) {
                assert tempG != null;
                tempG = add(tempG, G);
                assert publicKey != null;
                publicKey = add(tempA, publicKey);
            }
        }
        for (int j = 0; j < plaintext.length(); j++){
            int m = p[j];
            for (int l = 0; l < m; l++){
                assert pointm != null;
                pointm = add(G, pointm);
            }
            assert tempG != null;
            cipher[j][0] = tempG[0];
            cipher[j][1] = tempG[1];
            assert publicKey != null;
            assert pointm != null;
            cipher[j][2] = pointm[0].add(publicKey[0]);
            cipher[j][3] = pointm[1].add(publicKey[1]);
        }
        return cipher;
    }

    public static StringBuffer decrypt(BigInteger[][] cipher, long privateKey){
        BigInteger[] temp = new BigInteger[2];
        BigInteger[] krG = new BigInteger[2];
        BigInteger[] tempkrG = new BigInteger[2];
        BigInteger[] temp2 = G;
        StringBuffer plaintext = new StringBuffer();
        int ord = 0;
        for (BigInteger[] bigIntegers : cipher) {
            krG[0] = bigIntegers[0];
            krG[1] = bigIntegers[1];
            tempkrG[0] = bigIntegers[0];
            tempkrG[1] = bigIntegers[1];
            for (long j = 0; j < privateKey; j++) {
                assert krG != null;
                krG = add(tempkrG, krG);
            }
            assert krG != null;
            temp[0] = bigIntegers[2].subtract(krG[0]);
            temp[1] = bigIntegers[3].subtract(krG[1]);
            while (true) {
                assert temp2 != null;
                if (temp2[0].equals(temp[0])) break;
                temp2 = add(temp2, G);
                ord++;
            }
            plaintext.append((char) ord);
            ord = 0;
        }
        return plaintext;
    }

    public static void main(String[] args) {
        BigInteger[][] cipher;
        BigInteger[] publicKey = generatePublic(12);
        cipher = encrypt("asdadasdasd", publicKey);
//        cipher = toBigIntegerArray("[3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 7331938958529803370083368851695646554025910817138052448636, 3518730320171519592661643763079896497748029908576161586766][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 3701597672242239866541542882628505196167333350006174402833, 4271685582411050551233146009595781146628980552974536800040][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 4982558807427491536185063711209655214917251395631906016766, 6609853284223455341214678332257766416845520415526826687195][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 7053655352154559886958499329266956780446595921045032624687, 6155890156836956457768515707506749652068765994774409070761][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 3806623352217829595854517804632386932066894782458901510365, 2845810182774348181883669065930962662814523950908515339317][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 8521256951235635445295478961983477786368920328055227055544, 4683547166474281337777670774387785763094368792425992962515][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 7158661760432607007631013434490785838923146341784755914285, 4218326501594648598131628227696440661614559585202945938077][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 6790297675871356121141363489554156126863223517090717364194, 3085791899975714148891901905740398269283238393858973619924][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 6325261705975755432957737157929222754123053379215306693403, 1071476967274575677857055403662181748573167862237053410850][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 5827040159385369046988744340678198940846852792591511201867, 3100017133930367846216823623753953571954470629190792321328][3657692166549959653991720957359025099352303832326404285728, 3587486231766525220963290129906967601844868472016117403858, 4709755798600179406520505389980256798276054147669640350656, 5348803617674690702299269118445217683572404280604481056483]");
        for (BigInteger[] bigIntegers : cipher) {
            System.out.println(Arrays.toString(bigIntegers));
        }
        StringBuffer s;
        s = decrypt(cipher, 12);
        System.out.println(s);
    }
}
