#ifndef WEBDSP_H
#define WEBDSP_H

#include "Arduino.h"
#include <WebServer.h>
#include <btAudio.h>
#include <WiFi.h>
#include "PreferencesManager.h"

class webDSP {
public:
    webDSP();
    void begin(const char* ssid, const char* password, btAudio* audio);
    WebServer _server;

private:
    btAudio* _audio;
    void handleRoot();
    void handleCss();
    void handleGet();
};

const char PROGMEM html2[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
    <link rel="stylesheet" type="text/css" href="workStyle.css">
    <title>DSP</title>
    <style>
        .tab {
            overflow: hidden;
            border: 1px solid #ccc;
            background-color: #f1f1f1;
        }
        .tab button {
            background-color: inherit;
            float: left;
            border: none;
            outline: none;
            cursor: pointer;
            padding: 14px 16px;
            transition: 0.3s;
        }
        .tab button:hover {
            background-color: #ddd;
        }
        .tab button.active {
            background-color: #ccc;
        }
        .tabcontent {
            display: none;
            padding: 6px 12px;
            border: 1px solid #ccc;
            border-top: none;
        }
    </style>
</head>
<body>
    <center>
        <header>WiFi DSP</header>
        <div class="tab">
            <button class="tablinks" onclick="openTab(event, 'Volume')">Volume</button>
            <button class="tablinks" onclick="openTab(event, 'Filter')">Filter</button>
            <button class="tablinks" onclick="openTab(event, 'Compressor')">Compressor</button>
        </div>
        <div id="Volume" class="tabcontent">
            <h3>Volume Adjustment</h3>
            <form action="/get">
                <label for="volume">Volume:</label>
                <input type="text" id="volume" name="volume"><br>
                <input type="submit" value="Submit">
            </form>
        </div>
        <div id="Filter" class="tabcontent">
            <h3>Filter Adjustment</h3>
            <form action="/get">
                <label for="hpf">High Pass Filter:</label>
                <input type="text" id="hpf" name="hpf"><br>
                <label for="lpf">Low Pass Filter:</label>
                <input type="text" id="lpf" name="lpf"><br>
                <input type="submit" value="Submit">
            </form>
        </div>
        <div id="Compressor" class="tabcontent">
            <h3>Compressor Adjustment</h3>
            <form action="/get">
                <label for="thresh">Threshold:</label>
                <input type="text" id="thresh" name="thresh"><br>
                <label for="attack">Attack:</label>
                <input type="text" id="attack" name="attack"><br>
                <label for="release">Release:</label>
                <input type="text" id="release" name="release"><br>
                <label for="ratio">Ratio:</label>
                <input type="text" id="ratio" name="ratio"><br>
                <label for="kneeWidth">Knee Width:</label>
                <input type="text" id="kneeWidth" name="kneeWidth"><br>
                <label for="gain">Gain:</label>
                <input type="text" id="gain" name="gain"><br>
                <input type="submit" value="Submit">
            </form>
        </div>
    </center>
    <script>
        function openTab(evt, tabName) {
            var i, tabcontent, tablinks;
            tabcontent = document.getElementsByClassName("tabcontent");
            for (i = 0; i < tabcontent.length; i++) {
                tabcontent[i].style.display = "none";
            }
            tablinks = document.getElementsByClassName("tablinks");
            for (i = 0; i < tablinks.length; i++) {
                tablinks[i].className = tablinks[i].className.replace(" active", "");
            }
            document.getElementById(tabName).style.display = "block";
            evt.currentTarget.className += " active";
        }
    </script>
</body>
</html>
)rawliteral";

const char PROGMEM css2[] = R"rawliteral(
input[type=text], select {
    width: 40%;
    padding: 12px 20px;
    margin: 8px 0;
    border: 1px solid #ccc;
    border-radius: 4px;
}
#stop {
    width: 80%;
    padding: 12px 20px;
    margin: 8px 0;
}
)rawliteral";

#endif // WEBDSP_H