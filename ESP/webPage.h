#ifndef WEBPAGE_H
#define WEBPAGE_H

String getDashboardHTML(String esp32CamIp, String openAiApiKey) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta charset="UTF-8">
  <title>AI SPY ROBOT DASHBOARD</title>
  <style>
    body { background-color: #0f111a; color: white; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; text-align: center; margin: 0; padding: 15px; -webkit-user-select: none; user-select: none; }
    h2 { color: #00ffcc; margin-bottom: 5px; text-transform: uppercase; letter-spacing: 1px; }
    .camera-container { width: 100%; max-width: 400px; height: 240px; background-color: #000; border: 2px solid #00ffcc; margin: 10px auto; border-radius: 10px; overflow: hidden; display: flex; align-items: center; justify-content: center; position: relative; }
    .camera-container img { width: 100%; height: 100%; object-fit: cover; }
    .cam-input-container { display: flex; width: 100%; max-width: 400px; margin: 0 auto 15px auto; gap: 8px; }
    .cam-input { flex: 1; padding: 8px; border-radius: 5px; border: 1px solid #00ffcc; background: #1a1d2e; color: #fff; text-align: center; font-size: 0.85rem; }
    .btn-connect { background-color: #00ffcc; color: #000; border: none; padding: 8px 12px; border-radius: 5px; font-weight: bold; cursor: pointer; }
    .action-bar { display: flex; gap: 8px; width: 100%; max-width: 400px; margin: 0 auto 10px auto; }
    .btn-speak { flex: 1; background-color: #ff4d4d; color: white; padding: 14px; font-size: 0.95rem; font-weight: bold; border: none; border-radius: 12px; cursor: pointer; box-shadow: 0 4px 10px rgba(255, 77, 77, 0.3); transition: background-color 0.3s, transform 0.1s; }
    .btn-speak.active { background-color: #0088cc; box-shadow: 0 4px 12px rgba(0, 136, 204, 0.6); }
    .btn-speak:active { transform: scale(0.98); }
    .btn-photo { background-color: #ff9900; color: #000; padding: 14px 18px; font-size: 0.95rem; font-weight: bold; border: none; border-radius: 12px; cursor: pointer; box-shadow: 0 4px 10px rgba(255, 153, 0, 0.3); transition: background-color 0.2s, transform 0.1s; white-space: nowrap; }
    .btn-photo:active { transform: scale(0.95); background-color: #e68a00; }
    #status { color: #00ffcc; font-size: 0.85rem; min-height: 20px; margin-bottom: 15px; font-weight: bold; }
    .section-box { background: #1a1d2e; padding: 12px; border-radius: 10px; margin-bottom: 15px; border: 1px solid #333; max-width: 400px; margin-left: auto; margin-right: auto; }
    .section-title { color: #ffcc00; font-size: 0.95rem; margin-bottom: 8px; text-transform: uppercase; font-weight: bold; }
    .control-grid { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; justify-content: center; max-width: 280px; margin: 0 auto; }
    .btn-ctrl { background-color: #0088cc; color: white; padding: 15px 0; font-size: 0.9rem; border: none; border-radius: 8px; font-weight: bold; cursor: pointer; touch-action: manipulation; }
    .btn-ctrl:active { opacity: 0.8; }
    .btn-stop { background-color: #ff3333; }
    .btn-action { background-color: #2ecc71; color: white; padding: 8px 12px; margin: 2px; border: none; border-radius: 6px; font-weight: bold; cursor: pointer; touch-action: manipulation; font-size: 0.85rem; }
    .btn-sound { background-color: #9933ff; width: 100%; padding: 12px; font-size: 0.9rem; border: none; border-radius: 8px; color: white; font-weight: bold; cursor: pointer; margin-top: 5px; touch-action: manipulation; }
    .servo-row { display: flex; align-items: center; justify-content: space-between; margin: 8px 0; font-size: 0.85rem; color: #00ffcc; }
    .empty-slot { visibility: hidden; }
  </style>
</head>
<body>

  <h2>AI SPY ROBOT DASHBOARD</h2>

  <div class="cam-input-container">
    <input type="text" id="camUrl" class="cam-input" value=")rawliteral" + esp32CamIp + R"rawliteral(" placeholder="Camera URL (http://... or ngrok)">
    <button class="btn-connect" onclick="updateCamUrl()">Connect</button>
  </div>

  <div class="camera-container">
    <img id="cameraStream" src=")rawliteral" + esp32CamIp + R"rawliteral(" onerror="this.onerror=null; this.src=''; this.alt='Stream Offline';" alt="ESP32-CAM Stream">
  </div>

  <div class="action-bar">
    <button id="speakBtn" class="btn-speak" onclick="toggleContinuousListening()">Continuous Voice</button>
    <button class="btn-photo" onclick="takeSnapshot()">Photo</button>
  </div>

  <div id="status">Status: Mic Off</div>

  <div class="section-box">
    <div class="section-title">Drive Controls</div>
    <div class="control-grid">
      <div class="empty-slot"></div>
      <button class="btn-ctrl" ontouchstart="sendCommand('up')" onmousedown="sendCommand('up')">Up</button>
      <div class="empty-slot"></div>
      <button class="btn-ctrl" ontouchstart="sendCommand('left')" onmousedown="sendCommand('left')">Left</button>
      <button class="btn-ctrl btn-stop" onclick="sendCommand('stop')">Stop</button>
      <button class="btn-ctrl" ontouchstart="sendCommand('right')" onmousedown="sendCommand('right')">Right</button>
      <div class="empty-slot"></div>
      <button class="btn-ctrl" ontouchstart="sendCommand('down')" onmousedown="sendCommand('down')">Down</button>
      <div class="empty-slot"></div>
    </div>
  </div>

  <div class="section-box">
    <div class="section-title">Body Controls (Head & Hands)</div>

    <div class="servo-row">
      <span>HEAD PAN:</span>
      <div>
        <button class="btn-action" onclick="sendCommand('head_left')">Left</button>
        <button class="btn-action" onclick="sendCommand('head_center')">Center</button>
        <button class="btn-action" onclick="sendCommand('head_right')">Right</button>
      </div>
    </div>

    <div class="servo-row">
      <span>RIGHT ARM:</span>
      <div>
        <button class="btn-action" style="background:#0088cc" onclick="sendCommand('right_arm_up')">Up</button>
        <button class="btn-action" style="background:#e67e22" onclick="sendCommand('right_arm_down')">Down</button>
      </div>
    </div>

    <div class="servo-row">
      <span>LEFT ARM:</span>
      <div>
        <button class="btn-action" style="background:#0088cc" onclick="sendCommand('left_arm_up')">Up</button>
        <button class="btn-action" style="background:#e67e22" onclick="sendCommand('left_arm_down')">Down</button>
      </div>
    </div>

    <div style="margin-top: 10px;">
      <button class="btn-action" style="background:#9933ff" onclick="sendCommand('arms_up')">Both Up</button>
      <button class="btn-action" style="background:#555" onclick="sendCommand('arms_down')">Both Down</button>
    </div>
  </div>

  <div class="section-box">
    <button class="btn-sound" ontouchstart="sendCommand('playsound')" onmousedown="sendCommand('playsound')">Play ISD1820 Voice Alert</button>
  </div>

<script>
  const OPENAI_KEY = ")rawliteral" + openAiApiKey + R"rawliteral(";

  function updateCamUrl() {
    let url = document.getElementById('camUrl').value.trim();
    if (!url) return;
    if (url.endsWith('/')) url = url.slice(0, -1);
    const streamEndpoint = url.includes('/stream') ? url : url + '/stream';
    document.getElementById('cameraStream').src = streamEndpoint;
  }

  function takeSnapshot() {
    let url = document.getElementById('camUrl').value.trim();

    if (!url) {
      alert("Please enter the ESP32-CAM IP address first!");
      document.getElementById('status').innerText = 'Error: Enter ESP32-CAM IP';
      return;
    }

    if (!url.startsWith("http://") && !url.startsWith("https://")) {
      url = "http://" + url;
    }

    url = url.replace(/\/stream\/?$/, "").replace(/\/$/, "");

    let captureUrl;
    if (url.includes(":81")) {
      captureUrl = url + "/capture";
    } else {
      let ipOnly = url.replace(/:\d+$/, "");
      captureUrl = ipOnly + ":81/capture";
    }

    document.getElementById('status').innerText = 'Capturing Photo...';

    fetch(captureUrl)
      .then(response => {
        if (!response.ok) throw new Error("Camera capture network response failed");
        return response.blob();
      })
      .then(blob => {
        const downloadUrl = window.URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.style.display = 'none';
        a.href = downloadUrl;

        const dateStr = new Date().toISOString().replace(/[:.]/g, "-");
        a.download = `Robot_Photo_${dateStr}.jpg`;

        document.body.appendChild(a);
        a.click();
        window.URL.revokeObjectURL(downloadUrl);

        document.getElementById('status').innerText = 'Photo Saved!';
        speakText("Photo captured successfully!");
      })
      .catch(err => {
        console.error(err);
        document.getElementById('status').innerText = 'Capture Failed!';
        alert("Failed to capture photo! Make sure ESP32-CAM IP is reachable.");
      });
  }

  let activeController = null;

  function sendCommand(action) {
    document.getElementById('status').innerText = 'Executing: ' + action.toUpperCase();

    if (activeController) {
      activeController.abort();
    }

    activeController = new AbortController();

    fetch('/' + action, {
      cache: 'no-store',
      signal: activeController.signal
    }).catch(err => {
      if (err.name !== 'AbortError') console.log(err);
    });
  }

  const SpeechRecognition = window.SpeechRecognition || window.webkitSpeechRecognition;
  let recognition;
  let isContinuousOn = false;

  if (SpeechRecognition) {
    recognition = new SpeechRecognition();
    recognition.lang = 'en-US';
    recognition.continuous = true;
    recognition.interimResults = false;

    recognition.onstart = function() {
      document.getElementById('status').innerText = 'Mic Active (Always Listening...)';
    };

    recognition.onend = function() {
      if (isContinuousOn) {
        try {
          recognition.start();
        } catch (e) {
          console.log("Restarting mic loop...");
        }
      } else {
        document.getElementById('status').innerText = 'Status: Mic Off';
      }
    };

    recognition.onresult = async function(event) {
      const currentIdx = event.results.length - 1;
      const transcript = event.results[currentIdx][0].transcript.toLowerCase().trim();
      document.getElementById('status').innerText = 'Heard: "' + transcript + '"';

      if (transcript.includes('photo') || transcript.includes('picture') || transcript.includes('snap') || transcript.includes('capture') || transcript.includes('cheese')) {
        takeSnapshot();
      }
      else if (transcript.includes('right arm') || transcript.includes('arm right')) {
        if (transcript.includes('up') || transcript.includes('raise') || transcript.includes('lift')) {
          sendCommand('right_arm_up');
        } else if (transcript.includes('down') || transcript.includes('lower') || transcript.includes('drop')) {
          sendCommand('right_arm_down');
        }
      }
      else if (transcript.includes('left arm') || transcript.includes('arm left')) {
        if (transcript.includes('up') || transcript.includes('raise') || transcript.includes('lift')) {
          sendCommand('left_arm_up');
        } else if (transcript.includes('down') || transcript.includes('lower') || transcript.includes('drop')) {
          sendCommand('left_arm_down');
        }
      }
      else if (transcript.includes('both arms') || transcript.includes('raise hands') || transcript.includes('arms up')) {
        sendCommand('arms_up');
      } else if (transcript.includes('lower hands') || transcript.includes('arms down')) {
        sendCommand('arms_down');
      }
      else if (transcript.includes('head') || transcript.includes('look')) {
        if (transcript.includes('left')) sendCommand('head_left');
        else if (transcript.includes('right')) sendCommand('head_right');
        else if (transcript.includes('center') || transcript.includes('front') || transcript.includes('straight')) sendCommand('head_center');
      }
      else if (transcript.includes('forward') || transcript.includes('go') || transcript.includes('front') || transcript.includes('ahead') || transcript.includes('run') || transcript.includes('drive')) {
        sendCommand('up');
      } else if (transcript.includes('back') || transcript.includes('reverse') || transcript.includes('down') || transcript.includes('backward')) {
        sendCommand('down');
      } else if (transcript.includes('left') || transcript.includes('turn left')) {
        sendCommand('left');
      } else if (transcript.includes('right') || transcript.includes('turn right')) {
        sendCommand('right');
      } else if (transcript.includes('stop') || transcript.includes('halt') || transcript.includes('brake') || transcript.includes('wait')) {
        sendCommand('stop');
      }
      else if (transcript.includes('sound') || transcript.includes('alert') || transcript.includes('play sound') || transcript.includes('speak')) {
        sendCommand('playsound');
      }
      else if (transcript.split(' ').length >= 3) {
        document.getElementById('status').innerText = 'Asking ChatGPT...';
        const aiAnswer = await askChatGPT(transcript);
        speakText(aiAnswer);
      } else {
        document.getElementById('status').innerText = 'Unrecognized short command. Listening...';
      }
    };

    recognition.onerror = function(event) {
      if (event.error !== 'no-speech') {
        document.getElementById('status').innerText = 'Voice Error: ' + event.error;
      }
    };
  }

  function toggleContinuousListening() {
    const btn = document.getElementById('speakBtn');
    if (!SpeechRecognition) {
      document.getElementById('status').innerText = 'Speech Recognition Not Supported';
      return;
    }

    if (!isContinuousOn) {
      isContinuousOn = true;
      btn.innerText = 'Stop Continuous Voice Mode';
      btn.classList.add('active');
      recognition.start();
    } else {
      isContinuousOn = false;
      btn.innerText = 'Continuous Voice';
      btn.classList.remove('active');
      recognition.stop();
    }
  }

  async function askChatGPT(questionText) {
    try {
      const response = await fetch("https://api.openai.com/v1/chat/completions", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          "Authorization": "Bearer " + OPENAI_KEY
        },
        body: JSON.stringify({
          model: "gpt-3.5-turbo",
          messages: [
            { role: "system", content: "You are a smart AI spy robot companion. Keep answers under 2 sentences." },
            { role: "user", content: questionText }
          ],
          max_tokens: 80
        })
      });

      const data = await response.json();
      if (data.choices && data.choices.length > 0) {
        return data.choices[0].message.content;
      } else {
        return "I could not process that request.";
      }
    } catch (err) {
      return "Unable to reach ChatGPT servers.";
    }
  }

  function speakText(textToSpeak) {
    document.getElementById('status').innerText = 'Robot: ' + textToSpeak;
    window.speechSynthesis.cancel();
    const utterance = new SpeechSynthesisUtterance(textToSpeak);
    utterance.rate = 1.0;
    utterance.pitch = 1.0;
    window.speechSynthesis.speak(utterance);
  }
</script>

</body>
</html>
  )rawliteral";
  return html;
}

#endif