using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

using TMPro;
public class ScreenHandler : MonoBehaviour
{
    public void GoToFirstScreen()
    {
        SceneManager.LoadSceneAsync(1);
    }

    public void GoToSecondScreen()
    {
        SceneManager.LoadSceneAsync(2);
    }

    public void GoToThirdScreen()
    {
        SceneManager.LoadSceneAsync(3);
    }
}
