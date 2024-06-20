public class Password2 {
    /**
     * error message of password2 */
    private String [] errorMessage = new String[10];
    /**
     * error count of password2 */
    private int errorCount = 0;
    /**
     * constructor of username class
     * @param password2 password2 of user
     * @param denominations den of user*/
    public Password2(int password2, int [] denominations){
        isValidPassword2(password2, denominations);
    }

    /**
     * checks password2 has an exact division
     * this does not need any sorted array :)
     * @return password2 contains exact division or not*/
    private boolean isExactDivision(int password2, int [] denominations) {
        if (password2 == 0) {
            return true;
        }

        if (denominations.length == 0) {
            return false;
        }

        for (int i = 0; i <= password2 / denominations[0]; i++) {
            if (isExactDivision(password2 - i * denominations[0], copyOfRange(denominations, 1, denominations.length))) {
                return true;
            }
        }

        return false;
    }
    private static int[] copyOfRange(int[] original, int from, int to) {
        int newLength = to - from;
        if (newLength < 0) {
            throw new IllegalArgumentException("Invalid range");
        }
        int[] copy = new int[newLength];
        System.arraycopy(original, from, copy, 0, Math.min(original.length - from, newLength));
        return copy;
    }
    /**
     * control of password2 is valid or not
     * @return password2 is valid or not*/
    private boolean isValidPassword2(int password2, int [] denominations) {
        if(password2 < 10 || password2 > 10000 )
            errorMessage[errorCount++] = "It should be between 10 and 10,000!";

        if(isExactDivision(password2, denominations))
            return true;

        errorMessage[errorCount++] = "It is not compatible with the denominations.";
        return false;
    }

    /**
     * returns error message
     * @return error message*/
    public String getErrorMessage(int idx){return errorMessage[idx];}
    /**
     * return error count
     * @return error count*/
    public int getErrorCount(){return errorCount;}

}