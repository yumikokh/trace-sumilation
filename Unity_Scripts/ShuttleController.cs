using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using OscUnity;

public class ShuttleController : MonoBehaviour
{
    public string RemoteIP = "127.0.0.1";
    public int SendToPort = 0;
    public int ListenerPort = 8810;
    public Transform controller;
    private Osc handler;
    private UDPPacketIO udp;

    private GameObject shuttleObj;

    public Vector3 sm0;

    void Start()
    {
        UDPPacketIO udp = new UDPPacketIO();
        Osc handler = new Osc();
        udp.init(RemoteIP, SendToPort, ListenerPort);
        handler.init(udp);

        shuttleObj = GameObject.Find("Shuttle");

        handler.SetAddressHandler("/shuttlePos/", getShuttlePos);

    }


    void Update()
    {

        shuttleObj.transform.position = sm0;


    }

    void getShuttlePos(OscMessage oscMessage)
    {
        sm0.x = -(float)oscMessage.Values[0];
        sm0.y = (float)oscMessage.Values[1];
        sm0.z = (float)oscMessage.Values[2];
        //sm0 /= 50;
    }
}