
public class User {
    /**
     * error message of validation step*/
    private String error = "";
    /**
     * Constructor of user
     * @param username username of user
     * @param dens denominations of password2
     * @param password1 p1 of user
     * @param password2 p2 of user*/
    public User(String username, String password1, int password2, int [] dens) {
        Username userController = new Username(username);

        if(userController.getErrorCount() > 0)
            error = userController.getErrorMessage(0);
        else {
            Password1 p1Controller = new Password1(username, password1);

            if(p1Controller.getErrorCount() > 0)
                error = p1Controller.getErrorMessage(0);

            else{
                Password2 p2Controller = new Password2(password2, dens);
                if(p2Controller.getErrorCount() > 0)
                    error = p2Controller.getErrorMessage(0);
            }
        }
    }

    public String getError() {
        return error;
    }
}
