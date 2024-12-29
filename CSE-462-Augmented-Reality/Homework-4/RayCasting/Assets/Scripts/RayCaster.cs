using UnityEngine;
using UnityEditor;
using UnityEngine.UI;
using System.IO;
using System.Collections.Generic;

public class RayCaster : MonoBehaviour
{
    [Header("Camera Settings")]
    public Camera cam;
    public int imageWidth = 640;
    public int imageHeight = 480;
    public float fov = 100;
    public float aspectRatio = 1.33f;

    [Header("Rendering Settings")]
    public bool blackhole = true;
    public RawImage image;
    public float blackHoleInfluence = 0.1f;
    public float stepSize = 0.5f;
    public int maxStepsPerRay = 50;
    public int pixelsPerFrame = 1000;

    [Header("Debug Settings")]
    public bool showDebugGizmos = true;
    public float debugRayLength = 10f;
    public float blackHoleDebugRadius = 5f;

    // Private fields
    private List<GameObject> objects;
    private Light[] lightSources;
    private bool[,] allHits;
    private Ray[,] allRays;
    private Texture2D texture;
    private GameObject myBlackHole;
    private float startTime;
    private bool processingComplete = false;
    private float timeoutDuration = 10f;

    private void Awake()
    {
        InitializeArrays();
    }

    void Start()
    {
        InitializeComponents();
        InitializeRays();
        startTime = Time.time;

        int checkPointTri = 0;
        foreach (MeshFilter mf in FindObjectsOfType(typeof(MeshFilter)))    checkPointTri += mf.mesh.triangles.Length;
        Debug.Log("Total tri : "+checkPointTri);

        if (!blackhole)
        {
            RenderWithoutBlackHole();
        }
    }

    void FixedUpdate()
    {
        if (blackhole && !processingComplete)
        {
            CheckTimeout();
            ProcessFramePixels();
        }
    }

    private void InitializeArrays()
    {
        allHits = new bool[imageWidth, imageHeight];
        allRays = new Ray[imageWidth, imageHeight];
        texture = new Texture2D(imageWidth, imageHeight);
    }

    private void InitializeComponents()
    {
        // Find all object with thags / rend. ls. blackHOLE
        objects = new List<GameObject>(GameObject.FindGameObjectsWithTag("Renderable"));
        if (objects.Count == 0) Debug.LogError("No objects found with 'Renderable' tag!");

        GameObject[] lightObjects = GameObject.FindGameObjectsWithTag("LightSource");
        lightSources = new Light[lightObjects.Length];
        for (int i = 0; i < lightObjects.Length; i++)
        {
            lightSources[i] = lightObjects[i].GetComponent<Light>();
        }
        if (lightSources.Length == 0) Debug.LogError("No lights found with 'LightSource' tag!");

        myBlackHole = GameObject.FindGameObjectWithTag("blackHole");
        if (myBlackHole == null && blackhole) Debug.LogError("No black hole found with 'blackHole' tag!");

        image.texture = texture;
    }

    private void InitializeRays()
    {
        for (int i = 0; i < imageWidth; i++)
        {
            for (int j = 0; j < imageHeight; j++)
            {
                allRays[i, j] = cam.ViewportPointToRay(new Vector3((float)i / imageWidth, (float)j / imageHeight, 0));
                allHits[i, j] = false;
            }
        }
    }

    private void CheckTimeout()
    {
        if (Time.time - startTime > timeoutDuration)
        {
            Debug.Log("Rendering timed out - saving partial progress");
            SaveImage("_hole_notime");
            processingComplete = true;
        }
    }

    private void ProcessFramePixels()
    {
        int pixelsProcessed = 0;
        bool allComplete = true;

        for (int x = 0; x < imageWidth && pixelsProcessed < pixelsPerFrame; x++)
        {
            for (int y = 0; y < imageHeight && pixelsProcessed < pixelsPerFrame; y++)
            {
                if (!allHits[x, y])
                {
                    ProcessPixel(x, y);
                    pixelsProcessed++;
                    allComplete = false;
                }
            }
        }

        texture.Apply();

        if (allComplete && !processingComplete)
        {
            Debug.Log("Rendering completed successfully");
            SaveImage("_black_hole");
            processingComplete = true;
        }
    }

