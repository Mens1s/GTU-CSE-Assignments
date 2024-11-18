using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;
using Vuforia;

public class GameManager : MonoBehaviour
{
    private ObserverBehaviour observerBehaviour;

    public static int currentStep = 0;
    public int targetStep;

    void Start()
    {
        observerBehaviour = GetComponent<ObserverBehaviour>();
        if (observerBehaviour)
        {
            observerBehaviour.OnTargetStatusChanged += OnTargetStatusChanged;
        }
    }

    private void OnDestroy()
    {
        if (observerBehaviour)
        {
            observerBehaviour.OnTargetStatusChanged -= OnTargetStatusChanged;
        }
    }

    private void OnTargetStatusChanged(ObserverBehaviour behaviour, TargetStatus targetStatus)
    {
        // Hedef bulunduðunda tetiklenecek
        if (targetStatus.Status == Status.TRACKED || targetStatus.Status == Status.EXTENDED_TRACKED)
        {
            OnTargetFound();
        }
        // Hedef kaybolduðunda tetiklenecek
        else if (targetStatus.Status == Status.NO_POSE)
        {
            OnTargetLost();
        }
    }

    private void OnTargetFound()
    {
        // Eðer bu hedef sýrasýna geldiysek
        if (targetStep == currentStep)
        {
            if (targetStep == 0 && !GameManagerReference.Instance.isRobotFound)
            {
                StartCoroutine(ShowFirstBurnText());
                GameManagerReference.Instance.audioSource.PlayOneShot(GameManagerReference.Instance.infoAudios[0], 1f);
                GameManagerReference.Instance.isRobotFound = true;
            }
            else if (targetStep == 1 && !GameManagerReference.Instance.isCarFound)
            {
                GameManagerReference.Instance.audioSource.PlayOneShot(GameManagerReference.Instance.infoAudios[1], 1f);
                StartCoroutine(ShowEngieSeeCarText());
                GameManagerReference.Instance.driveButton.gameObject.SetActive(true);
                GameManagerReference.Instance.isCarFound = true;

            }
            else if (targetStep == 2 && GameManagerReference.Instance.isDrivedCar)
            {
                StartCoroutine(FindDrill());
                GameManagerReference.Instance.audioSource.PlayOneShot(GameManagerReference.Instance.infoAudios[2], 1f);
                GameManagerReference.Instance.learnWhoIsTheKing.gameObject.SetActive(true);
            }
            currentStep++;
        }
        else
        {
            if(targetStep == 1 && !GameManagerReference.Instance.isRobotFound)
            {
                GameManagerReference.Instance.infoBox.text = "You have to find Engie first!";
            }else if(targetStep == 2 && !GameManagerReference.Instance.isDrivedCar)
            {
                GameManagerReference.Instance.infoBox.text = "You have to find Engie and Rac first! Also drive RAC!";
            }
            
        }
    }

    private void OnTargetLost()
    {
        
    }

    
    IEnumerator ShowFirstBurnText()
    {
        Debug.Log("Show first");
        GameManagerReference.Instance.todoIcon.gameObject.SetActive(false);
        GameManagerReference.Instance.awardIcon.gameObject.SetActive(true);
        GameManagerReference.Instance.infoBox.text = "You found Engie!";

        GameManagerReference.Instance.subtitle.text = "";
        foreach (char letter in "Where am I?")
        {
            GameManagerReference.Instance.subtitle.text += letter; // Harfleri sýrayla ekle
            yield return new WaitForSeconds(0.085f); // Her harften sonra biraz bekle
        }
        GameManagerReference.Instance.subtitle.text = "";

        foreach (char letter in "This doesn't look like Cripton.")
        {
            GameManagerReference.Instance.subtitle.text += letter; // Harfleri sýrayla ekle
            yield return new WaitForSeconds(0.085f); // Her harften sonra biraz bekle
        }
        GameManagerReference.Instance.subtitle.text = "";

        foreach (char letter in "Uhhhhhh.")
        {
            GameManagerReference.Instance.subtitle.text += letter; // Harfleri sýrayla ekle
            yield return new WaitForSeconds(0.085f); // Her harften sonra biraz bekle
        }
        GameManagerReference.Instance.subtitle.text = "";

        foreach (char letter in "Yes, there's a metal heap over there, it looks like the raks from our world.")
        {
            GameManagerReference.Instance.subtitle.text += letter; // Harfleri sýrayla ekle
            yield return new WaitForSeconds(0.085f); // Her harften sonra biraz bekle
        }
        GameManagerReference.Instance.subtitle.text = "";

        foreach (char letter in "I hope it works.")
        {
            GameManagerReference.Instance.subtitle.text += letter; // Harfleri sýrayla ekle
            yield return new WaitForSeconds(0.085f); // Her harften sonra biraz bekle
        }
        yield return new WaitForSeconds(0.8f);
        GameManagerReference.Instance.subtitle.text = "";
        GameManagerReference.Instance.awardIcon.gameObject.SetActive(false);
        GameManagerReference.Instance.todoIcon.gameObject.SetActive(true);
        GameManagerReference.Instance.infoBox.text = "Found rak, it looks like car in Earth!";

    }

    IEnumerator ShowEngieSeeCarText()
    {
        GameManagerReference.Instance.todoIcon.gameObject.SetActive(false);
        GameManagerReference.Instance.awardIcon.gameObject.SetActive(true);
        GameManagerReference.Instance.infoBox.text = "You Found Rak.";

        GameManagerReference.Instance.subtitle.text = "";
        foreach (char letter in "Okay, lets drive it. I think its control same as in our world.")
        {
            GameManagerReference.Instance.subtitle.text += letter; // Harfleri sýrayla ekle
            yield return new WaitForSeconds(0.085f); // Her harften sonra biraz bekle
        }
        GameManagerReference.Instance.subtitle.text = "";
        GameManagerReference.Instance.awardIcon.gameObject.SetActive(false);
        GameManagerReference.Instance.todoIcon.gameObject.SetActive(true);
        GameManagerReference.Instance.infoBox.text = "Click button to get in to Rak!";
    }

    IEnumerator FindDrill()
    {
        GameManagerReference.Instance.todoIcon.gameObject.SetActive(false);
        GameManagerReference.Instance.awardIcon.gameObject.SetActive(true);
        GameManagerReference.Instance.infoBox.text = "You Found Drill.";

        GameManagerReference.Instance.subtitle.text = "";
        foreach (char letter in "This world is so dark, who is the king of this world?")
        {
            GameManagerReference.Instance.subtitle.text += letter; // Harfleri sýrayla ekle
            yield return new WaitForSeconds(0.085f); // Her harften sonra biraz bekle
        }
        GameManagerReference.Instance.subtitle.text = "";
        GameManagerReference.Instance.awardIcon.gameObject.SetActive(false);
        GameManagerReference.Instance.todoIcon.gameObject.SetActive(true);
        GameManagerReference.Instance.infoBox.text = "Click button and see the president! Scene changes.";
    }
}
