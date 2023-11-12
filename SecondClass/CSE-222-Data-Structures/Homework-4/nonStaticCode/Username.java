
public class Username {
    /**
     * String array which holds all errorMessages for password1*/
    private String [] errorMessage = new String[10];
    /**
     * error count of password2 */
    private int errorCount = 0;
    /**
     * constructor of username class
     * @param username username of user*/
    public Username(String username) {
        checkIfValidUsername(username);
    }
    /**
     * this function controls username is valid or not
     * @return it is valid or not*/
    private boolean checkIfValidUsername(String username) {
        if (username.length() < 1) {
            errorMessage[errorCount++] = "Username cannot be empty!";
            return false;
        }
        return checkIfValidUsername(username, 0);
    }
    /**
     * this function controls username is valid or not with recursive function
     * @return it is valid or not*/
    private boolean checkIfValidUsername(String username, int idx){
        if( Character.isLetter(username.charAt(idx)) ){
            if(idx == username.length() - 1) return true;
            return checkIfValidUsername(username, idx + 1);
        }
        errorMessage[errorCount++] = "Username has to be contain only letters!";
        return false;
    }

    /**
     * returns error messages
     * @return error messages*/
    public String getErrorMessage(int idx){return errorMessage[idx];}
    /**
     * return error count
     * @return error count*/
    public int getErrorCount(){return errorCount;}
}
