using UnityEngine;
using UnityEngine.UI;
using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.LinearAlgebra.Double;
using System.Collections.Generic;
using TMPro;
using System.IO;
using System;

public class FixedHomographyCalculator : MonoBehaviour
{
    public TMP_Text infoText;
    public TMP_Text currentImageText;
    public TMP_Text homographyMatrixText;
    public GameObject imageGameObject;

    [SerializeField]
    private GameObject teapotPrefab;

    [SerializeField]
    private float teapotSize = 0.15f;

    [SerializeField]
    private Camera mainCamera;

    // Add material references
    [SerializeField]
    private Material markerMaterial; // Assign a material in inspector for markers
    [SerializeField]
    private Material lineMaterial;   // Assign a material in inspector for lines

    private List<Vector2> selectedPoints = new List<Vector2>();

    private List<Vector2[]> worldPointList = new List<Vector2[]>()
    {
        new Vector2[5]
        {
            new Vector2(36.8f, 13.4f),  // USB bottom left
            new Vector2(38.3f, 12.7f),  // USB bottom right
            new Vector2(39.2f, 14.7f),  // USB top right
            new Vector2(37.7f, 15.4f),  // USB top left
            new Vector2(29.8f, 17.9f)   // Blue endpoint
        },
        new Vector2[5]
        {
            new Vector2(49.2f, 18.4f),  
            new Vector2(51.4f, 17.8f),  
            new Vector2(51.5f, 19.8f),  
            new Vector2(49.4f, 20.4f),  
            new Vector2(41.2f, 22.6f)  
        },
        new Vector2[5]
        {
            new Vector2(52.1f, 16f),
            new Vector2(53.5f, 16.3f),
            new Vector2(52.6f, 17.9f),
            new Vector2(51f, 17.9f),
            new Vector2(43.2f, 18.0f)
        },
        new Vector2[5]
        {
            new Vector2(52.7f, 13.6f),
            new Vector2(54f, 14.3f),
            new Vector2(52.6f, 16.1f),
            new Vector2(51.3f, 15.6f),
            new Vector2(42.5f, 14.1f)
        },
        new Vector2[5]
        {
            new Vector2(42.9f, 10.1f),
            new Vector2(45f, 10.7f),
            new Vector2(44.2f, 12.6f),
            new Vector2(42.3f, 12.8f),
            new Vector2(33.6f, 12.9f)
        },//20
        new Vector2[5]
        {
            new Vector2(38.8f, 8.3f),
            new Vector2(40.7f, 8.7f),
            new Vector2(40f, 11f),
            new Vector2(38.4f, 10.8f),
            new Vector2(30.3f, 11.7f),
        },
        new Vector2[5]
        {
            new Vector2(29.9f, 7.9f),
            new Vector2(31.5f, 8.3f),
            new Vector2(31.5f, 10.5f),
            new Vector2(29.9f, 10.6f),
            new Vector2(22.1f, 12.2f),
        },//22
        new Vector2[5]
        {
            new Vector2(25.3f, 8.7f),
            new Vector2(26.9f, 8.5f),
            new Vector2(27.2f, 10.1f),
            new Vector2(25.8f, 10.3f),
            new Vector2(18.2f, 12.5f),
        },//23
        new Vector2[5]
        {
            new Vector2(38.1f, 14.5f),
            new Vector2(39.8f, 14.7f),
            new Vector2(39.9f, 16.6f),
            new Vector2(38.1f, 16.8f),
            new Vector2(30.4f, 18.2f),
        },//24
        new Vector2[5]
        {
            new Vector2(53f, 22.8f),
            new Vector2(54.9f, 22.7f),
            new Vector2(55.3f, 24.6f),
            new Vector2(53.4f, 24.6f),
            new Vector2(44.7f, 26f),
        },//25
        new Vector2[5]
        {
            new Vector2(30f, 2.4f),
            new Vector2(31.7f, 2.5f),
            new Vector2(31.1f, 4.8f),
            new Vector2(29.6f, 4.6f),
            new Vector2(21.7f, 5.2f),
        },//26
        new Vector2[5]
        {
            new Vector2(26.2f, 8.5f),
            new Vector2(27.6f, 9.5f),
            new Vector2(26.4f, 11.3f),
            new Vector2(24.9f, 10.6f),
            new Vector2(17.2f, 8.1f),
        },//27
        new Vector2[5]
        {
            new Vector2(21.5f, 7f),
            new Vector2(22.5f, 8.4f),
            new Vector2(20.7f, 9.8f),
            new Vector2(19.6f, 9.1f),
            new Vector2(12.5f, 4.1f),
        },//28
        new Vector2[5]
        {
            new Vector2(19.8f, 13.1f),
            new Vector2(21.7f, 13f),
            new Vector2(21.9f, 15f),
            new Vector2(20.3f, 15.2f),
            new Vector2(12.3f, 17.1f),
        },//29
        new Vector2[5]
        {
            new Vector2(20.2f, 9.9f),
            new Vector2(21.9f, 9.6f),
            new Vector2(22.3f, 11.4f),
            new Vector2(20.8f, 11.7f),
            new Vector2(13.6f, 13.7f),
        },//30
        new Vector2[5]
        {
            new Vector2(29.6f, 10.4f),
            new Vector2(31.4f, 10.5f),
            new Vector2(31.4f, 13f),
            new Vector2(29.4f, 13.2f),
            new Vector2(20.9f, 14.2f),
        },//31
        new Vector2[5]
        {
            new Vector2(30.4f, 11.9f),
            new Vector2(31.8f, 12.8f),
            new Vector2(30.6f, 14.8f),
            new Vector2(29.2f, 14.2f),
            new Vector2(20.8f, 12.2f),
        },//32
        new Vector2[5]
        {
            new Vector2(42.4f, 16.3f),
            new Vector2(44.2f, 16.7f),
            new Vector2(42.6f, 18.7f),
            new Vector2(41.2f, 18.2f),
            new Vector2(32.6f, 16.4f),
        },//33
        new Vector2[5]
        {
            new Vector2(31.4f, 10.4f),
            new Vector2(33.3f, 10.8f),
            new Vector2(32.4f, 12.9f),
            new Vector2(30.6f, 12.6f),
            new Vector2(22.4f, 11.3f),
        },//34
    };

