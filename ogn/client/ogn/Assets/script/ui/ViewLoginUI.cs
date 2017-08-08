using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class ViewLoginUI : MonoBehaviour
{

    public Button loginButton;
    public InputField inputUser;
    public InputField inputPwd;
    void Awake()
    {
        loginButton = gameObject.transform.Find("loginbak/login").GetComponent<Button>();
        inputUser = gameObject.transform.Find("loginbak/InputUser").GetComponent<InputField>();
        inputPwd = gameObject.transform.Find("loginbak/InputPwd").GetComponent<InputField>();

    }
	// Use this for initialization
	void Start () {
        loginButton.onClick.AddListener(() => onLoginClick(loginButton));
    }

    // Update is called once per frame
    void Update () {
	}

    void onLoginClick(Button button)
    {
        if (GameStart.socketClient == null)
        {
            GameStart.gameStart.Connect("127.0.0.1", 13380);
            return;
        }
        NetLoginReq req = new NetLoginReq();
        req.user = inputUser.text;
        req.password = inputPwd.text;
        GameStart.gameStart.SendPacket(req, GameStart.socketClient.socket);
        //GameStart.textDebug.text = "onLoginClick";
    }
}
