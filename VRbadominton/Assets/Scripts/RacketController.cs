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

    private GameObject player001;
    private GameObject player002;

    public Vector3 sm0;
	public Vector3 sm1;
	public Vector3 sm2;
    public Vector3 sm3;
    public Vector3 sm4;
    public Vector3 sm5;

    public Vector3 sh0;

    public Vector3 pl1;
    public Vector3 pl2;

    public float smaller = 100;
	
	private Matrix4x4 masic4 = Matrix4x4.identity;
    //private Matrix4x4 masic5 = Matrix4x4.identity;
	
	private Vector3 V1;
	private Vector3 V2;
	private Vector3 V3;
    private Vector3 V4;
    private Vector3 V5;
    private Vector3 V6;

	void Start () 
	{
		UDPPacketIO udp = new UDPPacketIO();
		Osc handler = new Osc ();
		udp.init(RemoteIP, SendToPort, ListenerPort);
		handler.init(udp);
		
		racket001 = GameObject.Find("Racket01");
        racket002 = GameObject.Find("Racket02");
		
		handler.SetAddressHandler("/racket010", sumou10);
        handler.SetAddressHandler("/racket011", sumou11);
        handler.SetAddressHandler("/racket012", sumou12);
        handler.SetAddressHandler("/racket020", sumou20);
        handler.SetAddressHandler("/racket021", sumou21);
        handler.SetAddressHandler("/racket022", sumou22);

        shuttleObj = GameObject.Find("Shuttle");
        handler.SetAddressHandler("/shuttlePos/", getShuttlePos);

        player001 = GameObject.Find("Player01");
        player002 = GameObject.Find("Player02");
        handler.SetAddressHandler("/player01", getPlayer01Pos);
        handler.SetAddressHandler("/player02", getPlayer02Pos);


	}
	

	void Update () 
	{
		V1 = sm2 - sm0;
		V2 = sm1 - sm0;
		V3 = Vector3.Cross (V1, V2);
        
        V1.Normalize(); V2.Normalize(); V3.Normalize();
        masic4.m00 = V1.x; masic4.m01 = V2.x; masic4.m02 = V3.x;
        masic4.m10 = V1.y; masic4.m11 = V2.y; masic4.m12 = V3.y;
        masic4.m20 = V1.z; masic4.m21 = V2.z; masic4.m22 = V3.z;
        masic4.m30 = sm0.x; masic4.m31 = sm0.y; masic4.m32 = sm0.z;

        matrixtotransform.SetTransformFromMatrix(racket001.transform, masic4);
        racket001.transform.position = sm0;

        V4 = sm5 - sm3;
        V5 = sm4 - sm3;
        V6 = Vector3.Cross(V4, V5);
		
        V4.Normalize(); V5.Normalize(); V6.Normalize();
        masic4.m00 = V4.x; masic4.m01 = V5.x; masic4.m02 = V6.x;
        masic4.m10 = V4.y; masic4.m11 = V5.y; masic4.m12 = V6.y;
        masic4.m20 = V4.z; masic4.m21 = V5.z; masic4.m22 = V6.z;
        masic4.m30 = sm3.x; masic4.m31 = sm3.y; masic4.m32 = sm3.z;
		
        matrixtotransform.SetTransformFromMatrix(racket002.transform, masic4);
        racket002.transform.position = sm3;

        shuttleObj.transform.position = sh0;

        player001.transform.position = pl1;
        player002.transform.position = pl2;

	}
	void sumou10(OscMessage oscMessage)
	{	
		sm0.x = -(float)oscMessage.Values[0];
		sm0.y = (float)oscMessage.Values[1];
		sm0.z = (float)oscMessage.Values[2];
		sm0 /= smaller;
	} 
	
	void sumou11(OscMessage oscMessage)
	{	
		sm1.x = -(float)oscMessage.Values[0];
		sm1.y = (float)oscMessage.Values[1];
		sm1.z = (float)oscMessage.Values[2];
        sm1 /= smaller;
	} 
	
	void sumou12(OscMessage oscMessage)
	{	
		sm2.x = -(float)oscMessage.Values[0];
		sm2.y = (float)oscMessage.Values[1];
		sm2.z = (float)oscMessage.Values[2];
        sm2 /= smaller;
	}

    void sumou20(OscMessage oscMessage)
    {
        sm3.x = -(float)oscMessage.Values[0];
        sm3.y = (float)oscMessage.Values[1];
        sm3.z = (float)oscMessage.Values[2];
        sm3 /= smaller;
    }

    void sumou21(OscMessage oscMessage)
    {
        sm4.x = -(float)oscMessage.Values[0];
        sm4.y = (float)oscMessage.Values[1];
        sm4.z = (float)oscMessage.Values[2];
        sm4 /= smaller;
    }

    void sumou22(OscMessage oscMessage)
    {
        sm5.x = -(float)oscMessage.Values[0];
        sm5.y = (float)oscMessage.Values[1];
        sm5.z = (float)oscMessage.Values[2];
        sm5 /= smaller;
    }

    void getShuttlePos(OscMessage oscMessage)
    {
        sh0.x = -(float)oscMessage.Values[0];
        sh0.y = (float)oscMessage.Values[1];
        sh0.z = (float)oscMessage.Values[2];
        sh0 /= smaller;
    }

    void getPlayer01Pos(OscMessage oscMessage)
    {
        pl1.x = -(float)oscMessage.Values[0];
        pl1.y = (float)oscMessage.Values[1];
        pl1.z = (float)oscMessage.Values[2];
        pl1 /= smaller;
    }

    void getPlayer02Pos(OscMessage oscMessage)
    {
        pl2.x = -(float)oscMessage.Values[0];
        pl2.y = (float)oscMessage.Values[1];
        pl2.z = (float)oscMessage.Values[2];
        pl2 /= smaller;
    }
}
