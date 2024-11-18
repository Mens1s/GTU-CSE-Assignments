/*===============================================================================
Copyright (c) 2019 PTC Inc. All Rights Reserved.

Copyright (c) 2015 Qualcomm Connected Experiences, Inc. All Rights Reserved.

Vuforia is a trademark of PTC Inc., registered in the United States and other
countries.
===============================================================================*/

using UnityEngine;
using UnityEngine.UI;
using Vuforia;

public class MenuOptions : MonoBehaviour
{
    public CameraSettings CameraSettings;
    public DeviceObserverSettings deviceObserverSettings;
    public Toggle DeviceObserverToggle;
    public Toggle AutofocusToggle;
    public Toggle FlashToggle;
    public Toggle StaticDTToggle;
    public OptionsConfig OptionsConfig;

    public bool IsDisplayed { get; private set; }
    
    protected virtual void Start()
    {
        VuforiaApplication.Instance.OnVuforiaStarted += OnVuforiaStarted;
    }

    void OnDestroy()
    {
        VuforiaApplication.Instance.OnVuforiaStarted -= OnVuforiaStarted;
    }

    void OnVuforiaStarted()
    {
        DeviceObserverToggle.isOn = VuforiaBehaviour.Instance.DevicePoseBehaviour.enabled;

        if (FlashToggle != null)
        {
            FlashToggle.interactable = VuforiaBehaviour.Instance.CameraDevice.IsFlashSupported();
        }

        UpdateUI();
    }

    public void UpdateUI()
    {
        if (DeviceObserverToggle && deviceObserverSettings)
            DeviceObserverToggle.isOn = deviceObserverSettings.IsDeviceObserverEnabled();

        if (FlashToggle && CameraSettings)
            FlashToggle.isOn = CameraSettings.IsFlashTorchEnabled();
        
        if (AutofocusToggle && CameraSettings)
            AutofocusToggle.isOn = CameraSettings.IsAutofocusEnabled();

        if (StaticDTToggle && deviceObserverSettings)
            StaticDTToggle.isOn = deviceObserverSettings.IsStaticDeviceTrackerEnabled();
    }

    public void ShowOptionsMenu(bool show)    
    {
        if (OptionsConfig && OptionsConfig.AnyOptionsEnabled())
        {
            var canvasGroup = GetComponent<CanvasGroup>();

            if (show)
                UpdateUI();

            canvasGroup.interactable = show;
            canvasGroup.blocksRaycasts = show;
            canvasGroup.alpha = show ? 1.0f : 0.0f;
            IsDisplayed = show;
        }
    }
}
