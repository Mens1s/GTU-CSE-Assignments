
public class Main {
    public static void main(String[] args) {
        int testCaseCount = 11;

        String [] username = new String[testCaseCount];
        String [] password1 = new String[testCaseCount];
        int [][] denominations = new int[testCaseCount][3];

        username[0] = "sibelgulmez";username[1] = "";username[2] = "sibel1";username[3] = "sibel";username[4] = "sibel";
        username[5] = "sibel";username[6] = "sibel";username[7] = "sibel";username[8] = "sibel";username[9] = "sibel";username[10] = "sibel";

        password1[0] = "[rac()ecar]";password1[1] = "[rac()ecar]";password1[2] = "[rac()ecar]";password1[3] = "pass[]";password1[4] = "abcdabcd";
        password1[5] = "[[[[]]]]";password1[6] = "[no](no)";password1[7] = "[rac()ecar]]";password1[8] = "[rac()ecars]";password1[9] = "[rac()ecar]";password1[10] = "[rac()ecar]";

        int password2 = 75;

        denominations[0][0] = 29;
        denominations[0][1] = 4;
        denominations[0][2] = 17;

        for(int i = 0 ; i < testCaseCount; i++) {
            if(i == 10) password2 = 35;
            else if(i == 9) password2 = 5;

            System.out.println("Test - "+ (i+1) +"...\nUsername : "+username[i]+"\nPassword1 : "+password1[i]+"\nPassword2 : "+password2+"\nDenominations : 29, 4, 17");

            if(officerLogin.login(password1[i], password2, denominations[0], username[i]))
                System.out.println("The username and passwords are valid!. The door is opening, please wait...");

            System.out.println("\n\n");

            if(i == 10) password2 = 75;
            else if(i == 9) password2 = 75;

        }

    }
}
