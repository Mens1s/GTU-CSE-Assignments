using UnityEngine;
using System.IO;
using System.Globalization;
using System.Collections.Generic;
using TMPro;

public class CameraPos : MonoBehaviour
{
    public GameObject cameraObject;
    public TMP_Text infoText;

    private List<Dictionary<string, float>> cameraParameters = new List<Dictionary<string, float>>();
    private int index = 0;
    void Start()
    {
        //1
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", 1.48925721645f },
            { "camera_position_y", -0.696550130844f },
            { "camera_position_z", -3.27678751945f },
            { "camera_rotation_x", 0.96371086886f },
            { "camera_rotation_y", 0.0123908447297f },
            { "camera_rotation_z", 0.16223211587f },
            { "camera_rotation_w", 0.211633454026f },
            { "camera_forward_x", -0.307444930077f },
            { "camera_forward_y", 0.0925498306751f },
            { "camera_forward_z", 0.947054505348f  },
        }
        );
        //2
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", 0.296875715256f },
            { "camera_position_y", 0.0190966874361f },
            { "camera_position_z", -4.02261924744f },
            { "camera_rotation_x", 0.975588694464f },
            { "camera_rotation_y", 0.0084699724545f },
            { "camera_rotation_z", 0.12225485191f },
            { "camera_rotation_w", 0.182232957504f },
            { "camera_forward_x", -0.235453903675f },
            { "camera_forward_y", 0.0610841214657f },
            { "camera_forward_z", 0.969964265823f  },
        }
        );
        //3
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -4.27953624725f },
            { "camera_position_y", 0.0735201984644f },
            { "camera_position_z", -2.79945015907f },
            { "camera_rotation_x",0.998528970639f },
            { "camera_rotation_y", 0.0315285422968f },
            { "camera_rotation_z", -0.0246819064143f },
            { "camera_rotation_w",0.0365601488998f },
            { "camera_forward_x", 0.0515966154635f },
            { "camera_forward_y", 0.0611595995724f },
            { "camera_forward_z", 0.99679350853f  },
        }
        );

        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -4.73014640808f },
            { "camera_position_y", -0.000241667032242f },
            { "camera_position_z", -0.944474577904f },
            { "camera_rotation_x", 0.996702546716f },
            { "camera_rotation_y", 0.0202108545457f },
            { "camera_rotation_z", -0.0785086161144f },
            { "camera_rotation_w", 0.00347948523696f },
            { "camera_forward_x", 0.156640186906f },
            { "camera_forward_y", 0.0397420711815f },
            { "camera_forward_z", 0.986855864525f },
        });

        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -2.60007762909f },
            { "camera_position_y", 0.101600475609f },
            { "camera_position_z", -1.43247294426f },
            { "camera_rotation_x", 0.995275063761f },
            { "camera_rotation_y", 0.0130476990679f },
            { "camera_rotation_z", -0.00179449050273f },
            { "camera_rotation_w", 0.0961977971863f },
            { "camera_forward_x", 0.00608225772157f },
            { "camera_forward_y", 0.0256268177181f },
            { "camera_forward_z", 0.999653041363f },
        });
        //21
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -2.54693961143f },
            { "camera_position_y", -1.7700650692f },
            { "camera_position_z", -0.523337721825f },
            { "camera_rotation_x", 0.983035508343f },
            { "camera_rotation_y", 0.145546723193f },
            { "camera_rotation_z", 0.0168217403158f },
            { "camera_rotation_w", 0.11033725982f },
            { "camera_forward_x", -0.000954277755227f },
            { "camera_forward_y", 0.289867281914f },
            { "camera_forward_z", 0.95706641674f },
        });

        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -0.962511122227f },
            { "camera_position_y", -1.18648850918f },
            { "camera_position_z", -0.640179157257f },
            { "camera_rotation_x", 0.981974642146f },
            { "camera_rotation_y", 0.0956513886462f },
            { "camera_rotation_z", 0.0496235612945f },
            { "camera_rotation_w", 0.155287728515f },
            { "camera_forward_x", -0.0677511766553f },
            { "camera_forward_y", 0.203266337514f },
            { "camera_forward_z", 0.976777017117f },
        });
        //23
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", 0.255298703909f },
            { "camera_position_y", -0.00187779404223f },
            { "camera_position_z", -0.517998874187f },
            { "camera_rotation_x", 0.977372726023f },
            { "camera_rotation_y", 0.00842838576047f },
            { "camera_rotation_z", 0.0952260245091f },
            { "camera_rotation_w", 0.188689755345f },
            { "camera_forward_x", -0.182961910963f },
            { "camera_forward_y", 0.0524117015302f },
            { "camera_forward_z", 0.981722354889f },
        });
        //24
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -0.883982300758f },
            { "camera_position_y", -1.18604385853f },
            { "camera_position_z", -3.29885053635f },
            { "camera_rotation_x", 0.985362957377f },
            { "camera_rotation_y", 0.0621171446452f },
            { "camera_rotation_z", 0.0303315897888f },
            { "camera_rotation_w", 0.155824717455f },
            { "camera_forward_x", -0.0404164381325f },
            { "camera_forward_y", 0.131868630648f },
            { "camera_forward_z", 0.990442991257f },
        });
        //25    
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -0.352707624435f },
            { "camera_position_y", -0.00739498436451f },
            { "camera_position_z", -6.20341539383f },
            { "camera_rotation_x", 0.987524074249f },
            { "camera_rotation_y", 0.00474538555913f },
            { "camera_rotation_z", 0.0577657630005f },
            { "camera_rotation_w", 0.146413384437f },
            { "camera_forward_x", -0.112700603902f },
            { "camera_forward_y", 0.0262877251953f },
            { "camera_forward_z", 0.993281543255f },
        });
        //26
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -2.5562620163f },
            { "camera_position_y", -1.01465344429f },
            { "camera_position_z", 1.09403157234f },
            { "camera_rotation_x", 0.989311164103f },
            { "camera_rotation_y", 0.0940598600838f },
            { "camera_rotation_z", -0.00791893399094f },
            { "camera_rotation_w", 0.111146714931f },
            { "camera_forward_x", 0.0365774668753f },
            { "camera_forward_y", 0.184348613024f },
            { "camera_forward_z", 0.982180297375f },
        });
        //27
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -2.42138433456f },
            { "camera_position_y", -0.550716280937f },
            { "camera_position_z", 1.77312111855f },
            { "camera_rotation_x", 0.984292246353f },
            { "camera_rotation_y", 0.106077562848f },
            { "camera_rotation_z", -0.128824270733f },
            { "camera_rotation_w", -0.0576255268302f },
            { "camera_forward_x", 0.241375923157f },
            { "camera_forward_y", 0.223669812083f },
            { "camera_forward_z", 0.944303810596f },
        });
        //28
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -2.05876779556f },
            { "camera_position_y", -0.335703283548f },
            { "camera_position_z", 2.49281144142f },
            { "camera_rotation_x", 0.965707307536f },
            { "camera_rotation_y", 0.076791386971f },
            { "camera_rotation_z", -0.187603883492f },
            { "camera_rotation_w", -0.162227191239f },
            { "camera_forward_x", 0.337425708771f },
            { "camera_forward_y", 0.209184959531f },
            { "camera_forward_z", 0.917816221714f },
        });
        //29
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", 0.596497058868f },
            { "camera_position_y", 0.0378223955631f },
            { "camera_position_z", 0.535709023476f },
            { "camera_rotation_x", 0.978866247835f },
            { "camera_rotation_y", 0.0717974830276f },
            { "camera_rotation_z", 0.0635638147501f },
            { "camera_rotation_w", 0.180627131687f },
            { "camera_forward_x", -0.0985038727522f },
            { "camera_forward_y", 0.163522943854f },
            { "camera_forward_z", 0.98161059618f },
        });
        //30
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", 0.599378347397f },
            { "camera_position_y", 0.789136052132f },
            { "camera_position_z", 1.17848682404f },
            { "camera_rotation_x", 0.969841764893f },
            { "camera_rotation_y", 0.00925887118375f },
            { "camera_rotation_z", 0.126666384683f },
            { "camera_rotation_w", 0.208031179957f },
            { "camera_forward_x", -0.241840451956f },
            { "camera_forward_y", 0.0706603899598f },
            { "camera_forward_z", 0.967740118504f },
        });
        //31
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -1.42734694481f },
            { "camera_position_y", -0.982235252857f },
            { "camera_position_z", 0.259460270405f },
            { "camera_rotation_x", 0.978568054744f },
            { "camera_rotation_y", 0.137689396899f },
            { "camera_rotation_z", 0.00491626556179f },
            { "camera_rotation_w", 0.153043058139f },
            { "camera_forward_x", 0.0325230844319f },
            { "camera_forward_y", 0.270981818438f },
            { "camera_forward_z", 0.96203482151f },
        });
        //32
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -2.72251701355f },
            { "camera_position_y", -0.363232195377f },
            { "camera_position_z", 0.894587993622f },
            { "camera_rotation_x", 0.98588530165f },
            { "camera_rotation_y", 0.0907446673257f },
            { "camera_rotation_z", -0.139219941255f },
            { "camera_rotation_w", -0.0203397645663f },
            { "camera_forward_x", 0.270818382502f },
            { "camera_forward_y", 0.184590935707f },
            { "camera_forward_z", 0.944766700268f },
        });
        //33
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -3.93246459961f },
            { "camera_position_y", 0.322937369347f },
            { "camera_position_z", 0.210548639297f },
            { "camera_rotation_x", 0.991978653382f },
            { "camera_rotation_y", 0.0498062408979f },
            { "camera_rotation_z", -0.115961093572f },
            { "camera_rotation_w", -0.00713056256429f },
            { "camera_forward_x", 0.229351624846f },
            { "camera_forward_y", 0.100467137992f },
            { "camera_forward_z", 0.96814507246f },
        });
        //34
        cameraParameters.Add(new Dictionary<string, float>()
        {
            { "camera_position_x", -2.19480443001f },
            { "camera_position_y", 0.700502276421f },
            { "camera_position_z", -0.0692826211452f },
            { "camera_rotation_x", 0.99746277172f },
            { "camera_rotation_y", -0.0139431601794f },
            { "camera_rotation_z", -0.0649140805519f },
            { "camera_rotation_w", 0.0256918155921f },
            { "camera_forward_x", 0.128782182932f },
            { "camera_forward_y", -0.0311510749161f },
            { "camera_forward_z", 0.991183638573f },
        });

        LoadCameraParameters();
    }

    void Update()
    {

    }

    private void LoadCameraParameters()
    {
        cameraObject.transform.position = new Vector3(cameraParameters[index]["camera_position_x"], cameraParameters[index]["camera_position_y"], cameraParameters[index]["camera_position_z"]);
        cameraObject.transform.rotation = new Quaternion(cameraParameters[index]["camera_rotation_x"], cameraParameters[index]["camera_rotation_y"], cameraParameters[index]["camera_rotation_z"], cameraParameters[index]["camera_rotation_w"]);
        cameraObject.transform.forward = new Vector3(cameraParameters[index]["camera_forward_x"], cameraParameters[index]["camera_forward_y"], cameraParameters[index]["camera_forward_z"]);

        Debug.Log("Camera Position: " + cameraObject.transform.position);
        Debug.Log("Camera Rotation: " + cameraObject.transform.rotation);
        Debug.Log("Camera Forward: " + cameraObject.transform.forward);
    }
    
    public void NextCamera()
    {
        index++;
        infoText.text = "Camera " + (index + 1) + " / " + cameraParameters.Count;
        if (index == 19)
        {
            infoText.text = "Camera " + "1 / " + cameraParameters.Count;
        }
        if (index >= cameraParameters.Count)
        {
            index = 0;
        }
        LoadCameraParameters();
    }

    public void PreviousCamera()
    {
        index--;
        infoText.text = "Camera " + (index + 1) + " / " + cameraParameters.Count;
        if(index == 19) {
            infoText.text = "Camera " + "1 / " + cameraParameters.Count;
        }
        if (index < 0)
        {
            index = cameraParameters.Count - 1;
        }
        LoadCameraParameters();
    }
}