    private List<Vector2> teaPotRealWorldLocations = new List<Vector2>
    {
        new Vector2(38.0f, 14.3f),
        new Vector2(50.3f, 19.4f),
        new Vector2(52.3f, 17.1f),
        new Vector2(52.6f, 15f),
        new Vector2(43.5f, 11.5f),//20
        new Vector2(39.4f, 9.8f),//21
        new Vector2(30.6f, 9.8f),//22
        new Vector2(26.2f, 9.6f),//23
        new Vector2(39.3f, 15.7f),//24
        new Vector2(54.2f, 23.9f),//25
        new Vector2(30.5f, 3.7f),//26
        new Vector2(26.1f, 10f),//27
        new Vector2(21.1f, 8.7f),//28
        new Vector2(20.9f, 14f),//29
        new Vector2(21.1f, 10.8f),//30
        new Vector2(30.2f, 12.1f),//31
        new Vector2(30.4f, 13.6f),//32
        new Vector2(42.5f, 17.4f),//33
        new Vector2(31.8f, 11.8f),//34
    };

    private List<string> materialPaths = new List<string>
    {
        "Images/Materials/IMG_5315",
        "Images/Materials/IMG_5316",
        "Images/Materials/IMG_5318",
        "Images/Materials/IMG_5319",
        "Images/Materials/IMG_5320",
        "Images/Materials/IMG_5321",
        "Images/Materials/IMG_5322",
        "Images/Materials/IMG_5323",
        "Images/Materials/IMG_5324",
        "Images/Materials/IMG_5325",
        "Images/Materials/IMG_5326",
        "Images/Materials/IMG_5327",
        "Images/Materials/IMG_5328",
        "Images/Materials/IMG_5329",
        "Images/Materials/IMG_5330",
        "Images/Materials/IMG_5331",
        "Images/Materials/IMG_5332",
        "Images/Materials/IMG_5333",
        "Images/Materials/IMG_5334",
    };

    private bool isCollectingPoints = false;
    private Matrix<double> homographyMatrix;
    private int selectedImageIndex = 0;
    
