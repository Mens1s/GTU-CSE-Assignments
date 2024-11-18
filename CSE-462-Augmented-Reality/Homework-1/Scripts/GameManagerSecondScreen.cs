using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class GameManagerSecondScreen : MonoBehaviour
{
    private static int FoundInformationCount = 0;

    public GameObject firstTick;
    public GameObject secondTick;
    public GameObject thirdTick;
    
    public Button ThirdSceneButton;

    public TMP_Text subtitleText;

    private static bool isImageFound = false;
    private static bool isVideoFound = false;
    private static bool isTextFound  = false;
    private void findInformation()
    {
        if (FoundInformationCount == 1)
        {
            firstTick.gameObject.SetActive(true);
        }
        else if (FoundInformationCount == 2)
        {
            secondTick.gameObject.SetActive(true);
        }
        else if (FoundInformationCount == 3)
        {
            thirdTick.gameObject.SetActive(true);
            ThirdSceneButton.gameObject.SetActive(true);
        }
    }

    public void FindImage()
    {
        if (!isImageFound)
        {
            isImageFound = true;
            FoundInformationCount++;
            findInformation();
            subtitleText.text = "You Found Image. Find others!";
        }
    }

    public void FindVideo()
    {
        if (!isVideoFound)
        {
            isVideoFound = true;
            FoundInformationCount++;
            findInformation();
            subtitleText.text = "You Found Video. Find others!";
        }
    }

    public void FindText()
    {
        if (!isTextFound)
        {
            isTextFound = true;
            FoundInformationCount++;
            findInformation();
            subtitleText.text = "You Found Text. Find others!";
        }
    }

}
