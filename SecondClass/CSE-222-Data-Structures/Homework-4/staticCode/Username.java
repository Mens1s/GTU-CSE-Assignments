public class Username {
    /**
     * String array which holds all errorMessages for password1*/
    static private String [] errorMessage = new String[10];
    /**
     * error count of password2 */
    static private int errorCount = 0;
    /**
     * this function controls username is valid or not
     * @return it is valid or not*/
    static public boolean checkIfValidUsername(String username) {
        if (username.length() < 1) {
            errorMessage[errorCount++] = "Username cannot be empty!";
            return false;
        }
        return checkIfValidUsername(username, 0);
    }
    /**
     * this function controls username is valid or not with recursive function
     * @return it is valid or not*/
    static private boolean checkIfValidUsername(String username, int idx){
        if( Character.isLetter(username.charAt(idx)) ){
            if(idx == username.length() - 1) return true;
            return checkIfValidUsername(username, idx + 1);
        }
        errorMessage[errorCount++] = "Username has to be contain only letters!";
        return false;
    }
    /**
     * it cleans static variables as 0*/
    public static void newUserLogin() {errorCount = 0;}

    /**
     * returns error messages
     * @return error messages*/
    public static String getErrorMessage(int idx){return errorMessage[idx];}
    /**
     * return error count
     * @return error count*/
    public static int getErrorCount(){return errorCount;}
}