    void Start()
    {
        // Create default materials if not assigned
        if (markerMaterial == null)
        {
            markerMaterial = new Material(Shader.Find("Standard"));
            markerMaterial.color = Color.red;
        }

        if (lineMaterial == null)
        {
            lineMaterial = new Material(Shader.Find("Standard"));
            lineMaterial.color = Color.green;
        }

        StartPointCollection();
    }

    private void Update()
    {
        if (isCollectingPoints && Input.GetMouseButtonDown(0))
        {
            // Convert screen point to viewport point
            Vector2 viewportPoint = mainCamera.ScreenToViewportPoint(Input.mousePosition);

            // Store the point
            selectedPoints.Add(viewportPoint);
            Debug.Log($"Point {selectedPoints.Count} selected at viewport: {viewportPoint}");

            // Visual feedback
            CreatePointMarker(Input.mousePosition);

            if (selectedPoints.Count == 5)
            {
                infoText.text = "Points collected. Calculating homography... You can click the show teapot button...";
                isCollectingPoints = false;
                CalculateHomography();
            }
        }
    }

    private void CreatePointMarker(Vector3 screenPosition)
    {
        Vector3 worldPos = mainCamera.ScreenToWorldPoint(new Vector3(screenPosition.x, screenPosition.y, 10f));
        GameObject marker = GameObject.CreatePrimitive(PrimitiveType.Sphere);
        marker.tag = "Marker";
        marker.transform.position = worldPos;
        marker.transform.localScale = Vector3.one * 0.1f;

        // Use the assigned or default material
        marker.GetComponent<Renderer>().material = new Material(markerMaterial);
        Destroy(marker.GetComponent<Collider>());
    }

    private void CalculateHomography()
    {
        // Create matrices for SVD calculation
        var A = Matrix<double>.Build.Dense(2 * worldPointList[selectedImageIndex].Length, 9);

        for (int i = 0; i < worldPointList[selectedImageIndex].Length; i++)
        {
            // Use normalized viewport coordinates
            double x = selectedPoints[i].x;
            double y = selectedPoints[i].y;

            // Scale world points to match Unity scale (optional: adjust scaling factor)
            double X = worldPointList[selectedImageIndex][i].x * 0.1f; // Scale factor for better Unity world space fit
            double Y = worldPointList[selectedImageIndex][i].y * 0.1f;

            // Fill A matrix
            A[2 * i, 0] = x;
            A[2 * i, 1] = y;
            A[2 * i, 2] = 1;
            A[2 * i, 3] = 0;
            A[2 * i, 4] = 0;
            A[2 * i, 5] = 0;
            A[2 * i, 6] = -x * X;
            A[2 * i, 7] = -y * X;
            A[2 * i, 8] = -X;

            A[2 * i + 1, 0] = 0;
            A[2 * i + 1, 1] = 0;
            A[2 * i + 1, 2] = 0;
            A[2 * i + 1, 3] = x;
            A[2 * i + 1, 4] = y;
            A[2 * i + 1, 5] = 1;
            A[2 * i + 1, 6] = -x * Y;
            A[2 * i + 1, 7] = -y * Y;
            A[2 * i + 1, 8] = -Y;
        }

        // Compute SVD
        var svd = A.Svd(true);
        var V = svd.VT.Transpose();

        // Get the last column of V
        var h = V.Column(V.ColumnCount - 1);

        // Create homography matrix
        homographyMatrix = Matrix<double>.Build.Dense(3, 3);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                homographyMatrix[i, j] = h[i * 3 + j];
        homographyMatrixText.text = homographyMatrix.ToString();
        PlaceTeapot();
    }

