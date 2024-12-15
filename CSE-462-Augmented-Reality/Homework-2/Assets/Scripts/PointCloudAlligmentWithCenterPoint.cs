using System;
using System.Collections.Generic;
using UnityEngine;
using Accord.Math;
using TMPro;

public class PointCloudAllignmentWithCenterPoint : MonoBehaviour
{
    private List<UnityEngine.Vector3> generatedPointsP1;
    private List<UnityEngine.Vector3> generatedPointsP2;
    private List<UnityEngine.Vector3> transformedPoints;
    private List<GameObject> lines;

    public TMP_Text rotationMatrixText;
    public TMP_Text translationMatrixText;
    public TMP_Text bestIterationCount;
    public TMP_Text bestErrorRate;

    public GameObject generatedPointPrefab;
    public GameObject generatedPointParentPrefab;
    private void Start()
    {
        lines = new List<GameObject>();
        generatedPointsP2 = GameManagerReference.Instance.FirstPoints;
        generatedPointsP1 = GameManagerReference.Instance.SecondPoints;
    }

    public void AlignPointClouds()
    {

        if (generatedPointsP1.Count < 3 || generatedPointsP2.Count < 3)
        {
            rotationMatrixText.text = "Point counts less than 3!";
            return;
        }

        (UnityEngine.Matrix4x4 bestRotationMatrix, UnityEngine.Vector3 bestTranslationVector) = AlignUsingCentroids(generatedPointsP1, generatedPointsP2);

        rotationMatrixText.text = "Rotation Matrix:\n" + MatrixToString(bestRotationMatrix);
        translationMatrixText.text = "Translation Vector:\n" + bestTranslationVector;
        bestIterationCount.text = "";

        transformedPoints = ApplyTransformation(generatedPointsP2, bestRotationMatrix, bestTranslationVector);

        float bestError = CalculateAlignmentError(generatedPointsP1, transformedPoints);
        bestErrorRate.text = "Best Error Rate: " + bestError;
        GameManagerReference.Instance.GeneratedPoints = transformedPoints;
        CreatePoints(transformedPoints);
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
        foreach (var line in GameObject.FindGameObjectsWithTag("line"))
        {
            Destroy(line);
        }
    }

    private (UnityEngine.Matrix4x4, UnityEngine.Vector3) AlignUsingCentroids(List<UnityEngine.Vector3> pointsP1, List<UnityEngine.Vector3> pointsP2)
    {
        UnityEngine.Vector3 centroidP1 = GetCentroid(pointsP1);
        UnityEngine.Vector3 centroidP2 = GetCentroid(pointsP2);

        List<UnityEngine.Vector3> centeredP1 = CenterPoints(pointsP1, centroidP1);
        List<UnityEngine.Vector3> centeredP2 = CenterPoints(pointsP2, centroidP2);

        UnityEngine.Matrix4x4 rotationMatrix = CalculateRotationMatrix(centeredP1, centeredP2);
        UnityEngine.Vector3 translation = centroidP1 - rotationMatrix.MultiplyPoint3x4(centroidP2);

        return (rotationMatrix, translation);
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

    private List<UnityEngine.Vector3> CenterPoints(List<UnityEngine.Vector3> points, UnityEngine.Vector3 centroid)
    {
        List<UnityEngine.Vector3> centeredPoints = new List<UnityEngine.Vector3>();
        foreach (var point in points)
        {
            centeredPoints.Add(point - centroid);
        }
        return centeredPoints;
    }
    private UnityEngine.Matrix4x4 CalculateRotationMatrix(List<UnityEngine.Vector3> centeredP1, List<UnityEngine.Vector3> centeredP2)
    {
        double[,] covMatrix = ComputeCovarianceMatrix(centeredP1, centeredP2);

        var svd = new Accord.Math.Decompositions.SingularValueDecomposition(covMatrix, true, true);

        double[,] rotationMatrix = Accord.Math.Matrix.Dot(svd.LeftSingularVectors, svd.RightSingularVectors.Transpose());

        return ToUnityMatrix(rotationMatrix);
    }

    private UnityEngine.Matrix4x4 ToUnityMatrix(double[,] matrix)
    {
        UnityEngine.Matrix4x4 unityMatrix = UnityEngine.Matrix4x4.identity;

        unityMatrix.m00 = (float)matrix[0, 0];
        unityMatrix.m01 = (float)matrix[0, 1];
        unityMatrix.m02 = (float)matrix[0, 2];

        unityMatrix.m10 = (float)matrix[1, 0];
        unityMatrix.m11 = (float)matrix[1, 1];
        unityMatrix.m12 = (float)matrix[1, 2];

        unityMatrix.m20 = (float)matrix[2, 0];
        unityMatrix.m21 = (float)matrix[2, 1];
        unityMatrix.m22 = (float)matrix[2, 2];

        return unityMatrix;
    }
    private double[,] ComputeCovarianceMatrix(List<UnityEngine.Vector3> centeredP1, List<UnityEngine.Vector3> centeredP2)
    {
        double[,] covMatrix = new double[3, 3];

        for (int i = 0; i < centeredP1.Count && i < centeredP2.Count; i++)
        {
            UnityEngine.Vector3 p1 = centeredP1[i];
            UnityEngine.Vector3 p2 = centeredP2[i];

            covMatrix[0, 0] += p1.x * p2.x;
            covMatrix[0, 1] += p1.x * p2.y;
            covMatrix[0, 2] += p1.x * p2.z;

            covMatrix[1, 0] += p1.y * p2.x;
            covMatrix[1, 1] += p1.y * p2.y;
            covMatrix[1, 2] += p1.y * p2.z;

            covMatrix[2, 0] += p1.z * p2.x;
            covMatrix[2, 1] += p1.z * p2.y;
            covMatrix[2, 2] += p1.z * p2.z;
        }

        return covMatrix;
    }
    private List<UnityEngine.Vector3> ApplyTransformation(List<UnityEngine.Vector3> points, UnityEngine.Matrix4x4 rotationMatrix, UnityEngine.Vector3 translation)
    {
        List<UnityEngine.Vector3> transformed = new List<UnityEngine.Vector3>();
        foreach (var point in points)
        {
            transformed.Add(rotationMatrix.MultiplyPoint3x4(point) + translation);
        }
        return transformed;
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

    private string MatrixToString(UnityEngine.Matrix4x4 matrix)
    {
        return $"{matrix.m00} {matrix.m01} {matrix.m02} {matrix.m03}\n" +
               $"{matrix.m10} {matrix.m11} {matrix.m12} {matrix.m13}\n" +
               $"{matrix.m20} {matrix.m21} {matrix.m22} {matrix.m23}\n" +
               $"{matrix.m30} {matrix.m31} {matrix.m32} {matrix.m33}";
    }
}
