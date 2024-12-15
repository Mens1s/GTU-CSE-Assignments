using System;
using UnityEngine;
using System.Collections.Generic;
using Accord.Math.Decompositions;
using Accord.Math;
using TMPro;

public class PointCloudAlignmentSVD : MonoBehaviour
{
    private List<UnityEngine.Vector3> generatedPointsP1;
    private List<UnityEngine.Vector3> generatedPointsP2;
    private List<UnityEngine.Vector3> transformedPoints;
    private List<GameObject> lines;

    public GameObject generatedPointPrefab;
    public GameObject generatedPointParentPrefab;

    public TMP_Text rotationMatrixText;
    public TMP_Text translationMatrixText;
    public TMP_Text bestIterationCount;
    public TMP_Text bestErrorRate;

    private void Start()
    {
        lines = new List<GameObject>();
        generatedPointsP2 = GameManagerReference.Instance.FirstPoints;
        generatedPointsP1 = GameManagerReference.Instance.SecondPoints;
    }

    public void AlignPointClouds()
    {

        if(generatedPointsP1.Count < 3 || generatedPointsP2.Count < 3)
        {
            rotationMatrixText.text = "Point counts less than 3!";
            return;
        }
       
        (UnityEngine.Matrix4x4 bestRotationMatrix, UnityEngine.Vector3 bestTranslationVector, float bestError, int bestIteration) = FindBestTransformation();

        rotationMatrixText.text = "Rotation Matrix:\n" + MatrixToString(bestRotationMatrix);
        translationMatrixText.text = "Translation Vector:\n" + bestTranslationVector;
        bestIterationCount.text = "Best Iteration: " + bestIteration + "'th over 100 iteretion";
        bestErrorRate.text = "Best Error Rate: " + bestError;

        transformedPoints = ApplyTransformation(generatedPointsP2, bestRotationMatrix, bestTranslationVector);
        GameManagerReference.Instance.GeneratedPoints = transformedPoints;
        CreatePoints(transformedPoints);
    }

    public void DrawPointByUserInput()
    {
        removeLinesIfExists();
        transformedPoints = GameManagerReference.Instance.GeneratedPoints;

        if (transformedPoints == null)
        {
            rotationMatrixText.text = "You did not create points. First click find locs.";
            return;
        }
        if(transformedPoints.Count == 0)
        {
            rotationMatrixText.text = "You did not create points. First click find locs.";
            return;
        }
        DrawMatchingLines(generatedPointsP2, transformedPoints);
    }
    private void DrawMatchingLines(List<UnityEngine.Vector3> points1, List<UnityEngine.Vector3> points2)
    {
        if (points1.Count != points2.Count)
        {
            rotationMatrixText.text = "You did not create points. First click find locs.";
            return;
        }

        for (int i = 0; i < points1.Count; i++)
        {
            UnityEngine.Vector3 point1 = points1[i];
            UnityEngine.Vector3 point2 = points2[i];

            GameObject line = new GameObject("Line_" + i);
            LineRenderer lineRenderer = line.AddComponent<LineRenderer>();

            lineRenderer.material = new Material(Shader.Find("Sprites/Default"));
            lineRenderer.startColor = Color.white;
            lineRenderer.endColor = Color.white;
            lineRenderer.startWidth = 0.12f;
            lineRenderer.endWidth = 0.12f;

            lineRenderer.SetPosition(0, point1);
            lineRenderer.SetPosition(1, point2);

            lines.Add(line);
            line.tag = "line";

        }
    }

    private (UnityEngine.Matrix4x4, UnityEngine.Vector3, float, int) FindBestTransformation()
    {
        int iterations = 100;
        int bestIteration = 0;
        float bestError = float.MaxValue;
        UnityEngine.Matrix4x4 bestRotation = UnityEngine.Matrix4x4.identity;
        UnityEngine.Vector3 bestTranslation = UnityEngine.Vector3.zero;

        for (int i = 0; i < iterations; i++)
        {
            List<UnityEngine.Vector3> sampleP1 = GetRandomSample(generatedPointsP1, 3);
            List<UnityEngine.Vector3> sampleP2 = GetRandomSample(generatedPointsP2, 3);

            (UnityEngine.Matrix4x4 rotation, UnityEngine.Vector3 translation) = ComputeRigidTransformationWithSVD(sampleP1, sampleP2);

            float error = CalculateAlignmentError(generatedPointsP1, ApplyTransformation(generatedPointsP2, rotation, translation));

            if (error < bestError)
            {
                bestError = error;
                bestRotation = rotation;
                bestTranslation = translation;
                bestIteration = i;
            }
        }

        return (bestRotation, bestTranslation, bestError, bestIteration);
    }

    private float CalculateAlignmentError(List<UnityEngine.Vector3> P1, List<UnityEngine.Vector3> transformedP2)
    {
        float error = 0f;

        for (int i = 0; i < P1.Count && i < transformedP2.Count; i++)
        {
            error += (P1[i] - transformedP2[i]).sqrMagnitude;
        }

        return error / P1.Count;
    }

