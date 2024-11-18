using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CarController : MonoBehaviour
{
    private Transform car;
    public Transform frontLeftWheel;
    public Transform frontRightWheel;
    public Joystick movementJoystick;

    public float moveSpeed = 5f; // H�z de�i�keni
    public float turnSpeed = 2f; // D�n�� h�z�
    private float currentSpeed = 0f; // Mevcut h�z

    private void Start()
    {
        car = GetComponent<Transform>();
    }

    private void Update()
    {
        float speedThreshold = 0.00001f;
        if (movementJoystick.Direction.y > speedThreshold)
        {
            HandleCarMovement("W");
        }
        else if (movementJoystick.Direction.y < -speedThreshold)
        {
            HandleCarMovement("S");
        }
        else
        {
            HandleCarMovement(""); 
        }

        // Joystick'in sa�/sol hareketini kontrol et
        if (movementJoystick.Direction.x > speedThreshold)
        {
            HandleCarMovement("D");
        }
        else if (movementJoystick.Direction.x < -speedThreshold)
        {
            HandleCarMovement("A");
        }
        else
        {
            HandleCarMovement("");
        }
    }

    public void HandleCarMovement(string movement)
    {
        // W tu�una bas�l�ysa h�zland�r
        if ((Input.GetKey(KeyCode.W) || movement.Equals("W")) && currentSpeed < 2)
        {
            currentSpeed += moveSpeed * Time.deltaTime * 1.2f; // H�z� art�r
        }
        else if ((Input.GetKey(KeyCode.S) || movement.Equals("S")))
        {
            currentSpeed -= moveSpeed * Time.deltaTime * 30f; // H�z� azalt
        }
        else
        {
            currentSpeed = Mathf.Max(currentSpeed - moveSpeed * Time.deltaTime, 0); // H�z s�f�r�n alt�na d��mesin
        }

        // D�n��
        if ((Input.GetKey(KeyCode.A) || movement.Equals("A")))
        {
            car.Rotate(0, -turnSpeed * Time.deltaTime, 0); // Sol d�n��
            RotateWheels(-turnSpeed * Time.deltaTime); // Tekerlekleri d�nd�r
        }
        else if ((Input.GetKey(KeyCode.D) || movement.Equals("D")))
        {
            car.Rotate(0, turnSpeed * Time.deltaTime, 0); // Sa� d�n��
            RotateWheels(turnSpeed * Time.deltaTime); // Tekerlekleri d�nd�r
        }

        // Arac�n hareketi
        car.position += car.forward * currentSpeed * Time.deltaTime;
        RotateWheels(currentSpeed * Time.deltaTime); 
    }

    private void RotateWheels(float rotationAmount)
    {
        float currentLeftRotation = frontLeftWheel.localEulerAngles.y;
        float currentRightRotation = frontRightWheel.localEulerAngles.y;

        // A��y� 0-360 aras�nda normalize ediyoruz
        if (currentLeftRotation > 180) currentLeftRotation -= 360;
        if (currentRightRotation > 180) currentRightRotation -= 360;

        // Sola ve sa�a d�nme hareketi
        if ((rotationAmount > 0 && currentLeftRotation < 60) || (rotationAmount < 0 && currentLeftRotation > -60))
        {
            frontLeftWheel.Rotate(0, rotationAmount, 0); // Sol tekerlek d�nd�rme
        }

        if ((rotationAmount > 0 && currentRightRotation < 60) || (rotationAmount < 0 && currentRightRotation > -60))
        {
            frontRightWheel.Rotate(0, rotationAmount, 0); // Sa� tekerlek d�nd�rme
        }

        // D�z gidildi�inde tekerlekleri s�f�r pozisyonuna d�nd�r
        if (rotationAmount == 0)
        {
            // Yava� yava� eski konumuna d�nd�rmek i�in Lerp kullan�yoruz
            frontLeftWheel.localEulerAngles = new Vector3(
                frontLeftWheel.localEulerAngles.x,
                Mathf.LerpAngle(frontLeftWheel.localEulerAngles.y, 0, Time.deltaTime * 5),
                frontLeftWheel.localEulerAngles.z
            );
            frontRightWheel.localEulerAngles = new Vector3(
                frontRightWheel.localEulerAngles.x,
                Mathf.LerpAngle(frontRightWheel.localEulerAngles.y, 0, Time.deltaTime * 5),
                frontRightWheel.localEulerAngles.z
            );
        }

    }
}
