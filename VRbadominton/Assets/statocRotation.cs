using UnityEngine;
using System.Collections;

public class statocRotation : MonoBehaviour
{
    public GameObject objTarget;

    // Use this for initialization
    void Start()
    {
        updatePosition();
    }

    // Update is called once per frame
    void Update()
    {
        updatePosition();
    }

    void updatePosition()
    {
        Vector3 pos = objTarget.transform.localPosition;
        transform.localPosition = pos;
    }
}
