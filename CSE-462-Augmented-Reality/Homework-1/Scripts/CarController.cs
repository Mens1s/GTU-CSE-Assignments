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

    public float moveSpeed = 5f; // Hýz deðiþkeni
    public float turnSpeed = 2f; // Dönüþ hýzý
    private float currentSpeed = 0f; // Mevcut hýz

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

        // Joystick'in sað/sol hareketini kontrol et
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
        // W tuþuna basýlýysa hýzlandýr
        if ((Input.GetKey(KeyCode.W) || movement.Equals("W")) && currentSpeed < 2)
        {
            currentSpeed += moveSpeed * Time.deltaTime * 1.2f; // Hýzý artýr
        }
        else if ((Input.GetKey(KeyCode.S) || movement.Equals("S")))
        {
            currentSpeed -= moveSpeed * Time.deltaTime * 30f; // Hýzý azalt
        }
        else
        {
            currentSpeed = Mathf.Max(currentSpeed - moveSpeed * Time.deltaTime, 0); // Hýz sýfýrýn altýna düþmesin
        }

        // Dönüþ
        if ((Input.GetKey(KeyCode.A) || movement.Equals("A")))
        {
            car.Rotate(0, -turnSpeed * Time.deltaTime, 0); // Sol dönüþ
            RotateWheels(-turnSpeed * Time.deltaTime); // Tekerlekleri döndür
        }
        else if ((Input.GetKey(KeyCode.D) || movement.Equals("D")))
        {
            car.Rotate(0, turnSpeed * Time.deltaTime, 0); // Sað dönüþ
            RotateWheels(turnSpeed * Time.deltaTime); // Tekerlekleri döndür
        }

        // Aracýn hareketi
        car.position += car.forward * currentSpeed * Time.deltaTime;
        RotateWheels(currentSpeed * Time.deltaTime); 
    }

    private void RotateWheels(float rotationAmount)
    {
        float currentLeftRotation = frontLeftWheel.localEulerAngles.y;
        float currentRightRotation = frontRightWheel.localEulerAngles.y;

        // Açýyý 0-360 arasýnda normalize ediyoruz
        if (currentLeftRotation > 180) currentLeftRotation -= 360;
        if (currentRightRotation > 180) currentRightRotation -= 360;

        // Sola ve saða dönme hareketi
        if ((rotationAmount > 0 && currentLeftRotation < 60) || (rotationAmount < 0 && currentLeftRotation > -60))
        {
            frontLeftWheel.Rotate(0, rotationAmount, 0); // Sol tekerlek döndürme
        }

        if ((rotationAmount > 0 && currentRightRotation < 60) || (rotationAmount < 0 && currentRightRotation > -60))
        {
            frontRightWheel.Rotate(0, rotationAmount, 0); // Sað tekerlek döndürme
        }

        // Düz gidildiðinde tekerlekleri sýfýr pozisyonuna döndür
        if (rotationAmount == 0)
        {
            // Yavaþ yavaþ eski konumuna döndürmek için Lerp kullanýyoruz
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
