import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Window 2.2

import "./MaterialUI"
import "./MaterialUI/Element"
import "./MaterialUI/Interface"

import com.kevinlq.demo 1.0

Window {
    id: mainWindow;
    width: 800;
    height: 680;
    visible: true;
    //flags: Qt.FramelessWindowHint|Qt.Window|Qt.WindowMinMaxButtonsHint;
    title: qsTr("Demo");

    Component.onCompleted:
    {
        //materialUI.showLoading("please waiting....");
    }

    QInterface
    {
        id: interfaceApi;
    }

    Timer
    {
        id: time;
        interval: 1500;
        repeat: false;
        running: false;

        onTriggered: {
            materialUI.hideLoading();
            materialUI.showSnackbarMessage("this is test messge....");

            var jsonData = interfaceApi.initReposInfo("kevinlq");
            console.log("####jsonData:", jsonData);
        }
    }

    MaterialUI {
        id: materialUI;
        z: 2;
        x: 0;
        y: 0;
        width: parent.width;
        height: parent.height;
        dialogCancelText: "Cancel";
        dialogOKText: "Ok";
    }
}
