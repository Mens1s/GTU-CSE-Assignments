
import java.util.Stack;

public class Password1 {

    /**
     * String array which holds all errorMessages for password1*/
    private String [] errorMessage = new String[10];
    /**
     * error count of password2 */
    private int errorCount = 0;
    /** 
     * String that holds only letter of char it removes ([{ */
    private String password1Cleaned = "";
    /**
     * String that holds only brackets */
    private String password1Brackets = "";
    /**
     * constructor of password1
     * @param username username of user
     * @param password1 password1 of user*/
    public Password1(String username, String password1){
        checkPassword1( username,  password1);
    }
    /**
     * cleanPassword1 cleans password and removes brackets from user input and update 2 variable
     * @param password1 user input password1 */
    private void cleanPassword1(String password1) {
        for(int idx = 0; idx < password1.length(); idx++){
            if(password1.charAt(idx) == '(' || password1.charAt(idx) == ')' || password1.charAt(idx) == '[' || password1.charAt(idx) == ']' || password1.charAt(idx) == '{' || password1.charAt(idx) == '}')
                password1Brackets += password1.charAt(idx);
            else
                password1Cleaned += password1.charAt(idx);
        }
    }
    /**
     * this functions checks password1 contains letter from username
     * @param username user username
     * @return boolean value of password1 contains username or not*/
    private boolean containsUserNameSpirit(String username) {
        Stack<String> stack = new Stack<>();
        // this not efficient solution look it back man :)
        for(int idx = 0; idx < password1Cleaned.length(); idx++) {
            char chr = password1Cleaned.charAt(idx);

            if(stack.isEmpty() || !username.contains(String.valueOf(chr) ) ){
                stack.push(String.valueOf(chr));
            } else{
                return true;
            }
        }
        return false;
    }
    /**
     * function checks password brackets closed truthfully
     * @return password brackets closed or not*/
    private boolean isBalancedPassword() {
        Stack<Character> stack = new Stack<>();

        for(int idx = 0; idx < password1Brackets.length(); idx++) {
            char instantChar = password1Brackets.charAt(idx);

            switch (instantChar) {
                case '(', '[', '{' -> stack.push(instantChar);
                case ')' -> {
                    if (stack.isEmpty() || stack.pop() != '(')
                        return false;
                }
                case ']' -> {
                    if (stack.isEmpty() || stack.pop() != '[')
                        return false;
                }
                case '}' -> {
                    if (stack.isEmpty() || stack.pop() != '{')
                        return false;
                }
            }
        }

        return stack.isEmpty();
    }
    /**
     * function checks password1 contains only letter and brackets
     * @return it is holds brackets and letter or not*/
    private boolean containsOnlyLetter() {

        for(int idx = 0; idx < password1Cleaned.length(); idx++)
            if(!Character.isLetter(password1Cleaned.charAt(idx)))
                return false;
        return true;

    }
    /**
     * this function checks given password1 is palindroble or not
     * works method is so simple it takes firstChar, and removes from string and after that control it if it is or not if not and this situation occurs twice it return false
     * otherwise return true
     * @return if is polindrable*/
    private boolean isPalindromePossible(String password1, int oddCount){
        // odd length password1 control
        if (password1.length() == 1)
            return oddCount != 1;
        // even length password1 control
        else if(password1.length() == 0) return true;

        char firstChar = password1.charAt(0);
        password1 = password1.substring(1);

        int idx = password1.indexOf(firstChar);

        if(idx == -1){
            oddCount++;
            if(oddCount > 1)
                return false;
            return isPalindromePossible(password1, oddCount);
        }

        return isPalindromePossible(password1.substring(0, idx) + password1.substring(idx+1), oddCount);
    }
    /**
     * this function aim is controlling password1 is correct or not
     * @return password1 is true or not*/
    public boolean checkPassword1(String username, String password1){
        cleanPassword1(password1);

        if(password1.length() < 8)
            errorMessage[errorCount++] = "Password1 length is not enough. Minimum length is 8 character";

        if(password1Brackets.length() < 4) // I don't understand that brackets has to be closed or not so my aim count 2 brackets ({[) like that
            errorMessage[errorCount++] = "Bracket count has to be minimum 4!";

        if(password1Cleaned.length() < 1) // I don't understand that brackets has to be closed or not so my aim count 2 brackets ({[) like that
            errorMessage[errorCount++] = "It should have letters too.";

        if(!containsOnlyLetter())
            errorMessage[errorCount++] = "Password1 has to be include only letters and '[], {}, ()' ";

        if(!containsUserNameSpirit(username))
            errorMessage[errorCount++] = "Password1 does not include username character!";

        if(!isBalancedPassword())
            errorMessage[errorCount++] = "Password1 parenthesis does not match!";

        if(!isPalindromePossible(password1Cleaned, 0))
            errorMessage[errorCount++] = "Password1 must be palindroble!";

        return errorCount == 0;

    }

    /**
     * returns error messages
     * @return error messages*/
    public String getErrorMessage(int idx){return errorMessage[idx];}
    /**
     * return error count
     * @return error count*/
    public int getErrorCount(){return errorCount;}}
