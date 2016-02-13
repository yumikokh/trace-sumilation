using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using OscUnity;

public class RacketController : MonoBehaviour
{
    public string RemoteIP = null;
    public int SendToPort = 0;
    public int ListenerPort = 8810;
    public Transform controller;
    private Osc handler;
    private UDPPacketIO udp;


    private GameObject racket001;
    private GameObject racket002;

    private GameObject shuttleObj;

    private GameObject playerObj;

    public Vector3 rac10;
    public Vector3 rac11;
    public Vector3 rac12;
    public Vector3 rac20;
    public Vector3 rac21;
    public Vector3 rac22;

    public Vector3 sh0;

    public Vector3 pl1;
    public Vector3 pl2;

    public float smaller = 1;

    public bool player1 = true;

    private Matrix4x4 masic4 = Matrix4x4.identity;
    //private Matrix4x4 masic5 = Matrix4x4.identity;

    private Vector3 V1;
    private Vector3 V2;
    private Vector3 V3;
    private Vector3 V4;
    private Vector3 V5;
    private Vector3 V6;

    void Start()
    {
        UDPPacketIO udp = new UDPPacketIO();
        Osc handler = new Osc();
        udp.init(RemoteIP, SendToPort, ListenerPort);
        handler.init(udp);

        racket001 = GameObject.Find("Racket01");
        racket002 = GameObject.Find("Racket02");

        handler.SetAddressHandler("/racket010", getRac10);
        handler.SetAddressHandler("/racket011", getRac11);
        handler.SetAddressHandler("/racket012", getRac12);
        handler.SetAddressHandler("/racket020", getRac20);
        handler.SetAddressHandler("/racket021", getRac21);
        handler.SetAddressHandler("/racket022", getRac22);

        shuttleObj = GameObject.Find("Shuttle");
        handler.SetAddressHandler("/shuttlePos/", getShuttlePos);

        playerObj = GameObject.Find("HMDCamera");
        handler.SetAddressHandler("/player01", getPlayer01Pos);
        handler.SetAddressHandler("/player02", getPlayer02Pos);


    }


    void Update()
    {
        V1 = rac11 - rac10;
        V2 = rac12 - rac10;
        V3 = Vector3.Cross(V1, V2);

        V1.Normalize(); V2.Normalize(); V3.Normalize();
        masic4.m00 = V1.x; masic4.m01 = V2.x; masic4.m02 = V3.x;
        masic4.m10 = V1.y; masic4.m11 = V2.y; masic4.m12 = V3.y;
        masic4.m20 = V1.z; masic4.m21 = V2.z; masic4.m22 = V3.z;
        masic4.m30 = rac10.x; masic4.m31 = rac10.y; masic4.m32 = rac10.z;

        matrixtotransform.SetTransformFromMatrix(racket001.transform, masic4);
        racket001.transform.position = rac10;

        V4 = rac21 - rac20;
        V5 = rac22 - rac20;
        V6 = Vector3.Cross(V4, V5);

        V4.Normalize(); V5.Normalize(); V6.Normalize();
        masic4.m00 = V4.x; masic4.m01 = V5.x; masic4.m02 = V6.x;
        masic4.m10 = V4.y; masic4.m11 = V5.y; masic4.m12 = V6.y;
        masic4.m20 = V4.z; masic4.m21 = V5.z; masic4.m22 = V6.z;
        masic4.m30 = rac20.x; masic4.m31 = rac20.y; masic4.m32 = rac20.z;

        matrixtotransform.SetTransformFromMatrix(racket002.transform, masic4);
        racket002.transform.position = rac20;

        shuttleObj.transform.position = sh0;

        if (player1)
        {
            playerObj.transform.position = pl1;
        }
        else
        {
            playerObj.transform.position = pl2;
        }

    }


    void getRac10(OscMessage oscMessage)
    {
        rac10.x = (float)oscMessage.Values[0];
        rac10.y = (float)oscMessage.Values[1];
        rac10.z = (float)oscMessage.Values[2];
        rac10 /= smaller;
    }

    void getRac11(OscMessage oscMessage)
    {
        rac11.x = (float)oscMessage.Values[0];
        rac11.y = (float)oscMessage.Values[1];
        rac11.z = (float)oscMessage.Values[2];
        rac11 /= smaller;
    }

    void getRac12(OscMessage oscMessage)
    {
        rac12.x = (float)oscMessage.Values[0];
        rac12.y = (float)oscMessage.Values[1];
        rac12.z = (float)oscMessage.Values[2];
        rac12 /= smaller;
    }

    void getRac20(OscMessage oscMessage)
    {
        rac20.x = (float)oscMessage.Values[0];
        rac20.y = (float)oscMessage.Values[1];
        rac20.z = (float)oscMessage.Values[2];
        rac20 /= smaller;
    }

    void getRac21(OscMessage oscMessage)
    {
        rac21.x = (float)oscMessage.Values[0];
        rac21.y = (float)oscMessage.Values[1];
        rac21.z = (float)oscMessage.Values[2];
        rac21 /= smaller;
    }

    void getRac22(OscMessage oscMessage)
    {
        rac22.x = (float)oscMessage.Values[0];
        rac22.y = (float)oscMessage.Values[1];
        rac22.z = (float)oscMessage.Values[2];
        rac22 /= smaller;
    }

    void getShuttlePos(OscMessage oscMessage)
    {
        sh0.x = (float)oscMessage.Values[0];
        sh0.y = (float)oscMessage.Values[1];
        sh0.z = (float)oscMessage.Values[2];
        sh0 /= smaller;
    }

    void getPlayer01Pos(OscMessage oscMessage)
    {
        pl1.x = (float)oscMessage.Values[0];
        pl1.y = (float)oscMessage.Values[1];
        pl1.z = (float)oscMessage.Values[2];
        pl1 /= smaller;
    }

    void getPlayer02Pos(OscMessage oscMessage)
    {
        pl2.x = (float)oscMessage.Values[0];
        pl2.y = (float)oscMessage.Values[1];
        pl2.z = (float)oscMessage.Values[2];
        pl2 /= smaller;
    }
}
