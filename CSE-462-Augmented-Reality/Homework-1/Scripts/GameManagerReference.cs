using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;
using Vuforia;
using UnityEngine.SceneManagement;

public class GameManagerReference : MonoBehaviour
{
    public static GameManagerReference Instance { get; private set; }

    public TMP_Text infoBox;
    public TMP_Text subtitle;
    public RawImage todoIcon;
    public RawImage awardIcon;
    public Button driveButton;
    public Button learnWhoIsTheKing;
    public bool isDrivedCar = false;
    public bool isRobotFound = false;
    public bool isCarFound = false;

    public AudioSource audioSource;
    public List<AudioClip> infoAudios = new List<AudioClip>();

    void Start()
    {
        audioSource = GetComponent<AudioSource>();
    }

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    public void EnterRak()
    {
        StartCoroutine(EngieEnterCarText());
        driveButton.gameObject.SetActive(false);
    }

    public void ChangeScene()
    {
        SceneManager.LoadSceneAsync(2);
    }

    IEnumerator EngieEnterCarText()
    {
        todoIcon.gameObject.SetActive(false);
        awardIcon.gameObject.SetActive(true);
        infoBox.text = "You Entered Rak.";

        subtitle.text = "";

        awardIcon.gameObject.SetActive(false);
        todoIcon.gameObject.SetActive(true);
        infoBox.text = "Drive Rak! At least 5 Seconds. Your timer started.";
        yield return new WaitForSeconds(5f); // Her harften sonra biraz bekle

        isDrivedCar = true;
    }
}
