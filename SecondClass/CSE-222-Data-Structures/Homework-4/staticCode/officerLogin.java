public class officerLogin {
    /**
     * Login and see errors
     * @return login or not*/
    public static boolean login(String password1, int password2, int [] denominations, String username){
        newUserLogin();
        boolean isLogin = true;
        int printedErrorCount = 1;

        if(!Username.checkIfValidUsername(username)) {
            System.out.println("The username is invalid due to;");
            isLogin = false;
            //printedErrorCount = Username.getErrorCount();

            for(int i = 0; i < printedErrorCount; i++)
                System.out.println(Username.getErrorMessage(i));
        }

        // if you want to see all error messages delete comment lines and else
        else if(!Password1.checkPassword1(username, password1)) {
            System.out.println("The password1 is invalid due to;");
            isLogin = false;

            //printedErrorCount = Password1.getErrorCount();
            for(int i = 0; i < printedErrorCount; i++)
                System.out.println(Password1.getErrorMessage(i));

        }

        // if you want to see all error messages delete comment lines and else
        else if(!Password2.isValidPassword2(password2, denominations)){
            System.out.println("The password2 is invalid due to;");
            isLogin = false;

            //printedErrorCount = Password2.getErrorCount();
            for(int i = 0; i < printedErrorCount; i++)
                System.out.println(Password2.getErrorMessage(i));
        }

        return isLogin;
    }
    /**
     * set as all static variables to new user login */
    private static void newUserLogin(){
        Password1.newUserLogin();
        Password2.newUserLogin();
        Username.newUserLogin();
    }
}
