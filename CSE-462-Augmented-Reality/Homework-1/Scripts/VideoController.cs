using UnityEngine;
using UnityEngine.Video; 
using Vuforia;

public class VideoController : MonoBehaviour
{
    private ObserverBehaviour observerBehaviour;
    public VideoPlayer videoPlayer; 

    void Start()
    {
        observerBehaviour = GetComponent<ObserverBehaviour>();

        if (observerBehaviour)
        {
            observerBehaviour.OnTargetStatusChanged += OnTargetStatusChanged;
        }

        videoPlayer.Stop();
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
        if (targetStatus.Status == Status.TRACKED || targetStatus.Status == Status.EXTENDED_TRACKED)
        {
            OnTargetFound();
        }
        else if (targetStatus.Status == Status.NO_POSE)
        {
            OnTargetLost();
        }
    }

    private void OnTargetFound()
    {
        if (!videoPlayer.isPlaying)
        {
            videoPlayer.Play();
        }
    }

    private void OnTargetLost()
    {
        if (videoPlayer.isPlaying)
        {
            videoPlayer.Pause(); 
        }
    }
}