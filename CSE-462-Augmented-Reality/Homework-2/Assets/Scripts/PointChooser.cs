using System.Collections;
using System.Collections.Generic;
using System.IO;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class PointChooser : MonoBehaviour
{
    public GameObject firstPointPrefab;
    public GameObject firstPointParentPrefab;

    public GameObject secondPointPrefab;
    public GameObject secondPointParentPrefab;

    public TMP_Text firstPointText;
    public TMP_Text secondPointText;

    public Button button1;
    public Button button2;
    public Button button3;
    public Button button4;
    public Button button5;
    public Button button6;

    public Button button1_1;
    public Button button2_2;
    public Button button3_3;
    public Button button4_4;
    public Button button5_5;
    public Button button6_6;

    public TMP_Text rotationMatrixText;
    public TMP_Text translationMatrixText;
    public TMP_Text bestIterationCount;
    public TMP_Text bestErrorRate;

    public TMP_Text firstPointsText;
    public TMP_Text secondPointsText;

    private GameObject[] firstPointFileSelector;
    private GameObject[] secondPointFileSelector;
    private GameObject[] userUiElements;

    private List<GameObject> secondPoints;
    private bool isSecondPointListActive = false;
    void Start()
    {
        secondPoints = new List<GameObject>();
        firstPointFileSelector = GameObject.FindGameObjectsWithTag("firstPointList");
        secondPointFileSelector = GameObject.FindGameObjectsWithTag("secondPointList");
        userUiElements = GameObject.FindGameObjectsWithTag("userButtons");

        foreach (GameObject point in firstPointFileSelector)
        {
            point.SetActive(false);
        }

        foreach (GameObject point in secondPointFileSelector)
        {
            point.SetActive(false);
        }

        button1.onClick.AddListener(() => FirstButtonClicked("1"));
        button2.onClick.AddListener(() => FirstButtonClicked("2"));
        button3.onClick.AddListener(() => FirstButtonClicked("3"));
        button4.onClick.AddListener(() => FirstButtonClicked("4"));
        button5.onClick.AddListener(() => FirstButtonClicked("5"));
        button6.onClick.AddListener(() => FirstButtonClicked("6"));
        button1_1.onClick.AddListener(() => SecondButtonClicked("1_1"));
        button2_2.onClick.AddListener(() => SecondButtonClicked("2_1"));
        button3_3.onClick.AddListener(() => SecondButtonClicked("3_1"));
        button4_4.onClick.AddListener(() => SecondButtonClicked("4_1"));
        button5_5.onClick.AddListener(() => SecondButtonClicked("5_1"));
        button6_6.onClick.AddListener(() => SecondButtonClicked("6_1"));
    }

    void Update()
    {
        
    }

    public void ChooseFirstFile(bool opt = false)
    {
        foreach (GameObject point in userUiElements)
        {
            point.SetActive(opt);
        }
        if (opt == true)
        {
            button1.gameObject.SetActive(false);
            button2.gameObject.SetActive(false);
            button3.gameObject.SetActive(false);
            button4.gameObject.SetActive(false);
            button5.gameObject.SetActive(false);
            button6.gameObject.SetActive(false);
        }
        else
        {
            button1.gameObject.SetActive(true);
            button2.gameObject.SetActive(true);
            button3.gameObject.SetActive(true);
            button4.gameObject.SetActive(true);
            button5.gameObject.SetActive(true);
            button6.gameObject.SetActive(true);
        }
    }

    public void ChooseSecondFile(bool opt = false)
    {
        foreach (GameObject point in userUiElements)
        {
            point.SetActive(opt);
        }
        if(opt == true)
        {
            button1_1.gameObject.SetActive(false);
            button2_2.gameObject.SetActive(false);
            button3_3.gameObject.SetActive(false);
            button4_4.gameObject.SetActive(false);
            button5_5.gameObject.SetActive(false);
            button6_6.gameObject.SetActive(false);
        }
        else
        {
            button1_1.gameObject.SetActive(true);
            button2_2.gameObject.SetActive(true);
            button3_3.gameObject.SetActive(true);
            button4_4.gameObject.SetActive(true);
            button5_5.gameObject.SetActive(true);
            button6_6.gameObject.SetActive(true);
        }
    }

    public void CreatePointsByUser()
    {
        RemoveAllPoints();
        if(!CreatePoints(firstPointText.text))
        {
            Debug.Assert(false, "Ýlk noktalar oluþturulamadý.");
            return;
        }
        if (!CreatePoints(secondPointText.text, true))
        {
            Debug.Assert(false, "Ýkinci noktalar oluþturulamadý.");
            return;
        }
    }

    public void RemoveAllPoints()
    {
        GameObject[] points = GameObject.FindGameObjectsWithTag("point");
        bool flag = false;
        foreach (var point in points)
        {
            Destroy(point);
            flag = true;
        }
        if(flag)
        {
            GameManagerReference.Instance.FirstPoints.Clear();
            GameManagerReference.Instance.SecondPoints.Clear();
        }
        flag = false;
        points = GameObject.FindGameObjectsWithTag("generatedPoint");
        foreach (var point in points)
        {
            Destroy(point);
            flag = true;
        }
        if (flag)
        {
            GameManagerReference.Instance.GeneratedPoints.Clear();
        }
        points = GameObject.FindGameObjectsWithTag("line");
        foreach (var point in points)
        {
            Destroy(point);
        }

        rotationMatrixText.text = "";
        translationMatrixText.text = "";
        bestIterationCount.text = "";
        bestErrorRate.text = "";
    }

    private void FirstButtonClicked(string buttonNumber)
    {
        firstPointText.text = "point" + buttonNumber + ".txt";
        ChooseFirstFile(true);
    }

    private void SecondButtonClicked(string buttonNumber)
    {
        secondPointText.text = "point" + buttonNumber + ".txt";
        ChooseSecondFile(true);
    }

    private bool CreatePoints(string fileLocation, bool isSecond=false)
    {
        secondPoints.Clear();
        GameObject pointPrefab = firstPointPrefab;
        GameObject pointParentPrefab = firstPointParentPrefab;

        if (isSecond)
        {
            pointPrefab = secondPointPrefab;
            pointParentPrefab = secondPointParentPrefab;
        }
        List<List<float>> coordinates = GetPointsFromFile(fileLocation);
        if (coordinates == null)
        {
            return false;
        }
        foreach (var coordinate in coordinates)
        {
            if (coordinate.Count == 3)
            {
                
                UnityEngine.Vector3 pointPosition = new UnityEngine.Vector3(coordinate[0], coordinate[1], coordinate[2]);

                GameObject point = Instantiate(pointPrefab, pointPosition, Quaternion.identity);

                point.transform.SetParent(pointParentPrefab.transform, false);

                if (isSecond)
                {
                    GameManagerReference.Instance.SecondPoints.Add(pointPosition);
                    secondPoints.Add(point);
                }
                
                else
                    GameManagerReference.Instance.FirstPoints.Add(pointPosition);
                
            }
        }

        secondPointsText.text = "Second points:\n";
        foreach(var point in GameManagerReference.Instance.SecondPoints)
            secondPointsText.text += point.ToString() + "\n";


        firstPointsText.text = "First points:\n";
        foreach (var point in GameManagerReference.Instance.FirstPoints)
            firstPointsText.text += point.ToString() + "\n";
        
        return true;
    }

    public void DisapearSecondPointList()
    {
        foreach (var point in secondPoints)
        {
            if(point != null)
            {
                point.SetActive(isSecondPointListActive);
            }
        }
        isSecondPointListActive = !isSecondPointListActive;
    }

    private List<List<float>> GetPointsFromFile(string fileLocation)
    {
        List<List<float>> coordinates = new List<List<float>>();
        string path = Path.Combine(Application.dataPath, "PointLocations", fileLocation);

        if (File.Exists(path))
        {
            string[] lines = File.ReadAllLines(path);

            int numberOfPoints = int.Parse(lines[0]);

            for (int i = 1; i <= numberOfPoints; i++)
            {
                string[] lineParts = lines[i].Split(' ');

                if (lineParts.Length == 3)
                {
                    List<float> point = new List<float>
                    {
                        float.Parse(lineParts[0]), // x
                        float.Parse(lineParts[1]), // y
                        float.Parse(lineParts[2])  // z
                    };

                    coordinates.Add(point);
                }
            }

            if (coordinates.Count != numberOfPoints)
            {
                Debug.LogError("Dosya içeriði hatalý: " + path);
                return null;
            }
        }
        else
        {
            Debug.LogError("Dosya bulunamadý: " + path);
            return null;
        }

        return coordinates;
    }
    
}
