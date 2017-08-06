using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class ViewLoginUI : MonoBehaviour {

    public Button loginButton;
    public InputField inputUser;
    public InputField inputPwd;
    void Awake()
    {
        loginButton = gameObject.transform.Find("login").GetComponent<Button>();
        inputUser = gameObject.transform.Find("InputUser").GetComponent<InputField>();
        inputPwd = gameObject.transform.Find("InputPwd").GetComponent<InputField>();

        loginButton.onClick.AddListener(() => onLoginClick(loginButton));
    }
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void onLoginClick(Button button)
    {
        NetLoginReq req = new NetLoginReq();
        req.user = inputUser.text;
        req.password = inputPwd.text;
        GameStart.socketClient.Send(req);
        //GameStart.textDebug.text = "onLoginClick";
    }
}