    private void ProcessPixel(int x, int y)
    {
        Vector3 currentPos = allRays[x, y].origin;
        Vector3 currentDir = allRays[x, y].direction;

        for (int step = 0; step < maxStepsPerRay; step++)
        {
            Ray stepRay = new Ray(currentPos, currentDir);
            RaycastHit hit;

            if (Physics.Raycast(stepRay, out hit, stepSize))
            {
                SetPixelColor(x, y, hit);
                allHits[x, y] = true;
                return;
            }

            // get ray position and direction based on black hole influence -- every update works until my var true => false
            currentPos += currentDir * stepSize;
            Vector3 toBlackHole = (myBlackHole.transform.position - currentPos).normalized;
            float distanceToBlackHole = Vector3.Distance(currentPos, myBlackHole.transform.position);
            float influenceFactor = blackHoleInfluence / (distanceToBlackHole * distanceToBlackHole);
            currentDir = Vector3.Lerp(currentDir, toBlackHole, influenceFactor).normalized;
        }

        // Mark as processed even if no hit found
        allHits[x, y] = true;
        texture.SetPixel(x, y, Color.black);
    }

    private void RenderWithoutBlackHole()
    {
        for (int i = 0; i < imageWidth; i++)
        {
            for (int j = 0; j < imageHeight; j++)
            {
                Ray ray = cam.ViewportPointToRay(new Vector3((float)i / imageWidth, (float)j / imageHeight, 0));
                RaycastHit hit;

                if (Physics.Raycast(ray, out hit, Mathf.Infinity))
                {
                    SetPixelColor(i, j, hit);
                }
                else
                {
                    texture.SetPixel(i, j, Color.black);
                }
            }
        }

        texture.Apply();
        SaveImage("_no_hole");
    }

    private void SetPixelColor(int x, int y, RaycastHit hit)
    {
        int hitCount = 0;
        Color baseColor = hit.collider.GetComponent<Renderer>().material.color;

        foreach (var light in lightSources)
        {
            hitCount += CalculateLightContribution(light, hit.point);
        }

        Color finalColor = new Color(
            hitCount * (baseColor.r / lightSources.Length),
            hitCount * (baseColor.g / lightSources.Length),
            hitCount * (baseColor.b / lightSources.Length)
        );

        texture.SetPixel(x, y, finalColor);
    }

    private int CalculateLightContribution(Light light, Vector3 hitPoint)
    {
        Vector3 dirToLight = (hitPoint - light.transform.position).normalized;
        Ray lightRay = new Ray(light.transform.position, dirToLight);
        RaycastHit lightHit;

        if (Physics.Raycast(lightRay, out lightHit, Mathf.Infinity))
        {
            if (Vector3.Distance(lightHit.point, hitPoint) <= 0.001f)
            {
                return 1;
            }
        }
        return 0;
    }

    private void SaveImage(string subpath)
    {
        string directory = Application.dataPath + "/BuildedImages";
        if (!Directory.Exists(directory))
        {
            Directory.CreateDirectory(directory);
        }

        string filePath = directory + "/image" + subpath + ".png";
        File.WriteAllBytes(filePath, texture.EncodeToPNG());
        AssetDatabase.Refresh();
        Debug.Log($"Image saved to: {filePath}");
    }

    void OnDrawGizmos()
    {
        if (!showDebugGizmos || !Application.isPlaying) return;

        // Draw camera frustum
        DrawCameraFrustum();

        // Draw black hole influence
        DrawBlackHoleInfluence();

        // Draw sample rays
        DrawSampleRays();
    }

    private void DrawCameraFrustum()
    {
        Gizmos.color = Color.yellow;
        Vector3[] frustumCorners = new Vector3[4];
        cam.CalculateFrustumCorners(
            new Rect(0, 0, 1, 1),
            cam.farClipPlane,
            Camera.MonoOrStereoscopicEye.Mono,
            frustumCorners
        );

        for (int i = 0; i < 4; i++)
        {
            Vector3 worldSpaceCorner = cam.transform.TransformPoint(frustumCorners[i]);
            Gizmos.DrawLine(cam.transform.position, worldSpaceCorner);
        }
    }

    private void DrawBlackHoleInfluence()
    {
        if (myBlackHole != null)
        {
            Gizmos.color = Color.red;
            Gizmos.DrawWireSphere(myBlackHole.transform.position, blackHoleDebugRadius);
        }
    }

    private void DrawSampleRays()
    {
        if (allRays == null) return;

        Gizmos.color = Color.cyan;
        for (int x = 0; x < imageWidth; x += 20)
        {
            for (int y = 0; y < imageHeight; y += 20)
            {
                if (!allHits[x, y])
                {
                    Gizmos.DrawRay(allRays[x, y].origin, allRays[x, y].direction * debugRayLength);
                }
            }
        }
    }
}