    private List<UnityEngine.Vector3> GetRandomSample(List<UnityEngine.Vector3> points, int count)
    {
        List<UnityEngine.Vector3> sample = new List<UnityEngine.Vector3>();
        System.Random random = new System.Random(Guid.NewGuid().GetHashCode());

        while (sample.Count < count)
        {
            int index = random.Next(points.Count);
            if (!sample.Contains(points[index]))
            {
                sample.Add(points[index]);
            }
        }

        return sample;
    }

    private void CreatePoints(List<UnityEngine.Vector3> transformedPoints)
    {
        removePointsIfExists();
        foreach (var pointLoc in transformedPoints)
        {
            GameObject point = Instantiate(generatedPointPrefab, pointLoc, Quaternion.identity);
            point.transform.SetParent(generatedPointParentPrefab.transform, false);
        }
    }

    private void removePointsIfExists()
    {
        GameObject[] points = GameObject.FindGameObjectsWithTag("generatedPoint");
        foreach (var point in points)
        {
            Destroy(point);
        }
        removeLinesIfExists();
    }

    private void removeLinesIfExists()
    {
        foreach (var line in lines)
        {
            Destroy(line);
        }
    }
    private (UnityEngine.Matrix4x4, UnityEngine.Vector3) ComputeRigidTransformationWithSVD(List<UnityEngine.Vector3> P1, List<UnityEngine.Vector3> P2)
    {
        UnityEngine.Vector3 centroidP1 = GetCentroid(P1);
        UnityEngine.Vector3 centroidP2 = GetCentroid(P2);

        double[,] centeredP1 = CenterPoints(P1, centroidP1);
        double[,] centeredP2 = CenterPoints(P2, centroidP2);

        var H = centeredP1.Transpose().Dot(centeredP2);
        var svd = new SingularValueDecomposition(H, true, true);
        var rotationMatrix = svd.LeftSingularVectors.Dot(svd.RightSingularVectors.Transpose());

        UnityEngine.Vector3 translation = centroidP1 - Multiply(rotationMatrix, centroidP2);
        return (ToMatrix4x4(rotationMatrix), translation);
    }

    private UnityEngine.Vector3 GetCentroid(List<UnityEngine.Vector3> points)
    {
        UnityEngine.Vector3 sum = UnityEngine.Vector3.zero;
        foreach (var point in points)
        {
            sum += point;
        }
        return sum / points.Count;
    }

    private double[,] CenterPoints(List<UnityEngine.Vector3> points, UnityEngine.Vector3 centroid)
    {
        double[,] centered = new double[points.Count, 3];
        for (int i = 0; i < points.Count; i++)
        {
            centered[i, 0] = points[i].x - centroid.x;
            centered[i, 1] = points[i].y - centroid.y;
            centered[i, 2] = points[i].z - centroid.z;
        }
        return centered;
    }

    private UnityEngine.Vector3 Multiply(double[,] matrix, UnityEngine.Vector3 vector)
    {
        return new UnityEngine.Vector3(
            (float)(matrix[0, 0] * vector.x + matrix[0, 1] * vector.y + matrix[0, 2] * vector.z),
            (float)(matrix[1, 0] * vector.x + matrix[1, 1] * vector.y + matrix[1, 2] * vector.z),
            (float)(matrix[2, 0] * vector.x + matrix[2, 1] * vector.y + matrix[2, 2] * vector.z)
        );
    }

    private UnityEngine.Matrix4x4 ToMatrix4x4(double[,] matrix)
    {
        UnityEngine.Matrix4x4 mat = UnityEngine.Matrix4x4.identity;
        mat.m00 = (float)matrix[0, 0];
        mat.m01 = (float)matrix[0, 1];
        mat.m02 = (float)matrix[0, 2];
        mat.m10 = (float)matrix[1, 0];
        mat.m11 = (float)matrix[1, 1];
        mat.m12 = (float)matrix[1, 2];
        mat.m20 = (float)matrix[2, 0];
        mat.m21 = (float)matrix[2, 1];
        mat.m22 = (float)matrix[2, 2];
        return mat;
    }

    private List<UnityEngine.Vector3> ApplyTransformation(List<UnityEngine.Vector3> points, UnityEngine.Matrix4x4 rotation, UnityEngine.Vector3 translation)
    {
        List<UnityEngine.Vector3> transformed = new List<UnityEngine.Vector3>();
        foreach (var point in points)
        {
            transformed.Add(rotation.MultiplyPoint3x4(point) + translation);
        }
        return transformed;
    }

    private string MatrixToString(UnityEngine.Matrix4x4 matrix)
    {
        return $"{matrix.m00} {matrix.m01} {matrix.m02} {matrix.m03}\n" +
               $"{matrix.m10} {matrix.m11} {matrix.m12} {matrix.m13}\n" +
               $"{matrix.m20} {matrix.m21} {matrix.m22} {matrix.m23}\n" +
               $"{matrix.m30} {matrix.m31} {matrix.m32} {matrix.m33}";
    }
}