    private void PlaceTeapotNew()
    {
        var scaledTeapotPos = new Vector2(teaPotRealWorldLocations[selectedImageIndex].x * 0.1f, teaPotRealWorldLocations[selectedImageIndex].y * 0.1f);

        var point = Vector<double>.Build.Dense(3);
        point[0] = scaledTeapotPos.x;
        point[1] = scaledTeapotPos.y;
        point[2] = 1;

        var transformed = homographyMatrix.Multiply(point);
        Vector2 viewportPoint = new Vector2(
            (float)(transformed[0] / transformed[2]),
            (float)(transformed[1] / transformed[2])
        );

        Vector3 worldPoint = mainCamera.ViewportToWorldPoint(
            new Vector3(viewportPoint.x, viewportPoint.y, 10f)
        );

        GameObject teapot = Instantiate(teapotPrefab, worldPoint, Quaternion.identity);
        teapot.transform.localScale = Vector3.one * teapotSize;

        // Rotasyon hesapla ve uygula
        var rotationColumn1 = new Vector3(
            (float)homographyMatrix[0, 0],
            (float)homographyMatrix[1, 0],
            (float)homographyMatrix[2, 0]
        );

        var rotationColumn2 = new Vector3(
            (float)homographyMatrix[0, 1],
            (float)homographyMatrix[1, 1],
            (float)homographyMatrix[2, 1]
        );

        rotationColumn1.Normalize();
        rotationColumn2.Normalize();

        var rotationColumn3 = Vector3.Cross(rotationColumn1, rotationColumn2);

        var rotationMatrix = new Matrix4x4();
        rotationMatrix.SetColumn(0, rotationColumn1);
        rotationMatrix.SetColumn(1, rotationColumn2);
        rotationMatrix.SetColumn(2, rotationColumn3);
        rotationMatrix.SetColumn(3, new Vector4(0, 0, 0, 1));

        Quaternion rotation = rotationMatrix.rotation;
        teapot.transform.rotation = rotation;

        // Wireframe box oluþtur
        CreateWireframeBox(teapot.transform);

        Debug.Log($"Teapot placed at world position: {worldPoint} with rotation: {rotation.eulerAngles}");
    }


    private void PlaceTeapot()
    {
        // Scale teapot position to match Unity scale
        var scaledTeapotPos = new Vector2(teaPotRealWorldLocations[selectedImageIndex].x * 0.1f, teaPotRealWorldLocations[selectedImageIndex].y * 0.1f);

        // Create homogeneous coordinates
        var point = Vector<double>.Build.Dense(3);
        point[0] = scaledTeapotPos.x;
        point[1] = scaledTeapotPos.y;
        point[2] = 1;

        // Apply homography
        var transformed = homographyMatrix.Solve(point);
        Vector2 viewportPoint = new Vector2(
            (float)(transformed[0] / transformed[2]),
            (float)(transformed[1] / transformed[2])
        );

        // Convert viewport point to world position
        Vector3 worldPoint = mainCamera.ViewportToWorldPoint(
            new Vector3(viewportPoint.x, viewportPoint.y, 10f)
        );

        // Create teapot
        GameObject teapot = Instantiate(teapotPrefab, worldPoint, Quaternion.identity);
        teapot.transform.localScale = Vector3.one * teapotSize;
        teapot.transform.tag = "Teapot";

        /*Vector3 newScale = teapot.transform.localScale;
        newScale.x = 7;
        newScale.y = 7; 
        newScale.z = -5; 
        teapot.transform.localScale = newScale;*/
        // Homografi matrisinden ölçek hesaplama
        var scaleX = (float)Math.Sqrt(
            Math.Pow(homographyMatrix[0, 0], 2) + Math.Pow(homographyMatrix[1, 0], 2)
        );
        var scaleY = (float)Math.Sqrt(
            Math.Pow(homographyMatrix[0, 1], 2) + Math.Pow(homographyMatrix[1, 1], 2)
        );

        teapot.transform.localScale = new Vector3(scaleX * 15f, scaleY * 15f, (scaleY * 15f + 5)*-1);


        // Rotasyon hesapla ve uygula
        var rotationColumn1 = new Vector3(
            (float)homographyMatrix[0, 0],
            (float)homographyMatrix[1, 0],
            (float)homographyMatrix[2, 0]
        );

        var rotationColumn2 = new Vector3(
            (float)homographyMatrix[0, 1],
            (float)homographyMatrix[1, 1],
            (float)homographyMatrix[2, 1]
        );

        rotationColumn1.Normalize();
        rotationColumn2.Normalize();

        var rotationColumn3 = Vector3.Cross(rotationColumn1, rotationColumn2);

        var rotationMatrix = new Matrix4x4();
        rotationMatrix.SetColumn(0, rotationColumn1);
        rotationMatrix.SetColumn(1, rotationColumn2);
        rotationMatrix.SetColumn(2, rotationColumn3);
        rotationMatrix.SetColumn(3, new Vector4(0, 0, 0, 1));

        Quaternion rotation = rotationMatrix.rotation;
        teapot.transform.rotation = rotation;
        // Create wireframe box
        CreateWireframeBox(teapot.transform);

        Debug.Log($"Teapot placed at world position: {worldPoint}");
    }

