using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManagerReference : MonoBehaviour
{
    public static GameManagerReference Instance { get; private set; }

    public List<UnityEngine.Vector3> FirstPoints;
    public List<UnityEngine.Vector3> SecondPoints;
    public List<UnityEngine.Vector3> GeneratedPoints;

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
}