    private void CreateWireframeBox(Transform parent)
    {
        GameObject wireframeObj = new GameObject("WireframeBox");
        wireframeObj.transform.SetParent(parent);
        wireframeObj.transform.localPosition = Vector3.zero;

        LineRenderer lineRenderer = wireframeObj.AddComponent<LineRenderer>();
        lineRenderer.useWorldSpace = false;
        lineRenderer.material = new Material(lineMaterial);
        lineRenderer.startWidth = 0.005f;
        lineRenderer.endWidth = 0.005f;

        float halfSize = teapotSize / 2f;
        Vector3[] vertices = new Vector3[]
        {
            new Vector3(-halfSize, -halfSize, -halfSize),
            new Vector3(halfSize, -halfSize, -halfSize),
            new Vector3(halfSize, -halfSize, halfSize),
            new Vector3(-halfSize, -halfSize, halfSize),
            new Vector3(-halfSize, halfSize, -halfSize),
            new Vector3(halfSize, halfSize, -halfSize),
            new Vector3(halfSize, halfSize, halfSize),
            new Vector3(-halfSize, halfSize, halfSize)
        };

        Vector3[] positions = new Vector3[]
        {
            // Bottom
            vertices[0], vertices[1],
            vertices[1], vertices[2],
            vertices[2], vertices[3],
            vertices[3], vertices[0],
            // Top
            vertices[4], vertices[5],
            vertices[5], vertices[6],
            vertices[6], vertices[7],
            vertices[7], vertices[4],
            // Sides
            vertices[0], vertices[4],
            vertices[1], vertices[5],
            vertices[2], vertices[6],
            vertices[3], vertices[7]
        };

        lineRenderer.positionCount = positions.Length;
        lineRenderer.SetPositions(positions);
    }

    private void StartPointCollection()
    {
        infoText.text = "Click to select 5 points in order: USB (bottom-left, bottom-right, top-right, top-left), Blue endpoint";
        currentImageText.text = $"Current Image: {materialPaths[selectedImageIndex]}";
        MeshRenderer meshRenderer = imageGameObject.GetComponent<MeshRenderer>();

        if (meshRenderer != null)
        {
            // Yeni materyali Resources klasöründen yükle
            Material newMaterial = Resources.Load<Material>(materialPaths[selectedImageIndex]);

            if (newMaterial != null)
            {
                // MeshRenderer'a yeni materyali ata
                meshRenderer.material = newMaterial;
            }
            else
            {
                Debug.LogError("Yeni materyal bulunamadý! Dosya adýný ve yolu kontrol edin.");
            }
        }
        else
        {
            Debug.LogError("MeshRenderer bileþeni bulunamadý!");
        }
        selectedPoints.Clear();
        isCollectingPoints = true;
        Debug.Log("Click to select 5 points in order: USB (bottom-left, bottom-right, top-right, top-left), Blue endpoint");

    }

    public void ClearAll()
    {
        GameObject[] gameObjects = GameObject.FindGameObjectsWithTag("Teapot");
        if (gameObjects != null)
        {
            foreach (GameObject teapot in gameObjects)
            {
                Destroy(teapot);
            }
        }

        GameObject[] markers = GameObject.FindGameObjectsWithTag("Marker");
        if (markers != null)
        {
            foreach (GameObject marker in markers)
            {
                Destroy(marker);
            }
        }
        // Clear selected points
        selectedPoints.Clear();

        // Clear homography matrix
        homographyMatrix = null;

    }

    public void ResetHomography()
    {
        ClearAll();
        StartPointCollection();
    }

    public void NextImage()
    {
        selectedImageIndex = (selectedImageIndex + 1) % materialPaths.Count;
        ResetHomography();
    }

    public void PreviousImage()
    {
        selectedImageIndex = (selectedImageIndex - 1 + materialPaths.Count) % materialPaths.Count;
        ResetHomography();
    }
}